#! /bin/bash                                                                                                                                                          

VERSION=2.0.10
BRUN=0

RUNNUMBER1=("3032" "3034" "3036") 
###


function v2exe_flatten() {
    typeset -i I=0
    MIX=1

    while(( I < ${#RUNNUMBER1[@]} ))
    do
        RUN=${RUNNUMBER1[I]}
	LOG=asm${RUN}_v${VERSION}_${MIX}.log
        echo RUN${RUN} $LOG  version:$VERSION
	RUN=${RUN} VER=${VERSION} BRUN=${BRUN} MIX=${MIX} root -b -q AsmFlw_getFlatten.C\(-1\) >& $LOG &
        let I++
	if [ $I -ge ${#RUNNUMBER1[@]} ]; then
	    break;
	fi

        RUN=${RUNNUMBER1[I]}
	LOG=asm${RUN}_v${VERSION}_${MIX}.log
        echo RUN${RUN} $LOG  version:$VERSION
	RUN=${RUN} VER=${VERSION} BRUN=${BRUN} MIX=${MIX} root -b -q AsmFlw_getFlatten.C >& $LOG &
        let I++
	if [ $I -ge ${#RUNNUMBER1[@]} ]; then
	    break;
	fi

        RUN=${RUNNUMBER1[I]}
	LOG=asm${RUN}_v${VERSION}_${MIX}.log
        echo RUN${RUN} $LOG  version:$VERSION
	RUN=${RUN} VER=${VERSION} BRUN=${BRUN} MIX=${MIX} root -b -q AsmFlw_getFlatten.C >& $LOG 
        let I++
    done

}


v2exe_flatten

