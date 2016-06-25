#include <string>
#include <iostream>

typedef enum transitionState {
	TRANS_TO_READY,
	TRANS_TO_RUN,
	TRANS_TO_BLOCK,
	TRANS_TO_PREEMPT,
	TRANS_TO_DONE
} transitionState;

std::string transitionToString(transitionState ts){
	std::string str;
	switch(ts){
		case TRANS_TO_READY:
			str = "READY";
			break;
		case TRANS_TO_RUN:
			str = "RUNNG";
			break;
		case TRANS_TO_BLOCK:
			str = "BLOCK";
			break;
		case TRANS_TO_PREEMPT:
			str = "READY";
			break;
		case TRANS_TO_DONE:
			str = "Done";
			break;
		default:
			break;
	}
	return str;
}

class Event {
	int timestamp;
	transitionState transition;

	public:
		Process * process;
		Event * nextEvent;
		Event(){};
		Event(int timestamp, Process * process, transitionState transition){
			setTimestamp(timestamp);
			this->process = process;
			setTransition(transition);
		}
		void setTimestamp(int timestamp){this->timestamp = timestamp;}
		// void setProcess(Process * process){this->process = process;}
		void setTransition(transitionState transition){this->transition = transition;}
		int getTimestamp(){return this->timestamp;}
		// Process * getProcess(){return this->process;}
		transitionState getTransition(){return this->transition;}
};

class EventList {
	Event * head;
	int numEvents;

	public:
		EventList() {
			head = nullptr;
			numEvents = 0;
		}
		~EventList(){
			if(numEvents > 0){
				Event * cur = head;
				Event * next;
				do {
					next = cur->nextEvent;
					delete cur;
					cur = next;
				} while(cur);
			}
		}

		//sorted insert
		void putEvent(int timestamp, Process * process, transitionState transition){
			Event * newEvent = new Event(timestamp, process, transition);
			Event * cur;

			if(this->isEmpty() || head->getTimestamp() > newEvent->getTimestamp()){
				newEvent->nextEvent = head;
				head = newEvent;
			} else {
				cur = head;
				while(cur->nextEvent && cur->nextEvent->getTimestamp() <= newEvent->getTimestamp()){
					cur = cur->nextEvent;
				}
				newEvent->nextEvent = cur->nextEvent;
				cur->nextEvent = newEvent;
			}
			numEvents++;
		}

		//memory leak! remember to delete event from scheduler
		Event * getEvent(){
			Event * curEvent;
			if(this->isEmpty()){
				curEvent = nullptr;
			} else {
				curEvent = head;
				head = head->nextEvent;
				numEvents--;
			}
			return curEvent;
		}

		int getNextTimestamp(){
			if(numEvents == 0){
				return -1;
			} else {
				return head->getTimestamp();
			}
		}

		bool isEmpty(){
			if(numEvents == 0){
				return true;
			} else {
				return false;
			}
		}
};