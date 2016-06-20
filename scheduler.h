class Scheduler {
	public:
		//implementation will differ based on the scheduling algorithm
		virtual void putProcess(Process *);
		virtual Process * getProcess();
};

class FCFS_Scheduler: public Scheduler {
	public:
		FCFS_Scheduler(){

		}
		
		void putProcess(Process *){

		}

		Process * getProcess(){

		}
};