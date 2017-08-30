#! /bin/bash                                                                                                       

VERSION=3.2.0

#RUNNUMBER1=("2900" "2901" "2902" "2903" "2904" "2905" "2907" "2914" "2916" "2917" "2919" "2920" "2921" "2922")

#RUNNUMBER1=("2857" "2858" "2859" "2860" "2861" "2875" "2877" "2878" "2879" "2880" "2881" "2882")

RUNNUMBER1=("2883" "2884" "2887" "2888" "2889" "2890" "2891" "2892" "2893" "2894" "2896" "2898" "2899")
RUNNUMBER1=("2841" "2843" "2844" "2845" "2846" "2848" "2849" "2850" "2851" "2852" "2855" "2856")
RUNNUMBER1=("2857" "2858" "2859" "2860" "2861" "2875" "2877" "2878" "2879" "2880" "2881" "2882")
RUNNUMBER1=("2883" "2884" "2887" "2888" "2889" "2890" "2891" "2892" "2893" "2894" "2896" "2898" "2899")

MIXONLY=1
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
	    echo RUN=${RUN} VER=${VERSION} MIX=${MIX} root AsmFlw_getEvents.C  '>&' $LOG 
	    RUN=${RUN} VER=${VERSION} MIX=${MIX} root -b -q AsmFlw_getEvents.C >& $LOG &
            let I++
	    if [ $I -ge ${#RUNNUMBER1[@]} ]; then
		break;
	    fi
	    
            RUN=${RUNNUMBER1[I]}
	    LOG=asm${RUN}_v${VERSION}_${MIX}.log
	    echo RUN=${RUN} VER=${VERSION} MIX=${MIX} root AsmFlw_getEvents.C  '>&' $LOG 
	    RUN=${RUN} VER=${VERSION} MIX=${MIX} root -b -q AsmFlw_getEvents.C >& $LOG &
            let I++
	    if [ $I -ge ${#RUNNUMBER1[@]} ]; then
		break;
	    fi
	    
            RUN=${RUNNUMBER1[I]}
	    LOG=asm${RUN}_v${VERSION}_${MIX}.log
	    echo RUN=${RUN} VER=${VERSION} MIX=${MIX} root AsmFlw_getEvents.C  '>&' $LOG 
	    RUN=${RUN} VER=${VERSION} MIX=${MIX} root -b -q AsmFlw_getEvents.C >& $LOG 
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
	    echo RUN=${RUN} VER=${VERSION} MIX=${MIX} root AsmFlw_getEvents.C  '>&' $LOG 
	    RUN=${RUN} VER=${VERSION} MIX=${MIX} root -b -q AsmFlw_getEvents.C >& $LOG &
	    let I++
	    if [ $I -ge ${#RUNNUMBER1[@]} ]; then
		break;
	    fi

	    RUN=${RUNNUMBER1[I]}
	    LOG=asm${RUN}_v${VERSION}_${MIX}.log
	    echo RUN=${RUN} VER=${VERSION} MIX=${MIX} root AsmFlw_getEvents.C  '>&' $LOG 
	    RUN=${RUN} VER=${VERSION} MIX=${MIX} root -b -q AsmFlw_getEvents.C >& $LOG &
	    let I++
	    if [ $I -ge ${#RUNNUMBER1[@]} ]; then
		break;
	    fi
	    
	    RUN=${RUNNUMBER1[I]}
	    LOG=asm${RUN}_v${VERSION}_${MIX}.log
	    echo RUN=${RUN} VER=${VERSION} MIX=${MIX} root AsmFlw_getEvents.C  '>&' $LOG 
	    RUN=${RUN} VER=${VERSION} MIX=${MIX} root -b -q AsmFlw_getEvents.C >& $LOG 
	    let I++
	done
    fi

}


v2exe_flatten

