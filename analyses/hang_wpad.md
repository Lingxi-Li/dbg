# Hang in WPAD

Application hangs for long within Web Proxy Auto Discovery (WPAD) code. WPAD is enabled by default on Windows. Code path is hit without user configuring any proxy setting. The issue is intermittent. Once hit, seems to consistently repro thereafter until reboot which seems to help mitigate (for a while).

## Scenario

- SSIS package execution on Azure with custom setup (Docker container)
- Script task that uses Azure Storage .NET SDK to upload blob failed with timeout

## Related Search Results

- [The Old New Thing blog post](https://devblogs.microsoft.com/oldnewthing/20200501-00/?p=103720)
- [Report on Dev Community](https://developercommunity.visualstudio.com/t/intermittent-and-indefinite-wcf-hang-blocking-requ/282756#T-N336412)

## Dump Analysis

- First thing is to locate the thread running script task. Know that script task functions reside in `ST_`-prefixed namespace.<br>
`dx @$curprocess.Threads.Where(t => t.Stack.Frames.Any(f => f.Attributes.SourceInformation.FunctionName.StartsWith("ST_")))`<br>
Located thread with index `27`.
- Call stack reveals thread was waiting on WPAD code path
- Open last dump and switch to thread `27`
- Find time spent on waiting with<br>
`.shell -ci "!runaway 7" findstr 27:`<br>
Top to bottom are user-mode, kernel-mode, elapsed time. Time spent on waiting is (`elapsed time` - `user-mode time` - `kernel-mode time`)

## Root Cause Analysis

The issue is confirmed to be a WinHTTP bug. The `WinHttpAutoProxySvc` service has `AutoIdleShutdown` enabled by default and self shuts down when idle for 27 mins. At each start, the service registers a new explicit RPC endpoint. This does not work well when the service is hosted in a shared long-standing `svchost` process. Every single process can register at most 500 explicit RPC endpoints and the service would fail to start when having restarted for 500 times already in the shared `svchost` process. The bug is fixed in Windows Server 2022 ([release notes](https://support.microsoft.com/en-gb/topic/october-25-2022-kb5018485-os-build-20348-1194-preview-becf7d1a-9482-4d56-955d-097e35b992a4)). As the time of this writing, no plan to backport the fix to Windows Server 2019.

The thread was stuck in waiting for service `WinHttpAutoProxySvc` to start. When this essential service is down, all system-wide downstream threads would [get stuck](https://serverfault.com/a/762987). [Disabling WPAD](https://learn.microsoft.com/en-us/troubleshoot/windows-server/networking/disable-http-proxy-auth-features#how-to-disable-wpad) does not help as that requires the service be up and running in the first place.

In the case of Docker container, `WinHttpAutoProxySvc` always runs in a shared `svchost` process and would hit the issue. On host machine with 8G+ memory, the service always runs in its own dedicated `svchost` process that terminates with the service, and is therefore free of this issue.


## Mitigation

For container case, first-choice mitigation is to patch the base image with Dockerfile and have the service run in dedicated svchost. For example,

~~~
FROM mcr.microsoft.com/windows/servercore:ltsc2019
RUN reg add HKLM\System\CurrentControlSet\Services\WinhttpAutoProxySvc /v Type /t REG_DWORD /d 0x10 /f
~~~

Other mitigations include

1. schedule regular OS restart to avoid reaching the 500 limit,
2. schedule regular WPAD workload (with less than 27 mins intermission) to keep the service busy.
