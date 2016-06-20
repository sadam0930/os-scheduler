#include <string>

typedef enum transitionState {
	TRANS_TO_READY,
	TRANS_TO_RUN,
	TRANS_TO_BLOCK,
	TRANS_TO_PREEMPT
} transitionState;

class Event {
	int timestamp;
	int pid;
	transitionState transition;

	public:
		Event * nextEvent;
		Event(){};
		Event(int timestamp, int pid, transitionState transition){
			setTimestamp(timestamp);
			setProcessID(pid);
			setTransition(transition);
		}
		void setTimestamp(int timestamp){this->timestamp = timestamp;}
		void setProcessID(int pid){this->pid = pid;}
		void setTransition(transitionState transition){this->transition = transition;}
		int getTimestamp(){return this->timestamp;}
		int getProcessID(){return this->pid;}
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
		void putEvent(int timestamp, int pid, transitionState transition){
			Event * newEvent = new Event(timestamp, pid, transition);
			Event * cur;

			if(this->isEmpty() || head->getTimestamp() > newEvent->getTimestamp()){
				newEvent->nextEvent = head;
				head = newEvent;
			} else {
				cur = head;
				while(cur->nextEvent && cur->nextEvent->getTimestamp() < newEvent->getTimestamp()){
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
			return head->getTimestamp();
		}

		bool isEmpty(){
			if(numEvents == 0){
				return true;
			} else {
				return false;
			}
		}
};