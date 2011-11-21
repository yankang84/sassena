#!/usr/bin/python
#  -*- mode: python; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
# 
#  $Id$
# 
#  Copyright (c) Erik Lindahl, David van der Spoel 2003-2007.
#  Coordinate compression (c) by Frans van Hoesel.
#  Python wrapper (c) by Roland Schulz
# 
#  IN contrast to the rest of Gromacs, XDRFILE is distributed under the
#  BSD license, so you can use it any way you wish, including closed source:
# 
#  Permission is hereby granted, free of charge, to any person obtaining a
#  copy of this software and associated documentation files (the "Software"),
#  to deal in the Software without restriction, including without limitation
#  the rights to use, copy, modify, merge, publish, distribute, sublicense,
#  and/or sell copies of the Software, and to permit persons to whom the
#  Software is furnished to do so, subject to the following conditions:
# 
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
# 
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#  DEALINGS IN THE SOFTWARE.
# 
from xdrfile import *
from math import *

#you have to compile with --enable-shared
#and have libxdrfile.so in the LD_LIBRARY_PATH
#it expect the test.xtc/test.trr file generated by xdrfile_c_test

DIM=3
natoms1 = 173
step1 = 1993
prec1 = 1000
time1 = 1097.23
lam1 = 0.4
box1=[[(i+1)*3.7 + (j+1) for j in range(DIM)] for i in range(DIM)]
toler = 1e-3

def test(fn):
    x=xdrfile(fn)
    if x.natoms != natoms1: print "natoms != natoms1",x.natoms,natoms1
    for k,f in enumerate(x):
        if f.step != step1+k: print "incorrect step",f.step,step1+k,k
        if fabs(f.time-time1-k) > toler: print "incorrect time",f.time,time1+k
        if not x.mode&mTrr and fabs(f.prec-prec1) > toler: print "incorrect precision",f.prec,prec1
        if x.mode&mTrr and fabs(f.lam-lam1) > toler: print "incorrect lambda",f.lam,lam1
        for i in range(DIM):
            for j in range(DIM):
                if fabs(f.box[i][j] - box1[i][j]) > toler: print "box incorrect",f.box[i][j],box1[i][j]
        for i in range(x.natoms):
            for j in range(DIM):
                target = (i+1)*3.7 + (j+1)
                if  f.x[i][j] - target > toler : print "x incorrect"
    print fn,"OK"

test("../test.trr")
test("../test.xtc")