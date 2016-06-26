#include "process.h"
#include "events.h"

class Scheduler {		
	public:
		char schedulerType;
		int quantum;
		ProcessList * runQueue;
		ProcessList * expQueue; //only for PRIO
		Scheduler(char schedulerType, int quantum){
			this->runQueue = new ProcessList();
			this->expQueue = new ProcessList();
			this->schedulerType = schedulerType;
			this->quantum = quantum;
		}
		//implementation will differ based on the scheduling algorithm
		virtual void putProcess(Process * process) =0;
		virtual Process * getNextProcess() =0;
		virtual void putExpProcess(Process * process) =0; //only for PRIO
};

/**********************************************************
differentiate how to getProcess between scheduler algorithms
***********************************************************/

//First Come First Serve
class FCFS_Scheduler: public Scheduler {
	public:
		FCFS_Scheduler(char schedulerType, int quantum) : Scheduler(schedulerType, quantum) {}
		
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

		void putExpProcess(Process * process){};
};

//Last Come First Served
class LCFS_Scheduler: public Scheduler {
	public:
		LCFS_Scheduler(char schedulerType, int quantum) : Scheduler(schedulerType, quantum) {}

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

		void putExpProcess(Process * process){};
};

//Shortest Job First
class SJF_Scheduler: public Scheduler {
	public:
		SJF_Scheduler(char schedulerType, int quantum) : Scheduler(schedulerType, quantum) {}

		//sorted insert based on lowest remaining execution time
		void putProcess(Process * process){
			if(runQueue->isEmpty() || runQueue->head->remExTime > process->remExTime){
				process->nextProcess = runQueue->head;
				runQueue->head = process;

				if(runQueue->isEmpty()){
					runQueue->tail = runQueue->head;
				}
			} else {
				Process * cur = runQueue->head;
				while(cur->nextProcess && cur->nextProcess->remExTime <= process->remExTime){
					cur = cur->nextProcess;
				}
				process->nextProcess = cur->nextProcess;
				cur->nextProcess = process;
			}
			(runQueue->numProcesses)++;
		}

		//get process from front of sorted run queue 
		Process * getNextProcess(){
			Process * returnProcess;
			if(runQueue->numProcesses == 0){
				returnProcess = nullptr;
			} else {
				returnProcess = runQueue->head;
				runQueue->head = runQueue->head->nextProcess;
				(runQueue->numProcesses)--;
			}
			return returnProcess;
		}

		void putExpProcess(Process * process){};
};

//Round Robin
class RR_Scheduler: public Scheduler {
	public:
		RR_Scheduler(char schedulerType, int quantum) : Scheduler(schedulerType, quantum) {}

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
				(runQueue->numProcesses)--;
			}
			return returnProcess;
		}

		void putExpProcess(Process * process){};
};

//Priority
class PRIO_Scheduler: public Scheduler {
	public:
		PRIO_Scheduler(char schedulerType, int quantum) : Scheduler(schedulerType, quantum) {
		}

		//inserted sort on dynamic priority
		void putProcess(Process * process){
			if(runQueue->isEmpty() || runQueue->head->dynamicPrio < process->dynamicPrio){
				process->nextProcess = runQueue->head;
				runQueue->head = process;

				if(runQueue->isEmpty()){
					runQueue->tail = runQueue->head;
				}
			} else {
				Process * cur = runQueue->head;
				while(cur->nextProcess && cur->nextProcess->dynamicPrio >= process->dynamicPrio){
					cur = cur->nextProcess;
				}
				process->nextProcess = cur->nextProcess;
				cur->nextProcess = process;
			}
			(runQueue->numProcesses)++;
		}

		//get process from front of sorted run queue
		Process * getNextProcess(){
			Process * returnProcess;
			if(runQueue->numProcesses == 0){
				returnProcess = nullptr;
			} else {
				returnProcess = runQueue->head;
				runQueue->head = runQueue->head->nextProcess;
				(runQueue->numProcesses)--;
			}
			return returnProcess;
		}

		//inserted sort by dynamic priority
		void putExpProcess(Process * process){
			if(expQueue->isEmpty() || expQueue->head->dynamicPrio < process->dynamicPrio){
				process->nextProcess = expQueue->head;
				expQueue->head = process;

				if(expQueue->isEmpty()){
					expQueue->tail = expQueue->head;
				}
			} else {
				Process * cur = expQueue->head;
				while(cur->nextProcess && cur->nextProcess->dynamicPrio >= process->dynamicPrio){
					cur = cur->nextProcess;
				}
				process->nextProcess = cur->nextProcess;
				cur->nextProcess = process;
			}
			(expQueue->numProcesses)++;
		}

};