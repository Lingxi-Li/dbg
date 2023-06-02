# [Symbols](https://learn.microsoft.com/en-us/windows/win32/dxtecharts/debugging-with-symbols)

- Windows uses PDB format symbols
- Stripped PDB has no local variable, struct, and source code correlation info
- Symbols search order
    - Paths embedded in binary
    - Alongside binary
    - Local host cache
    - Local network symbol server
    - Internet symbol server
- symchk tool
- Symbols and binary are matched by their embedded GUIDs
- Microsoft [public symbol server](https://msdl.microsoft.com/download/symbols)
- Configure symbol search path via `_NT_SYMBOL_PATH` env. variable
    - Respected by all tools like VS, WinDbg
    - Syntax: `srv*[local cache]*[private symbol server]*[internet symbol server]`