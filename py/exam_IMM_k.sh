#!/bin/bash
k=$2
echo $k

cmd=./cpp/build/exam_eval_dg
for T in 1 5 10
do
	  edges=./$1/imm_L10K/L10Kp0.001_T${T}K.edges
	  nodes=./$1/imm_L10K/nodes_k$k.dat

    # run IMM
    ./run_IMM.py -k $k $edges
done

# evaluating nodes value
$cmd -graph $edges -nodes $nodes -nodecay -eval_nodes
