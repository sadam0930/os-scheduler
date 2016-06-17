#include <string>

class Event {
	public:
		int timestamp;
		Process * process;
		std::string eventName;
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

		void putEvent(char * timestamp, Process *, char * eventName){
			//??case for list is empty - head == NULL
			//??put at tail? - they should be in order
			//??make sure char * == string
		};

		// Process * getEvent(){
		// 	//??next relative to what?
		// };

		// char * getNextTimestamp(){

		// };
};