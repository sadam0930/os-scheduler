#include <string>

#define nullptr __null

typedef enum processState {
	CREATED,
	READY,
	RUNNING,
	BLOCKED, 
	DONE
} processState;

std::string stateToString(processState ps){
	std::string str;
	switch(ps){
		case CREATED:
			str = "CREATED";
			break;
		case READY:
			str = "READY";
			break;
		case RUNNING:
			str = "RUNNG";
			break;
		case BLOCKED:
			str = "BLOCK";
			break;
		case DONE:
			str = "Done";
			break;
		default:
			break;
	}
	return str;
}

class Process {
	/*
	A process is characterized by 4 parameters:
	Arrival Time (AT), Total CPU Time (TC), CPU Burst (CB) and IO Burst (IO). 
	*/
	int AT, TC, CB, IO;
	int pid;

	public:
		//Each Process is a node in a linked list
		Process * nextProcess;
		Process * prevProcess;
		
		processState currentState;
		int timeInPreviousState;
		int stateTimeStamp;
		int FT, TT, IT, CW, PRIO; //Finish Time, Turnaround time, IO time, CPU Wait, Static Priority
		int dynamicPrio;
		int remExTime;
		int rCB, rIO; //random CPU/IO Burst
		int remCB; //remaining CPU Burst from peemption
		
		Process(){}
		Process(int pid, int AT, int TC, int CB, int IO, int staticPrio){
			setPid(pid);
			setArrivalTime(AT);
			setTotalCPUTime(TC);
			setCPUBurst(CB);
			setIOBurst(IO);
			this->currentState = CREATED;
			this->nextProcess = nullptr;
			this->prevProcess = nullptr;
			this->PRIO = staticPrio;
			this->dynamicPrio = staticPrio-1;
			this->stateTimeStamp = AT;
			this->FT = 0;
			this->TT = 0;
			this->CW = 0;
			this->IT = 0;
			this->remCB = 0;
			this->rCB = 0;
		}
		void setPid(int pid){ this->pid = pid; }
		void setArrivalTime(int AT){ this->AT = AT; }
		void setTotalCPUTime(int TC){ this->TC = TC; this->remExTime = TC;}
		void setCPUBurst(int CB){ this->CB = CB; }
		void setIOBurst(int IO){ this->IO = IO; }
		int getPid(){ return this->pid; }
		int getArrivalTime(){ return this->AT; }
		int getTotalCPUTime(){ return this->TC; }
		int getCPUBurst(){ return this->CB; }
		int getIOBurst(){ return this->IO; }
};

class ProcessList {
	public:
		Process * head;
		Process * tail;
		int numProcesses;

		ProcessList(){
			head = nullptr;
			tail = head;
			numProcesses = 0;
		}
		~ProcessList(){
			if(numProcesses > 0){
				Process * cur = head;
				Process * next;
				do {
					next = cur->nextProcess;
					delete cur;
					cur = next;
				} while(cur);
			}
		}

		Process * findProcess(int pid){
			Process * foundProcess = nullptr;
			Process * cur = head;

			while(cur){
				if(cur->getPid() == pid){
					foundProcess = cur;
					break;
				}
				cur = cur->nextProcess;
			}

			return foundProcess;
		}

		bool isEmpty(){
			if(numProcesses == 0){
				return true;
			} else {
				return false;
			}
		}

};