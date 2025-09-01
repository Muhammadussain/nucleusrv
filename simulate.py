from os.path import \
    dirname, \
    abspath, \
    join, \
    isdir, \
    split, isfile
from subprocess import run
from argparse import ArgumentParser
from sys import exit
from shutil import rmtree
from os import walk, makedirs, chdir
from re import search
import glob

ROOT = dirname(
    abspath(__file__)
)

def execute_sp(cmd, **kwargs):
    exec_sp = run(cmd, **kwargs)
    exec_sp.check_returncode()
    if exec_sp.returncode != 0:
        exit(1)

if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('prog', help = 'Path to RISC-V assembly or C program')
    parser.add_argument(
        '--timing',
        action = 'store_const',
        const = '--timing',
        help = 'Enable timing support'
    )
    args = parser.parse_args()
    prog_name = split(args.prog)[-1]
    target_dir = join(ROOT, 'out', prog_name)
    elf = join(target_dir, prog_name.split('.')[0])
    if isdir(target_dir):
        rmtree(target_dir)
    makedirs(target_dir)
    # If the firmware is in babykyber, add the SDK driver source to the build
    extra_sources = []
    extra_includes = []
    if 'babykyber' in args.prog:
        # Check for new SDK structure first (include/, src/)
        sdk_include_dir = join(dirname(args.prog), 'include')
        sdk_src_dir = join(dirname(args.prog), 'src')

        if isdir(sdk_include_dir) and isdir(sdk_src_dir):
            # New structure: include/ and src/ directories
            extra_sources.extend(glob.glob(join(sdk_src_dir, '*.c')))
            extra_includes.append('-I' + sdk_include_dir)
        else:
            # Fallback to old structure: sdk/ directory
            sdk_dir = join(dirname(args.prog), 'sdk')
            if not isdir(sdk_dir):
                sdk_dir = join(dirname(dirname(args.prog)), 'sdk')
            if isdir(sdk_dir):
                # Add all .c files in the SDK directory
                extra_sources.extend(glob.glob(join(sdk_dir, '*.c')))
                extra_includes.append('-I' + sdk_dir)
            else:
                # Final fallback: look for HAL files in the same directory as the main file
                prog_dir = dirname(args.prog)
                hal_files = ['babykyber_hal.c', 'hal.c']  # common HAL filenames
                for hal_file in hal_files:
                    hal_path = join(prog_dir, hal_file)
                    if isfile(hal_path):
                        extra_sources.append(hal_path)
                # Add the program directory to include path for header files
                extra_includes.append('-I' + prog_dir)
    # Build the ELF including any extra SDK/HAL sources and include paths
    gcc_cmd = [
        'riscv32-unknown-elf-gcc',
        '-static',
        '-mcmodel=medany',
        '-fvisibility=hidden',
        '-nostdlib',
        '-nostartfiles',
        '-g',
        '-T', join(ROOT, "riscof", "nucleusrv", "env", "link.ld"),
        '-march=rv32if',
        '-mabi=ilp32f',
        '-o', elf,
        args.prog
    ] + extra_sources + extra_includes
    execute_sp(gcc_cmd, text = True)
    execute_sp(
        f'riscv32-unknown-elf-objdump -d -Mno-aliases {elf} > {elf}.objdump',
        shell = True,
        text = True
    )
    execute_sp((
        'riscv32-unknown-elf-objcopy',
        '-O', 'binary',
        '-j', '.text',
        elf,
        join(target_dir, 'imem.bin')
    ), text = True)
    execute_sp((
        'riscv32-unknown-elf-objcopy',
        '-O', 'binary',
        '-j', '.data',
        elf,
        join(target_dir, 'dmem.bin')
    ), text = True)
    execute_sp(
        f'hexdump -v -e \'1/4 "%08x\\n"\' {join(target_dir, "imem.bin")} > {join(target_dir, "imem.hex")}',
        shell = True,
        text = True
    )
    execute_sp(
        f'hexdump -v -e \'1/4 "%08x\\n"\' {join(target_dir, "dmem.bin")} > {join(target_dir, "dmem.hex")}',
        shell = True,
        text = True
    )
    # Invoke sbt in a way that targets the `nucleusrv` subproject when running from repo root.
    # This works whether simulate.py is run from the repo root or from inside `nucleusrv`.
    sbt_cmd = (
        f"sbt 'project nucleusrv' 'runMain nucleusrv.components.NRVDriver --imem {join(target_dir, 'imem.hex')} --dmem {join(target_dir, 'dmem.hex')} --target-dir {target_dir}'"
    )
    execute_sp(sbt_cmd, shell = True, text = True)
    sv_files = tuple(
        join(target_dir, file)
        for file in next(walk(target_dir))[-1]
            if search(r'\.s?v$', file)
    )
    # Ensure the testbench source path is absolute so the generated Makefile in --Mdir
    # can find it when make runs inside the obj_dir.
    tb_cpp = join(ROOT, 'nrv_tb.cpp')
    execute_sp((
        'verilator',
        '--cc',
        '--exe',
        '--build',
        '--trace',
        '-Wno-fatal',
        '-Wno-lint',
        '-Wno-style',
        '--no-timing' if args.timing is None \
            else args.timing,
        '-j', '0',
        '--top', 'Top',
        '--Mdir', join(target_dir, 'obj_dir'),
        tb_cpp
    ) + sv_files, text = True)
    chdir(target_dir)
    execute_sp([join(
        target_dir,
        'obj_dir',
        'VTop'
    )], text = True)
    chdir(ROOT)