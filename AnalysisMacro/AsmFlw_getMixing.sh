#! /bin/bash                                                                                                                                                          

VERSION=2.0.8
ROT=1


#LOG=asmr2900_v${VERSION}_${ROT}.log
#RUN=2900 VER=${VERSION} ROT=${ROT} MIX=0 root -b -q AsmFlw_getMixing.C >& $LOG 
#LOG=asmr2900_v${VERSION}_${ROT}.log
#RUN=2900 VER=${VERSION} ROT=${ROT} MIX=1 root -b -q AsmFlw_getMixing.C >& $LOG &


RUNNUMBER1=("2901" "2905" "2907" "2913" "2914" "2916" "2917" "2918" "2919")
#RUNNUMBER1=( "2920" "2921" "2922" "2924" "2925" "2926" "2927" "2928" "2929" "2930" "2931" "2932" "2933" "2934" "2935" "2936" "2937" "2938" "2939" "2940" "2941")

function v2exe_mix() {
    typeset -i I=0


    while(( I < ${#RUNNUMBER1[@]} ))
    do
        RUN=${RUNNUMBER1[I]}
	LOG=asmm${RUN}_v${VERSION}_${ROT}.log
        echo RUN${RUN} $LOG  version:$VERSION
	RUN=${RUN} VER=${VERSION} ROT=${ROT} MIX=1 root -b -q AsmFlw_getMixing.C >& $LOG &
        let I++

        RUN=${RUNNUMBER1[I]}
	LOG=asmm${RUN}_v${VERSION}_${ROT}.log
        echo RUN${RUN} $LOG  version:$VERSION
	RUN=${RUN} VER=${VERSION} ROT=${ROT} MIX=1 root -b -q AsmFlw_getMixing.C >& $LOG &
        let I++

        RUN=${RUNNUMBER1[I]}
	LOG=asmm${RUN}_v${VERSION}_${ROT}.log
        echo RUN${RUN} $LOG  version:$VERSION
	RUN=${RUN} VER=${VERSION} ROT=${ROT} MIX=1 root -b -q AsmFlw_getMixing.C >& $LOG 
        let I++
    done

    I=0
    while(( I < ${#RUNNUMBER1[@]} ))
    do
        RUN=${RUNNUMBER1[I]}
	LOG=asmr${RUN}_v${VERSION}_${ROT}.log
        echo RUN${RUN} $LOG  version:$VERSION
	RUN=${RUN} VER=${VERSION} ROT=${ROT} MIX=0 root -b -q AsmFlw_getMixing.C >& $LOG &
        let I++

        RUN=${RUNNUMBER1[I]}
	LOG=asmr${RUN}_v${VERSION}_${ROT}.log
        echo RUN${RUN} $LOG  version:$VERSION
	RUN=${RUN} VER=${VERSION} ROT=${ROT} MIX=0 root -b -q AsmFlw_getMixing.C >& $LOG &
        let I++

        RUN=${RUNNUMBER1[I]}
	LOG=asmr${RUN}_v${VERSION}_${ROT}.log
        echo RUN${RUN} $LOG  version:$VERSION
	RUN=${RUN} VER=${VERSION} ROT=${ROT} MIX=0 root -b -q AsmFlw_getMixing.C >& $LOG 
        let I++
    done

}


v2exe_mix
