{
  TCut sn108 = "bmpid == 10";
  TCut sn132 = "bmpid == 20";
  TCut sn    = sn108||sn132;
  //  TCut sn    ;
  TCut bv2   = "bevt[2]";
  TCut bv4   = "bevt[4]";
  TCut ntrk  = "ntrack[1]>1";
  TCut gtrack= "gTrack&&gTgTrack";
  TCut bestcut  = sn&&bv2&&bv4&&gtrack;

  TCut InKyotoL = "fKyotoLy<=-51&&fKyotoLy>=-501 && fKyotoLz>=168&&fKyotoLz<=1685";
  TCut InKyotoR = "fKyotoRy<=-51&&fKyotoRy>=-501 && fKyotoRz>=168&&fKyotoRz<=1685";

  TCut FindKyotoL = "dzKyotoL<40&&dzKyotoL>-40";
  TCut FindKyotoR = "dzKyotoR<40&&dzKyotoR>-40";

  TCut InKatana = "fKatanax>=-800 && fKatanax <=500 && fKatanay <=0 && fKatanay >= -400";

}

