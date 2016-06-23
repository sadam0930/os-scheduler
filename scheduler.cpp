// #include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <unistd.h>

#include "scheduler.h"

using namespace std;

// global
vector<int> randvals; 
int ofs = 0;
int myrandom(int burst) { 
	int ranNum = 1 + (randvals[ofs] % burst);
	ofs++;
	if(ofs == randvals.size()-1){
		ofs = 0;
	}
	return ranNum; 
}

void initialize(string filename, string randfile, EventList * events, Scheduler * scheduler){
	ifstream f;
	//read randfile
	f.open(randfile);
	if(f.is_open()){
		string line;
		//First line is the count of random numbers in the file
		getline(f, line); //throw it away
		//Each line in the file is a random number
		while(getline(f, line)){
			// cout << line << endl;
			randvals.push_back(stoi(line, nullptr));
		}
	} else {
		cout << "Could not open file: " << randfile << endl;
		exit(1);
	}
	f.close();

	//read input file
	f.open(filename);
	if(f.is_open()){
		// Each line in the file is a new process
		string line;
		int pid = 0;  
		while(getline(f, line)){
			stringstream lineStream(line);
			string token;
			vector<int> processArgs;
			while(lineStream >> token){
				processArgs.push_back(stoi(token, nullptr));
			}
			Process * newProcess = new Process(pid, processArgs[0], processArgs[1], processArgs[2], processArgs[3], myrandom(4));
			events->putEvent(processArgs[0], newProcess, TRANS_TO_READY);
			// scheduler->putProcess(newProcess);
			pid++;
		}
	} else {
		cout << "Could not open file: " << filename << endl;
		exit(1);
	}
	f.close();
}

void printVerbose(bool enabled, Event * event){
	if(enabled){
		cout << event->getTimestamp() << " ";
		cout << event->process->getPid() << " ";
		cout << event->process->timeInPreviousState << ": ";
		
		if(event->getTransition() != TRANS_TO_DONE){
			cout << stateToString(event->process->currentState) << " -> ";
		}
		cout << transitionToString(event->getTransition());
		if(event->getTransition() == TRANS_TO_RUN){
			cout << "  cb=" << event->process->rCB << " ";
			cout << "rem=" << event->process->remExTime << " ";
			cout << "prio=" << event->process->dynamicPrio;
		}
		if(event->getTransition() == TRANS_TO_BLOCK){
			cout << "  ib=" << event->process->rIO << " ";
			cout << "rem=" << event->process->remExTime << " ";
		}
		cout << endl;
	}
}

void start_simulation(EventList * events, Scheduler * scheduler, bool refoutv){
	int curTime = 0;
	Event * curEvent;
	bool callScheduler = false;
	Process * curRunningProcess = nullptr;
	
	while((curEvent = events->getEvent())){
		curTime = curEvent->getTimestamp();
		curEvent->process->timeInPreviousState = curTime - curEvent->process->stateTimeStamp;

		switch(curEvent->getTransition()){
			case TRANS_TO_READY:
				printVerbose(refoutv, curEvent);
				//tally IO time before changing the state
				if(curEvent->process->currentState == BLOCKED){
					(curEvent->process->IT) += curEvent->process->timeInPreviousState;
				}
				curEvent->process->currentState = READY;
				curEvent->process->stateTimeStamp = curTime;
				scheduler->putProcess(curEvent->process);
				callScheduler = true;

				break;
			case TRANS_TO_RUN:
				curEvent->process->rCB = myrandom(curEvent->process->getCPUBurst());
				if(curEvent->process->remExTime < curEvent->process->rCB){
					//next CPU cycle is last cycle
					curEvent->process->rCB = curEvent->process->remExTime;
					events->putEvent((curTime + curEvent->process->rCB), curEvent->process, TRANS_TO_DONE);
				} else {
					events->putEvent((curTime + curEvent->process->rCB), curEvent->process, TRANS_TO_BLOCK);
				}

				printVerbose(refoutv, curEvent);
				curEvent->process->currentState = RUNNING;
				curEvent->process->stateTimeStamp = curTime;

				break;
			case TRANS_TO_BLOCK:
				curEvent->process->rIO = myrandom(curEvent->process->getIOBurst());
				(curEvent->process->remExTime) -= curEvent->process->rCB;
				printVerbose(refoutv, curEvent);
				curEvent->process->currentState = BLOCKED;
				curEvent->process->stateTimeStamp = curTime;
				events->putEvent((curTime + curEvent->process->rIO), curEvent->process, TRANS_TO_READY);
				//free current running process
				curRunningProcess = nullptr;
				callScheduler = true;

				break;
			case TRANS_TO_PREEMPT:
				printVerbose(refoutv, curEvent);
				curEvent->process->currentState = READY;
				curEvent->process->stateTimeStamp = curTime;

				break; 
			case TRANS_TO_DONE:
				printVerbose(refoutv, curEvent);
				curEvent->process->FT = curTime;
				//free current running process
				curRunningProcess = nullptr;
				callScheduler = true;
				break;
		}

		delete curEvent; //free memory

		if(callScheduler){
			//process all events from the same timestamp first
			if(events->getNextTimestamp() == curTime){
				continue;
			} else {
				callScheduler = false;
				if(curRunningProcess == nullptr){
					curRunningProcess = scheduler->getNextProcess();
					if(curRunningProcess){
						events->putEvent(curTime, curRunningProcess, TRANS_TO_RUN);
					}
				}
			}
		}
	}
}

//begin scheduler simulation
int main(int argc, char **argv){
	int opt, quant;
	bool refoutv;
	Scheduler * scheduler;
	while ((opt = getopt (argc, argv, "vs:")) != -1) {
		//instantiate a scheduler based on the -s option
        if (opt == 's') {
            if (optarg[0] == 'R' || optarg[0] == 'P') {
                sscanf(optarg + 1, "%d", &quant);
            } else {quant = 9999;}
            if (optarg[0] == 'F'){
            	scheduler = new FCFS_Scheduler();
            } else if (optarg[0] == 'L'){

            } else if (optarg[0] == 'P'){

            } else if (optarg[0] == 'R'){

            } else if (optarg[0] == 'S'){

            } else {
            	cout << "Invalid argument " << optarg[0] << endl;
            	exit(1);
            }
        }
        if (opt == 'v'){refoutv = true;}
    }

    string filename = argv[optind];
	string randfile = argv[optind+1];

	EventList * events = new EventList();

	initialize(filename, randfile, events, scheduler);
	start_simulation(events, scheduler, refoutv);

	return 0;
}