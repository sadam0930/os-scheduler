typedef enum processState {
	CREATED,
	READY,
	RUNNING,
	BLOCKED
} processState;

class Process {
	/*
	A process is characterized by 4 parameters:
	Arrival Time (AT), Total CPU Time (TC), CPU Burst (CB) and IO Burst (IO). 
	*/
	int AT, TC, CB, IO;
	int pid;

	public:
		Process(){}
		Process(int pid, int AT, int TC, int CB, int IO){
			setPid(pid);
			setArrivalTime(AT);
			setTotalCPUTime(TC);
			setCPUBurst(CB);
			setIOBurst(IO);
		}
		void setPid(int pid){ this->pid = pid; }
		void setArrivalTime(int AT){ this->AT = AT; }
		void setTotalCPUTime(int TC){ this->TC = TC; }
		void setCPUBurst(int CB){ this->CB = CB; }
		void setIOBurst(int IO){ this->IO = IO; }
		int getPid(){ return this->pid; }
		int getArrivalTime(){ return this->AT; }
		int getTotalCPUTime(){ return this->TC; }
		int getCPUBurst(){ return this->CB; }
		int getIOBurst(){ return this->IO; }
};