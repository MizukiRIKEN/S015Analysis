#! /bin/bash                                                                                                       

VERSION=2.3.0

RUNNUMBER1=("2900")
RUNNUMBER1=("2900" "2901" "2905" "2907" "2913" "2914" "2916" "2917" "2918")
RUNNUMBER1=("2900" "2901" "2905" "2907" "2914" "2916" "2917" "2918" "2919" "2920" "2921")
RUNNUMBER1=("2900" "2901" "2905" "2907" "2913" "2914" "2916" "2917" "2918" "2919" "2920" "2921")
MIXONLY=0
## 0 REAL and MIXed
## 1 REAL only
## 2 MIXed only


function v2exe_flatten() {
    typeset -i I=0
    
    if [ ${MIXONLY} -ne "2" ]; then
	MIX=0
	echo " REAL data --- "

	while(( I < ${#RUNNUMBER1[@]} ))
	do
            RUN=${RUNNUMBER1[I]}
	    LOG=asm${RUN}_v${VERSION}_${MIX}.log
	    echo RUN=${RUN} VER=${VERSION} BRUN=${BRUN} MIX=${MIX} BVER=${BVER} SBPR=${SBPR} root AsmFlw_getEvents.C  '>&' $LOG 
	    RUN=${RUN} VER=${VERSION} BRUN=${BRUN} MIX=${MIX} BVER=${BVER} SBPR=${SBPR} root -b -q AsmFlw_getEvents.C >& $LOG &
            let I++
	    if [ $I -ge ${#RUNNUMBER1[@]} ]; then
		break;
	    fi
	    
            RUN=${RUNNUMBER1[I]}
	    LOG=asm${RUN}_v${VERSION}_${MIX}.log
	    echo RUN=${RUN} VER=${VERSION} BRUN=${BRUN} MIX=${MIX} BVER=${BVER} SBPR=${SBPR} root AsmFlw_getEvents.C  '>&' $LOG 
	    RUN=${RUN} VER=${VERSION} BRUN=${BRUN} MIX=${MIX} BVER=${BVER} SBPR=${SBPR} root -b -q AsmFlw_getEvents.C >& $LOG &
            let I++
	    if [ $I -ge ${#RUNNUMBER1[@]} ]; then
		break;
	    fi
	    
            RUN=${RUNNUMBER1[I]}
	    LOG=asm${RUN}_v${VERSION}_${MIX}.log
	    echo RUN=${RUN} VER=${VERSION} BRUN=${BRUN} MIX=${MIX} BVER=${BVER} SBPR=${SBPR} root AsmFlw_getEvents.C  '>&' $LOG 
	    RUN=${RUN} VER=${VERSION} BRUN=${BRUN} MIX=${MIX} BVER=${BVER} SBPR=${SBPR} root -b -q AsmFlw_getEvents.C >& $LOG 
            let I++
	done
    fi

    if [ ${MIXONLY} -ne "1" ]; then
	echo " MIXed data --- "
	MIX=1
	I=0
	while(( I < ${#RUNNUMBER1[@]} ))
	do
	    RUN=${RUNNUMBER1[I]}
	    LOG=asm${RUN}_v${VERSION}_${MIX}.log
	    echo RUN=${RUN} VER=${VERSION} BRUN=${BRUN} MIX=${MIX} BVER=${BVER} SBPR=${SBPR} root AsmFlw_getEvents.C  '>&' $LOG 
	    RUN=${RUN} VER=${VERSION} BRUN=${BRUN} MIX=${MIX} BVER=${BVER} SBPR=${SBPR} root -b -q AsmFlw_getEvents.C >& $LOG &
	    let I++
	    if [ $I -ge ${#RUNNUMBER1[@]} ]; then
		break;
	    fi

	    RUN=${RUNNUMBER1[I]}
	    LOG=asm${RUN}_v${VERSION}_${MIX}.log
	    echo RUN=${RUN} VER=${VERSION} BRUN=${BRUN} MIX=${MIX} BVER=${BVER} SBPR=${SBPR} root AsmFlw_getEvents.C  '>&' $LOG 
	    RUN=${RUN} VER=${VERSION} BRUN=${BRUN} MIX=${MIX} BVER=${BVER} SBPR=${SBPR} root -b -q AsmFlw_getEvents.C >& $LOG &
	    let I++
	    if [ $I -ge ${#RUNNUMBER1[@]} ]; then
		break;
	    fi
	    
	    RUN=${RUNNUMBER1[I]}
	    LOG=asm${RUN}_v${VERSION}_${MIX}.log
	    echo RUN=${RUN} VER=${VERSION} BRUN=${BRUN} MIX=${MIX} BVER=${BVER} SBPR=${SBPR} root AsmFlw_getEvents.C  '>&' $LOG 
	    RUN=${RUN} VER=${VERSION} BRUN=${BRUN} MIX=${MIX} BVER=${BVER} SBPR=${SBPR} root -b -q AsmFlw_getEvents.C >& $LOG 
	    let I++
	done
    fi

}


v2exe_flatten

