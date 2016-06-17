#include <string>

class Event {
	int timestamp;
	int pid;
	std::string eventType;

	public:
		Event * nextEvent;
		Event(){};
		Event(int timestamp, int pid, std::string eventType){
			setTimestamp(timestamp);
			setProcessID(pid);
			seteventType(eventType);
		}
		void setTimestamp(int timestamp){this->timestamp = timestamp;}
		void setProcessID(int pid){this->pid = pid;}
		void seteventType(std::string eventType){this->eventType = eventType;}
		int getTimestamp(){return this->timestamp;}
		int getProcessID(){return this->pid;}
		std::string geteventType(){return this->eventType;}
};

class EventList {
	Event * head;
	int numEvents;

	public:
		EventList() {
			head = NULL;
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
		void putEvent(int timestamp, int pid, std::string eventType){
			Event * newEvent = new Event(timestamp, pid, eventType);
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

		Event getEvent(){
			Event curEvent = *head;
			Event * temp = head;
			head = head->nextEvent;
			delete temp;
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