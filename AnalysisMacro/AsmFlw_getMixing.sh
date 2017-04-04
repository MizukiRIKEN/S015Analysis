#!/bin/bash


function exe1(){
#RUN=2900 VER=6.3 MIX=0 root -b -q AsmFlw_getMixing.C > asm2900v6r.log 
#RUN=2900 VER=6.3 MIX=1 root -b -q AsmFlw_getMixing.C > asm2900v6m.log 

RUN=2901 VER=6.3 MIX=0 root -b -q AsmFlw_getMixing.C > asm2901v6r.log 
RUN=2901 VER=6.3 MIX=1 root -b -q AsmFlw_getMixing.C > asm2901v6m.log 

RUN=2913 VER=6.3 MIX=0 root -b -q AsmFlw_getMixing.C > asm2913v6r.log 
RUN=2913 VER=6.3 MIX=1 root -b -q AsmFlw_getMixing.C > asm2913v6m.log 
}

function exe2(){
RUN=2905 VER=6.3 MIX=0 root -b -q AsmFlw_getMixing.C > asm2905v6r.log 
RUN=2905 VER=6.3 MIX=1 root -b -q AsmFlw_getMixing.C > asm2905v6m.log 

RUN=2907 VER=6.3 MIX=0 root -b -q AsmFlw_getMixing.C > asm2907v6r.log 
RUN=2907 VER=6.3 MIX=1 root -b -q AsmFlw_getMixing.C > asm2907v6m.log 

RUN=2914 VER=6.3 MIX=0 root -b -q AsmFlw_getMixing.C > asm2914v6r.log 
RUN=2914 VER=6.3 MIX=1 root -b -q AsmFlw_getMixing.C > asm2914v6m.log 
}

function exe3(){
RUN=2916 VER=6.3 MIX=0 root -b -q AsmFlw_getMixing.C > asm2916v6r.log 
RUN=2916 VER=6.3 MIX=1 root -b -q AsmFlw_getMixing.C > asm2916v6m.log 

RUN=2917 VER=6.3 MIX=0 root -b -q AsmFlw_getMixing.C > asm2917v6r.log 
RUN=2917 VER=6.3 MIX=1 root -b -q AsmFlw_getMixing.C > asm2917v6m.log 

RUN=2918 VER=6.3 MIX=0 root -b -q AsmFlw_getMixing.C > asm2918v6r.log 
RUN=2918 VER=6.3 MIX=1 root -b -q AsmFlw_getMixing.C > asm2918v6m.log 
}
