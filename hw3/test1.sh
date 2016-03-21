#!/bin/sh
# Usage: test1.sh <sys#>
#
# shell file to test hw2
#  ~S for 10-sec wait, ~s for 1-sec wait
#  use 20 secs for reset, then CR, then 1 sec, then ~d, 10 secs for download
#  then run program and cause input of 01234567890..., one char/sec.
#  to simulate user input
mtip -b $1 -f test1.lnx << MRK
~r~S~S
~s~S~d~S~SGO 100100
~s0~s1~s2~s3~s4~s5~s6~s7~s8~s9~s0~s1~s2~s3~s4~s5~s6~s7~s8~s9~s~s~s~q
MRK






