#include <iostream>
#include <ifstream>
#include <string>

using namespace std;


class Scheduler {
	//implementation will differ based on the scheduling algorithm
	virtual void putProcess(Process *);
	virtual Process * getProcess();
};

class Event {
	public:
		char * timestamp;
		Process * process;
		char * eventName;
		Event * nextEvent;
};

class EventList {
	public:
		Event * head;
		Event * tail;
		
		EventList() {
			head = NULL;
			tail = NULL;
		};

		void createEvent(char * timestamp, Process *, char * eventName){
			//??case for list is empty - head == NULL
			//??put at tail? - they should be in order
			//??make sure char * == string
		};

		Process * getNextEvent(){
			//??next relative to what?
		};

		char * getNextTimestamp(){

		};
};

class Process {
	public:
		processState state;

};

typedef enum processState {
	CREATED,
	READY,
	RUNNING,
	BLOCKED
} processState;

//begin scheduler simulation
int main(){
	//??read -s flag
	//??initialize scheduler based on type from -s

	//get filename
	string filename = "";
	cout << "Enter filename: ";
	getline(cin, filename);

	//read file
	ifstream f;
	f.open(filename);
	if(f.is_open()){

	} else {
		cout << "Could not open file: " << filename << "\n";
	}

	f.close();

	return 0;
}