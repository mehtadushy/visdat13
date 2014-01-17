#!/bin/sh
export QTDIR=/installer/arch/
export PATH=$QTDIR/bin:$PATH
exec kdevelop $*
