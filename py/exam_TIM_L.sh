#!/bin/bash

L=$1
echo $L

cmd=./cpp/build/exam_eval_dg
for T in 1 5 10
do
	edges=./st_c2q/imm_L${L}K/L${L}Kp0.001_T${T}K.edges
	nodes=./st_c2q/imm_L${L}K/nodes_k10.dat

	# run TIM
	./run_TIM.py -k 10 $edges

done
# evaluating nodes value
$cmd -graph $edges -nodes $nodes -nodecay -eval_nodes
