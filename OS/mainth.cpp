/*
 * mainth.cpp
 *
 *  Created on: May 23, 2019
 *      Author: OS1
 */





#include "schedule.h"
#include "MainTh.h";
#include "Timer.h"
#include "IOSTREAM.H"
extern int userMain(int, char*[]);

MainThread::MainThread(int argc_,char *argv_[]):Thread(){
	argc = argc_;

	argv = argv_;

}



MainThread::~MainThread(){

	waitToComplete();

}



void MainThread::run(){
	userMain(argc, argv);
	Scheduler::put(Timer::mainPCB);
}

