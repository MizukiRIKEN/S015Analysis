#! /bin/bash
#RUN=2900 VER=0 root -b -q Assemble_flwv2.C
VERSION=0

RUNNUMBER1=("2901" "2905" "2907" "2913" "2914" "2916" "2917" "2918" "2919"
 "2920" "2921" "2922" "2924" "2925" "2926" "2927" "2928" "2929" "2930" "2931" "2932" "2933" "2934" "2935" "2936" "2937" "2938" "2939" "2940" "2941")

function v2exe1(){
    typeset -i I=0
    while(( $I < ${#RUNNUMBER1[@]} ))
    do
	RUN=${RUNNUMBER1[I]} 
	echo RUN${RUN} asmv2run${RUN}.log
	RUN=${RUNNUMBER1[$I]} VER=$VERSION root -b -q Assemble_flwv2.C >& asmv2run${RUN}.log &

	let I++
	RUN=${RUNNUMBER1[I]} 
	echo RUN${RUN} asmv2run${RUN}.log
	RUN=${RUNNUMBER1[I]} VER=$VERSION root -b -q Assemble_flwv2.C >& asmv2run${RUN}.log &

	let I++
	RUN=${RUNNUMBER1[I]} 
	echo RUN${RUN} asmv2run${RUN}.log
	RUN=${RUNNUMBER1[I]} VER=$VERSION root -b -q Assemble_flwv2.C >& asmv2run${RUN}.log 

	let I++
    done
}


v2exe1 





