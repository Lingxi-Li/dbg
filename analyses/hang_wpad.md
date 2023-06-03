# Hang in WPAD

Application hangs for long within Web Proxy Auto Discovery (WPAD) code. WPAD is enabled by default on Windows. Code path is hit without user configuring any proxy setting. The issue is intermittent. Once hit, seems to consistently repro thereafter until reboot which seems to help mitigate (for a while).

## Scenario

- SSIS package execution on Azure with custom setup (Docker container)
- Script task that uses Azure Storage .NET SDK to upload blob failed with timeout

## Related Search Results

- [The Old New Thing blog post](https://devblogs.microsoft.com/oldnewthing/20200501-00/?p=103720)
- [Report on Dev Community](https://developercommunity.visualstudio.com/t/intermittent-and-indefinite-wcf-hang-blocking-requ/282756#T-N336412)

## Analysis

- First thing is to locate the thread running script task. Know that script task functions reside in `ST_`-prefixed namespace.<br>
`dx @$curprocess.Threads.Where(t => t.Stack.Frames.Any(f => f.Attributes.SourceInformation.FunctionName.StartsWith("ST_")))`<br>
Located thread with index `27`.
- Call stack reveals thread was waiting on WPAD code path
- Open last dump and switch to thread `27`
- Find time spent on waiting with<br>
`.shell -ci "!runaway 7" findstr 27:`<br>
Top to bottom are user-mode, kernel-mode, elapsed time. Time spent on waiting is (`elapsed time` - `user-mode time` - `kernel-mode time`)
