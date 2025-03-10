# NucleusRV

[![Join the chat at https://gitter.im/merledu/nucleusrv](https://badges.gitter.im/merledu/nucleusrv.svg)](https://gitter.im/merledu/nucleusrv?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

A chisel based riscv 5-stage pipelined cpu design, implementing 32-bit version of the ISA (incomplete).


## Dependencies

* [`verilator`](https://verilator.org/guide/latest/install.html) v4.016 recommended by Chisel. Tested on v4.210.
* [`riscv-gnu-toolchain`](https://github.com/riscv/riscv-gnu-toolchain) To build the C program


## Build Instructions

#### Building with SBT
Run this command is SBT shell
```bash
testOnly nucleusrv.components.TopTest -- -DwriteVcd=1 -DprogramFile=/path/to/instructions/hex
```

#### Running RISCOF
**NOTE** Make sure to have Verilator, RISCV-GNU-Toolchain and Spike in your PATH

01.Set-up Riscof
```
python3 -m venv .venv
source .venv/bin/activate
pip install git+https://github.com/5hayanB/riscof
```
02.Update Config.ini to you NRV path
```
make update_config
```
03.Clone arch-tests and validate yaml
```
riscof --verbose info arch-test --clone
riscof validateyaml --config=config.ini
```

04.Trigger Compliance Tests (it's going to take a while so hold on)
```
riscof run --config=config.ini --suite=riscv-arch-test/riscv-test-suite/ --env=riscv-arch-test/riscv-test-suite/env
```

#### Running Compliance Tests
* Clone `riscv-arch-test` repo in nucleusrv root `git clone git@github.com:riscv-non-isa/riscv-arch-test.git -b 1.0`
* Build the simulation executable as defined in "Building with SBT" section
* Run `./run-compliance.sh` in root directory

#### Building C Programs
* In `tools/tests` directory, create a folder and write c program in the `main.c` file
* Run `make PROGRAM=<your_newly_created_test_folder_name`> inside tools directory
* Build the program with `sbt` command listed above. Make sure you are in root directory

* Optionally, you can skip writing/building c program and directly write hex instructions to `program.hex` file in `tools/out` directory.
