#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <unistd.h>

#include "process.h"
#include "events.h"
#include "scheduler.h"

using namespace std;

void initialize(string filename, string randfile){
	//read file
	ifstream f;
	f.open(filename);
	if(f.is_open()){
		// Each line in the file is a new process
		string line; 
		while(getline(f, line)){
			stringstream lineStream(line);
			string token;
			vector<int> processArgs;
			int index = 0; //to compare two processes with the same arrival time
			while(lineStream >> token){
				processArgs.push_back(stoi(token, nullptr));
			}
			Process * newProcess = new Process(index, processArgs[0], processArgs[1], processArgs[2], processArgs[3]);
			index++;
		}
	} else {
		cout << "Could not open file: " << filename << endl;
	}
	f.close();
}

//begin scheduler simulation
int main(int argc, char **argv){
	int opt, quant;
	bool refoutv;
	while ((opt = getopt (argc, argv, "vs:")) != -1) {
		//instantiate a scheduler based on the -s option
        if (opt == 's') {
            if (optarg[0] == 'R' || optarg[0] == 'P') {
                sscanf(optarg + 1, "%d", &quant);
            } else {quant = 9999;}
            if (optarg[0] == 'F'){

            } else if (optarg[0] == 'L'){

            } else if (optarg[0] == 'P'){

            } else if (optarg[0] == 'R'){

            } else if (optarg[0] == 'S'){

            }
        }
        if (opt == 'v'){refoutv = true;}
    }
    string filename = argv[optind];
	string randfile = argv[optind+1];

	initialize(filename, randfile);

	

	return 0;
}