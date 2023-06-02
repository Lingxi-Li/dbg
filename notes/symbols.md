# Symbols

## Concepts

- Windows uses PDB format symbols
- Stripped PDB has no local variable, struct, and source code correlation info
- Symbols search order
    - Paths embedded in binary
    - Alongside binary
    - `_NT_SYMBOL_PATH` env. variable that encodes
        - Local host cache
        - Local network symbol server
        - Internet symbol server
- symchk tool
- Symbols and binary are matched by their embedded GUIDs
- Microsoft [public symbol server](https://msdl.microsoft.com/download/symbols)
- Configure symbol search path via `_NT_SYMBOL_PATH` env. variable
    - Respected by all tools like VS, WinDbg
    - Syntax: `srv*[local cache]*[private symbol server]*[internet symbol server]`
- Symbol server may further correlate with *source (code) server*

## Set Up Debug Session

- Set up WinDbg default source/symbol/cache path
    - Use [AgeStore](https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/agestore) to trim local cache
- [`.sympath[+] [Path [; ...]]`](https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/-sympath--set-symbol-path-)
- `.reload /f`
- `lm` and expect `private pdb symbols` next to each loaded module
- To troubleshoot symbol loading ([wiki](https://www.osgwiki.com/wiki/WinDbg_102_-_Setting_up_WinDbg#Trouble_in_loading_private_symbols))
    - `!sym noisy` enables symbol loading verbose output
    - `.reload /f` again
- [`.srcpath[+] [Directory [; ...]]`](https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/-srcpath---lsrcpath--set-source-path-)