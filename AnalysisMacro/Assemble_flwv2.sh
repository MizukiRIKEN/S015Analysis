#! /bin/bash
VERSION=0

function v2exe1(){
#RUN=2900 VER=$VERSION root -b -q Assemble_flwv2.C
    RUN=2901 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2905 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2907 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2913 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
}

function v2exe2(){
    RUN=2914 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2916 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2917 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2918 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2919 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
}

function v2exe3(){
    RUN=2920 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2921 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2922 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2924 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2925 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
}

function v2exe4(){
    RUN=2926 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2927 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2928 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2929 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2930 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
}

function v2exe5(){
    RUN=2931 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2932 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2933 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2934 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2935 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
}

function v2exe6(){
    RUN=2936 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2937 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2938 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2939 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2940 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
    RUN=2941 VER=$VERSION root -b -q Assemble_flwv2.C > asmv2{$RUN}.log
}


v2exe1 &
v2exe2 &
v2exe3 &
v2exe4 &
v2exe5 &
v2exe6 &
