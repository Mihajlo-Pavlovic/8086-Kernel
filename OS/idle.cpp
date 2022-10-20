/*
 * idle.cpp
 *
 *  Created on: May 19, 2019
 *      Author: OS1
 */

#include "idle.h"
#include "def.h"
#include "pcb.h"
#include "lock.h"
#include "timer.h"
#include "kernsem.h"
#include <iostream.h>

Idle::Idle() : Thread(defaultStackSize, 1) { }
void Idle::run() {

		while (1) {;}

	};
