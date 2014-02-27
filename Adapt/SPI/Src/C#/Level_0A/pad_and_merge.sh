#! /usr/bin/bash

dd if=tinyclr.bin bs=3M count=1 of=MFout.bin conv=sync

cat SPI_Level_0A.dat >> MFout.bin

