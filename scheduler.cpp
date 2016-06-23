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

void initialize(string filename, string randfile, EventList * events, Scheduler * scheduler, vector<Process *> * allProcesses){
	ifstream f;
	//read randfile
	f.open(randfile);
	if(f.is_open()){
		string line;
		//First line is the count of random numbers in the file
		getline(f, line); //throw it away
		//Each line in the file is a random number
		while(getline(f, line)){
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
			allProcesses->push_back(newProcess);
			events->putEvent(processArgs[0], newProcess, TRANS_TO_READY);
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

int start_simulation(EventList * events, Scheduler * scheduler, bool refoutv){
	int curTime = 0;
	int lastEventFinish;
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
				if(curEvent->process->remExTime <= curEvent->process->rCB){
					//next CPU cycle is last cycle for this process
					curEvent->process->rCB = curEvent->process->remExTime;
					events->putEvent((curTime + curEvent->process->rCB), curEvent->process, TRANS_TO_DONE);
				} else {
					events->putEvent((curTime + curEvent->process->rCB), curEvent->process, TRANS_TO_BLOCK);
				}

				printVerbose(refoutv, curEvent);
				//tally CPU wait time, time spent in ready
				(curEvent->process->CW) += curEvent->process->timeInPreviousState;
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
				(curEvent->process->TT) = curEvent->process->FT - curEvent->process->getArrivalTime();
				//free current running process
				curRunningProcess = nullptr;
				callScheduler = true;
				lastEventFinish = curTime; //overwrite until it's over
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
	return lastEventFinish;
}

void print_proc_data(vector<Process *> * allProcesses, Scheduler * scheduler){
	switch(scheduler->schedulerType){
		case 'F': cout<<"FCFS"<<endl; break;
        case 'L': cout<<"LCFS"<<endl; break;
        case 'S': cout<<"SJF"<<endl; break;
        case 'R':
            // cout<<"RR "<<scheduler->getQuantum()<<endl;
            break;
        case 'P':
            // cout<<"PRIO "<<scheduler->getQuantum()<<endl;
            break;
        default: break;
	}

	unsigned long size = allProcesses->size();
	for (int i=0; i<size; i++) {
		printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
               allProcesses->at(i)->getPid(),
               allProcesses->at(i)->getArrivalTime(),
               allProcesses->at(i)->getTotalCPUTime(),
               allProcesses->at(i)->getCPUBurst(),
               allProcesses->at(i)->getIOBurst(),
               allProcesses->at(i)->PRIO,
               allProcesses->at(i)->FT,
               allProcesses->at(i)->TT,
               allProcesses->at(i)->IT,
               allProcesses->at(i)->CW
               );
	}
}

void print_sum(int lastEventFinish, double cpuUtil, double ioUtil, double avgTurn, double avgCW, double throughput){
	printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",
           lastEventFinish, cpuUtil, ioUtil, avgTurn, avgCW, throughput);
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
            	scheduler = new FCFS_Scheduler(optarg[0]);
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
	vector<Process *> * allProcesses = new vector<Process *>();

	initialize(filename, randfile, events, scheduler, allProcesses);
	int totalTime = start_simulation(events, scheduler, refoutv);
	print_proc_data(allProcesses, scheduler);

	//computer some overall statistics
	double cpuUtil, ioUtil, avgTurn, avgCW, throughput;
	cpuUtil = ioUtil = avgTurn = avgCW = throughput = 0;
	
	unsigned long numProcesses = allProcesses->size();
	for (int i=0; i < numProcesses; i++) {
        cpuUtil += allProcesses->at(i)->getTotalCPUTime();
        ioUtil += allProcesses->at(i)->IT;
        avgTurn += allProcesses->at(i)->TT;
        avgCW += allProcesses->at(i)->CW;
    }
    cpuUtil = ((double)cpuUtil*100)/(double)totalTime;
    ioUtil = ((double)ioUtil*100)/(double)totalTime; //wrong
    avgTurn = (double)avgTurn/(double)numProcesses;
    avgCW = (double)avgCW/(double)numProcesses;
    throughput = ((double)numProcesses*100)/(double)totalTime;

	print_sum(totalTime, cpuUtil, ioUtil, avgTurn, avgCW, throughput);

	return 0;
}