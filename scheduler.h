#include "process.h"
#include "events.h"

class Scheduler {		
	public:
		char schedulerType;
		ProcessList * runQueue;
		Scheduler(char schedulerType){
			this->runQueue = new ProcessList();
			this->schedulerType = schedulerType;
		}
		//implementation will differ based on the scheduling algorithm
		virtual void putProcess(Process * process) =0;
		virtual Process * getNextProcess() =0;
};

/**********************************************************
differentiate how to getProcess between scheduler algorithms
***********************************************************/
//First Come First Serve
class FCFS_Scheduler: public Scheduler {
	public:
		FCFS_Scheduler(char schedulerType) : Scheduler(schedulerType) {}
		
		//add Process to end of run queue
		void putProcess(Process * process){
			if(runQueue->isEmpty()){
				runQueue->head = process;
				runQueue->tail = runQueue->head;
			} else {
				process->prevProcess = runQueue->tail;
				runQueue->tail->nextProcess = process;
				runQueue->tail = process;
			}
			(runQueue->numProcesses)++;
		}

		//get process from front of run queue
		Process * getNextProcess(){
			Process * returnProcess;
			if(runQueue->numProcesses == 0){
				returnProcess = nullptr;
			} else {
				returnProcess = runQueue->head;
				runQueue->head = runQueue->head->nextProcess;
				// runQueue->head->prevProcess = nullptr;
				(runQueue->numProcesses)--;
			}
			return returnProcess;
		}
};

//Last Come First Served
class LCFS_Scheduler: public Scheduler {
	public:
		LCFS_Scheduler(char schedulerType) : Scheduler(schedulerType) {}

		//add Process to end of run queue
		void putProcess(Process * process){
			if(runQueue->isEmpty()){
				runQueue->head = process;
				runQueue->tail = runQueue->head;
			} else {
				process->prevProcess = runQueue->tail;
				runQueue->tail->nextProcess = process;
				runQueue->tail = process;
			}
			(runQueue->numProcesses)++;
		}

		//get process from end of run queue
		Process * getNextProcess(){
			Process * returnProcess;
			if(runQueue->numProcesses == 0){
				returnProcess = nullptr;
			} else {
				returnProcess = runQueue->tail;
				runQueue->tail = runQueue->tail->prevProcess;
				(runQueue->numProcesses)--;
			}
			return returnProcess;
		}
};

//Shortest Job First
class SJF_Scheduler: public Scheduler {
	public:
		SJF_Scheduler(char schedulerType) : Scheduler(schedulerType) {}

		void putProcess(Process * process){

		}

		//get process based on lowest remaining execution time
		Process * getNextProcess(){
			Process * returnProcess;
			if(runQueue->numProcesses == 0){
				returnProcess = nullptr;
			} else {
				
			}
			return returnProcess;
		}
};