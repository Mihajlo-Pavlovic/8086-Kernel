/*
 * def.h
 *
 *  Created on: Jun 30, 2019
 *      Author: OS1
 */

#ifndef DEF_H_
#define DEF_H_

typedef unsigned int  Word;
typedef unsigned int Register;

typedef unsigned long StackSize;
typedef unsigned int  Time; // 55 ms multiples
typedef int ID;

typedef void (*WrapperFunc)();
typedef void interrupt (*InterruptRoutine)(...);

#endif /* DEF_H_ */
