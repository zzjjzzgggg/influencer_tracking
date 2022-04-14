#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os
import sys
import argparse
import subprocess as sp
from collections import Counter

import iotools as io
from utils import Timer, pretty_number

from gen_IC_weights import *


def runIMM(dirname, k):
    cmd = "./IMM/imm_discrete -dataset {}/ -epsilon 0.1 -k {} -model IC".format(
        dirname, k)
    out = sp.check_output(cmd, stderr=sp.STDOUT, shell=True).decode('utf8')

    mark_beg, mark_end = 'g.seedSet=', ' \n'
    pos1 = out.find(mark_beg)
    if pos1 < 0:
        print("Failed to find string '{}'. Exiting!".format(mark_beg))
        system.exit(-1)

    pos1 += len(mark_beg)
    pos2 = out.find(mark_end, pos1)
    if pos2 < 0:
        print("Failed to find string '{}'. Exiting!".format(mark_end))
        system.exit(-1)

    seeds = map(int, out[pos1:pos2].split(" "))

    id_nd = io.loadIntMap(os.path.join(dirname, "nd_id.map"), 1, 0)
    nodes = [id_nd[seed] for seed in seeds]

    io.saveList(nodes, os.path.join(dirname, "nodes_k{}.dat".format(k)))


if __name__ == '__main__':
    tm = Timer()
    parser = argparse.ArgumentParser()
    parser.add_argument('input', help='edge list filename')
    parser.add_argument('-k', type=int, default=10, help='budget')
    args = parser.parse_args()

    print("generating IC model weights...")
    dirname = genWeights(args.input)

    print("running IMM...")
    runIMM(dirname, args.k)

    print("cost time", tm.tmstr())
