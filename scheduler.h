#include "process.h"
#include "events.h"

class Scheduler {		
	public:
		ProcessList * runQueue;
		Scheduler(){
			this->runQueue = new ProcessList();
		}
		//implementation will differ based on the scheduling algorithm
		virtual void putProcess(Process * process) =0;
		virtual Process * getNextProcess() =0;
};

//differentiate how to getProcess between scheduler algorithms
class FCFS_Scheduler: public Scheduler {
	public:
		// FCFS_Scheduler() : Scheduler() {}
		
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
				// std::cout<<"no process"<<std::endl;
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