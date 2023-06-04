# WinDbg Commands

### General

- `.shell -ci "Command" findstr Key` (filter `Command` output)
- `.time`

## Data Model

- [`dx [-g #]-r[#] Expression[,d#]`](https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/dx--display-visualizer-variables-)
- Root is `Debugger`
- [Supports LINQ queries](https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/using-linq-with-the-debugger-objects#supported-linq-syntax---query-methods)

### Current Context

- `@$cursession`
- `@$curprocess`
- `@$curthread`
- `@$curstack`
- `@$curframe`

## Live Debugging

### Stepping

- `g` (go, continue, F5)
- `gu` (go upwards, step out, SHF + F11)
- `t` (trace, step into, F11)
- `p` (pass, step over, F10)

### Breakpoint

- `bp` (full symbol name or address)
- `bm` (symbol pattern)
- `bu` (break at point in module not loaded yet)
- `ba [r|w|e] [Size] Address` (break at data access)

### Thread

- `!runaway 7`
- `.ttime`
- `~ThreadIndex s`

### Process

- `.childdbg`
