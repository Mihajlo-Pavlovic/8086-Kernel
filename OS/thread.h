/*
 * thread.h
 *
 *  Created on: Jun 30, 2019
 *      Author: OS1
 */

#ifndef THREAD_H_
#define THREAD_H_


typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;


typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms

typedef unsigned SignalID;

typedef void (*SignalHandler)();


typedef int ID;

class PCB; // Kernel's implementation of a user's thread

class Thread {
public:
	void start();
	void waitToComplete();
	virtual ~Thread();

	ID getId();
	static ID getRunningId();
	static Thread* getThreadById(ID id);

	int signalBlocked(SignalID signal){
		return (blockedSignalsGlobaly[signal] || blockedSignalsLocaly[signal]);
	}
	void processSignal(SignalID signal);
	int takeNext(){ return signalList.takeNext(); }

public:
	friend class PCB;
	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {}

	void signal(SignalID signal);

	void registerHandler(SignalID signal, SignalHandler handler);
	void unregisterAllHandlers(SignalID id);

	void blockSignal(SignalID signla);
	static void blockSignalGlobally(SignalID signal);

	void unblockSignal(SignalID signal);
	static void unblockSignalGlobally(SignalID signal);

	void swap(SignalID  id,  SignalHandler hand1,  SignalHandler  hand2);



public:
	PCB* myPCB;

public:
	class SignalList{
	public:
		struct SignalListElem{
			SignalID signalID;
			SignalListElem* next;
			SignalListElem(SignalID signalID) : signalID(signalID), next(0) {}
		};

		SignalListElem *head, *tail;

		SignalList() : head(0), tail(0) {}
		~SignalList();

		void push(SignalID signalID);
		int takeNext();
		void remove(SignalListElem* elem);



	};

	class SignalHandlerList{
	public:
		struct SignalHandlerListElem{
			SignalHandler handler;
			SignalHandlerListElem* next;
			SignalHandlerListElem(SignalHandler handler,SignalHandlerListElem* next = 0) : handler(handler), next(next){}
		};
			SignalHandlerList() : head(0), tail(0) {}
			~SignalHandlerList();

			SignalHandlerListElem *head, *tail;

			void push(SignalHandler handler);

			void removeAll();



	};
public:



	int blockedSignalsLocaly[16];
	static int blockedSignalsGlobaly[16];

	SignalList signalList;

	void inicSignals();

	void static signal0();


};

void dispatch ();




#endif /* THREAD_H_ */
