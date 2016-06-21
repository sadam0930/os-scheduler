typedef enum processState {
	CREATED,
	READY,
	RUNNING,
	BLOCKED, 
	DONE
} processState;

class Process {
	/*
	A process is characterized by 4 parameters:
	Arrival Time (AT), Total CPU Time (TC), CPU Burst (CB) and IO Burst (IO). 
	*/
	int AT, TC, CB, IO;
	int pid;
	processState currentState;

	public:
		Process * nextProcess;
		Process * prevProcess;
		Process(){}
		Process(int pid, int AT, int TC, int CB, int IO){
			setPid(pid);
			setArrivalTime(AT);
			setTotalCPUTime(TC);
			setCPUBurst(CB);
			setIOBurst(IO);
			currentState = CREATED;
			nextProcess = nullptr;
			prevProcess = nullptr;
		}
		void setPid(int pid){ this->pid = pid; }
		void setArrivalTime(int AT){ this->AT = AT; }
		void setTotalCPUTime(int TC){ this->TC = TC; }
		void setCPUBurst(int CB){ this->CB = CB; }
		void setIOBurst(int IO){ this->IO = IO; }
		void setCurrentState(processState state){ this->currentState = state; }
		int getPid(){ return this->pid; }
		int getArrivalTime(){ return this->AT; }
		int getTotalCPUTime(){ return this->TC; }
		int getCPUBurst(){ return this->CB; }
		int getIOBurst(){ return this->IO; }
		processState getCurrentState(){ return this->currentState; }
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

		//for initialization of full process list
		/*void addProcess(int pid, int AT, int TC, int CB, int IO){
			Process * newProcess = new Process(pid, AT, TC, CB, IO);

			if(this->isEmpty()){
				this->head = newProcess;
				this->tail = this->head;
			} else {
				newProcess->prevProcess = this->tail;
				this->tail->nextProcess = newProcess;
				this->tail = newProcess;
			}
			numProcesses++;
		}*/

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