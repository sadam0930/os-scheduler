#include "process.h"
#include "events.h"

class Scheduler {
	protected:
		ProcessList * runQueue;
	public:
		Scheduler(){
			runQueue = new ProcessList;
		}
		//implementation will differ based on the scheduling algorithm
		virtual void putProcess(int pid, int AT, int TC, int CB, int IO);
		virtual void putProcess(Process * process);
		virtual Process * getNextProcess();
};

//differentiate how to getProcess between scheduler algorithms
class FCFS_Scheduler: public Scheduler {
	public:
		FCFS_Scheduler(): Scheduler() {

		}
		
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
			runQueue->numProcesses++;
		}

		void putProcess(int pid, int AT, int TC, int CB, int IO){
			Process * newProcess = new Process(pid, AT, TC, CB, IO);

			if(runQueue->isEmpty()){
				runQueue->head = newProcess;
				runQueue->tail = runQueue->head;
			} else {
				newProcess->prevProcess = runQueue->tail;
				runQueue->tail->nextProcess = newProcess;
				runQueue->tail = newProcess;
			}
			runQueue->numProcesses++;
		}

		//get process from front of run queue
		Process * getNextProcess(){
			Process * returnProcess;
			returnProcess = runQueue->head;
			runQueue->head = runQueue->head->nextProcess;
			runQueue->head->prevProcess = nullptr;
			runQueue->numProcesses--;
			return returnProcess;
		}
};