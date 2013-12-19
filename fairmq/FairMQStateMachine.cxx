/*
 * FairMQStateMachine.cxx
 *
 *  Created on: Oct 25, 2012
 *      Author: dklein
 */

#include <iostream>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "FairMQStateMachine.h"
#include "FairMQLogger.h"


FairMQStateMachine::FairMQStateMachine() :
  fState(IDLE)
{
}

void FairMQStateMachine::ChangeState(int event)
{
  switch(fState) {

  case IDLE:
    switch(event) {

    case INIT:
      FairMQLogger::GetInstance()->Log(FairMQLogger::STATE, "IDLE --init--> INITIALIZING");
      fState = INITIALIZING;
      Init();
      return;

    case END:
      FairMQLogger::GetInstance()->Log(FairMQLogger::STATE, "IDLE --end--> (o)");
      return;

    default:
      return;
    }
    break;


  case INITIALIZING:
    switch(event) {

    case SETOUTPUT:
      FairMQLogger::GetInstance()->Log(FairMQLogger::STATE, "INITIALIZING --bind--> SETTINGOUTPUT");
      fState = SETTINGOUTPUT;
      InitOutput();
      return;

    default:
      return;
    }
    break;


  case SETTINGOUTPUT:
    switch(event) {

    case SETINPUT:
      FairMQLogger::GetInstance()->Log(FairMQLogger::STATE, "SETTINGOUTPUT --connect--> SETTINGINPUT");
      fState = SETTINGINPUT;
      InitInput();
      return;

    default:
      return;
    }
    break;


  case SETTINGINPUT:
    switch(event) {

    case PAUSE:
      FairMQLogger::GetInstance()->Log(FairMQLogger::STATE, "SETTINGINPUT --pause--> WAITING");
      fState = WAITING;
      Pause();
      return;

    case RUN:
      FairMQLogger::GetInstance()->Log(FairMQLogger::STATE, "SETTINGINPUT --run--> RUNNING");
      fState = RUNNING;
      running_state = boost::thread(boost::bind(&FairMQStateMachine::Run, this));
      return;

    default:
      return;
    }
    break;


  case WAITING:
    switch(event) {

    case RUN:
      FairMQLogger::GetInstance()->Log(FairMQLogger::STATE, "WAITING --run--> RUNNING");
      fState = RUNNING;
      running_state = boost::thread(boost::bind(&FairMQStateMachine::Run, this));
      return;

    case STOP:
      FairMQLogger::GetInstance()->Log(FairMQLogger::STATE, "WAITING --stop--> IDLE");
      fState = IDLE;
      Shutdown();
      return;

    default:
      return;
    }
    break;


  case RUNNING:
    switch(event) {

    case PAUSE:
      FairMQLogger::GetInstance()->Log(FairMQLogger::STATE, "RUNNING --pause--> WAITING");
      fState = WAITING;
      running_state.join();
      return;

    case STOP:
      FairMQLogger::GetInstance()->Log(FairMQLogger::STATE, "RUNNING --stop--> IDLE");
      fState = IDLE;
      running_state.join();
      Shutdown();
      return;

    default:
      return;
    }
    break;


  default:
    break;

  }//switch fState
}

void FairMQStateMachine::Init()
{
}

void FairMQStateMachine::Run()
{
}

void FairMQStateMachine::Pause()
{
}

void FairMQStateMachine::Shutdown()
{
}

void FairMQStateMachine::InitOutput()
{
}

void FairMQStateMachine::InitInput()
{
}

FairMQStateMachine::~FairMQStateMachine()
{
}

