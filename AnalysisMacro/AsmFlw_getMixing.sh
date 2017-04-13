#! /bin/bash                                                                                                                                                          

ASMV=2
VERSION=0.1

#RUN=2900 ASMV=2 VER=0.1 MIX=0 root AsmFlw_getMixing.C
#RUN=2900 ASMV=2 VER=0.1 MIX=1 root AsmFlw_getMixing.C


RUNNUMBER1=("2901" "2905" "2907" "2913" "2914" "2916" "2917" "2918" "2919"
 "2920" "2921" "2922" "2924" "2925" "2926" "2927" "2928" "2929" "2930" "2931" "2932" "2933" "2934" "2935" "2936" "2937" "2938" "2939" "2940" "2941")

function v2exe_mix(){
    typeset -i I=0
    while(( I < ${#RUNNUMBER1[@]} ))
    do
        RUN=${RUNNUMBER1[I]}
	LOG=asmv2r${RUN}.log
        echo RUN${RUN} $LOG  ASMV:$ASMV version:$VERSION
	RUN=${RUN} ASMV=${ASMV} VER=${VERSION} MIX=0 root -b -q AsmFlw_getMixing.C >& $LOG &
        let I++

        RUN=${RUNNUMBER1[I]}
	LOG=asmv2r${RUN}.log
        echo RUN${RUN} $LOG  ASMV:$ASMV version:$VERSION
	RUN=${RUN} ASMV=${ASMV} VER=${VERSION} MIX=0 root -b -q AsmFlw_getMixing.C >& $LOG &
        let I++

        RUN=${RUNNUMBER1[I]}
	LOG=asmv2r${RUN}.log
        echo RUN${RUN} $LOG  ASMV:$ASMV version:$VERSION
	RUN=${RUN} ASMV=${ASMV} VER=${VERSION} MIX=0 root -b -q AsmFlw_getMixing.C >& $LOG 
        let I++
    done

    I=0
    while(( I < ${#RUNNUMBER1[@]} ))
    do
        RUN=${RUNNUMBER1[I]}
	LOG=asmv2m${RUN}.log
        echo RUN${RUN} $LOG  ASMV:$ASMV version:$VERSION
	RUN=${RUN} ASMV=${ASMV} VER=${VERSION} MIX=1 root -b -q AsmFlw_getMixing.C >& $LOG &
        let I++

        RUN=${RUNNUMBER1[I]}
	LOG=asmv2m${RUN}.log
        echo RUN${RUN} $LOG  ASMV:$ASMV version:$VERSION
	RUN=${RUN} ASMV=${ASMV} VER=${VERSION} MIX=1 root -b -q AsmFlw_getMixing.C >& $LOG &
        let I++

        RUN=${RUNNUMBER1[I]}
	LOG=asmv2m${RUN}.log
        echo RUN${RUN} $LOG  ASMV:$ASMV version:$VERSION
	RUN=${RUN} ASMV=${ASMV} VER=${VERSION} MIX=1 root -b -q AsmFlw_getMixing.C >& $LOG 
        let I++
    done





}


v2exe_mix
