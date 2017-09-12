#! /bin/bash                                                                                                       

VERSION=4.0.1

RUNNUMBER1=("2841")
RUNNUMBER1=("2843" "2844" "2845" "2846" "2848" "2849" "2850" "2851" "2852" "2855" "2856" \
"2857" "2858" "2859" "2860" "2861" "2875" "2877" "2878" "2879" "2880" "2881" "2882" \
"2883" "2884" "2887" "2888" )
RUNNUMBER1=(\
"2889" "2890" "2891" "2892" "2893" "2894" "2896" "2898" "2899" \
"2900" "2901" "2902" "2903" "2904" "2905" "2907" "2914" "2916" "2917" "2919" "2920" "2921" "2922" \
"2924" "2925" "2926" "2927" "2929" "2930" "2931" "2932" "2933" \
"2934" "2935" "2936" "2939" "2940" "2941" "2942" "2943" "2944" "2945" "2946" "2948" \
"2955" "2956" "2958" "2959" "2960" "2961" "2962" "2964" "2965" "2966" "2968" "2969" \
"2970" "2971" "2972" "2973" "2975" "2976" "2977" "2978" "2979" "2980" "2981" "2982" \
"2983" "2984" "2985" "2986" "2988" "2989" "2990" "2991" "2992" "2993" "2997" "2999" \
"3000" "3002" "3003" "3007" "3039" "2975" "2976" "2977" "2978" "2979" "2980" "2981" "2982" \
"2983" "2984" "2985")


MIXONLY=2
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
	    echo RUN=${RUN} VER=${VERSION} MIX=${MIX} root AsmFlw_getEvents_v2.C  '>&' $LOG 
	    RUN=${RUN} VER=${VERSION} MIX=${MIX} root -b -q AsmFlw_getEvents_v2.C >& $LOG &
            let I++
	    if [ $I -ge ${#RUNNUMBER1[@]} ]; then
		break;
	    fi
	    
            RUN=${RUNNUMBER1[I]}
	    LOG=asm${RUN}_v${VERSION}_${MIX}.log
	    echo RUN=${RUN} VER=${VERSION} MIX=${MIX} root AsmFlw_getEvents_v2.C  '>&' $LOG 
	    RUN=${RUN} VER=${VERSION} MIX=${MIX} root -b -q AsmFlw_getEvents_v2.C >& $LOG &
            let I++
	    if [ $I -ge ${#RUNNUMBER1[@]} ]; then
		break;
	    fi
	    
            RUN=${RUNNUMBER1[I]}
	    LOG=asm${RUN}_v${VERSION}_${MIX}.log
	    echo RUN=${RUN} VER=${VERSION} MIX=${MIX} root AsmFlw_getEvents_v2.C  '>&' $LOG 
	    RUN=${RUN} VER=${VERSION} MIX=${MIX} root -b -q AsmFlw_getEvents_v2.C >& $LOG 
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
	    echo RUN=${RUN} VER=${VERSION} MIX=${MIX} root AsmFlw_getEvents_v2.C  '>&' $LOG 
	    RUN=${RUN} VER=${VERSION} MIX=${MIX} root -b -q AsmFlw_getEvents_v2.C >& $LOG &
	    let I++
	    if [ $I -ge ${#RUNNUMBER1[@]} ]; then
		break;
	    fi

	    RUN=${RUNNUMBER1[I]}
	    LOG=asm${RUN}_v${VERSION}_${MIX}.log
	    echo RUN=${RUN} VER=${VERSION} MIX=${MIX} root AsmFlw_getEvents_v2.C  '>&' $LOG 
	    RUN=${RUN} VER=${VERSION} MIX=${MIX} root -b -q AsmFlw_getEvents_v2.C >& $LOG &
	    let I++
	    if [ $I -ge ${#RUNNUMBER1[@]} ]; then
		break;
	    fi
	    
	    RUN=${RUNNUMBER1[I]}
	    LOG=asm${RUN}_v${VERSION}_${MIX}.log
	    echo RUN=${RUN} VER=${VERSION} MIX=${MIX} root AsmFlw_getEvents_v2.C  '>&' $LOG 
	    RUN=${RUN} VER=${VERSION} MIX=${MIX} root -b -q AsmFlw_getEvents_v2.C >& $LOG 
	    let I++
	done
    fi

}


v2exe_flatten

