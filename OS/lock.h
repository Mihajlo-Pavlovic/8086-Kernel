/*
 * lock.h
 *
 *  Created on: Jun 30, 2019
 *      Author: OS1
 */

#ifndef LOCK_H_
#define LOCK_H_

extern unsigned volatile lock;
void dispatch();

#define LOCK      ++lock
#define UNLOCK    /*if (--lock == 0 && Timer::lockTimedOut) {  dispatch(); }*/ --lock;
#define LOCKED(s) LOCK; s UNLOCK

#define HARD_LOCK      asm { pushf; cli; }
#define HARD_UNLOCK    asm popf
#define HARD_LOCKED(s) HARD_LOCK s HARD_UNLOCK;



#endif /* LOCK_H_ */
