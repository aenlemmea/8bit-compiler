#!/usr/bin/env bash

set -e

prg=$1

if [ $# -eq 0 ]; then
    echo "Usage: ./driver.sh <filename_without_extension>"
    echo "Do not supply extensions"
    exit 1;
fi

prg="$1"
asmprg="$(pwd)/$prg.asm"
simprg="$(pwd)/$prg.simpl"
asm_py=../lib/8bit-computer/asm/asm.py

if [[ ! -d ../build/ ]]; then
    echo "Please build the project first.";
fi

mkdir -p ../out/

if [[ -d ../lib/8bit-computer/asm/ ]]; then

    echo "Generating memory.list"
    ../build/etbit $simprg
    python3 $asm_py $asmprg > ../out/memory.list
    echo "Copying memory.list to lib/8bit-computer"
    cp ../out/memory.list ../lib/8bit-computer/
    echo "Running: "
    cd ../lib/8bit-computer/ && make run
    echo "Cleaning up"
    make clean
else
    echo "Please recursively clone the repo. Submodules are not generated";
    exit 0
fi

