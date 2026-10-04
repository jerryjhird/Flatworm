<div align="center">
  <img src="assets/mascot.webp" alt="girardia dorotocephala" width="50%">

  # Flatworm

  [Documentation] | [Examples] | [Contributing]
</div>

---

**Flatworm** is a register oriented bytecode language inspired by real world instruction set architecture's like x86, arm and riscv

This repository contains an assembler, runtime and documentation (documentation can be limited)

[Documentation]: docs/table_of_contents.md
[Examples]: examples/
[Contributing]: CONTRIBUTING.md

## How to use

run `make` to compile the project. and after you can use either of the below commands, provided you have the correct files

### compiling the main example:
```bash
./flatworm compile examples/1.asm -o program.bin
```

### running the main example:
```bash
./flatworm run program.bin
```

### running the main example in debug mode:
```bash
DEBUG=1 ./flatworm run program.bin
```
