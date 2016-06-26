# os-scheduler
-------------

# To compile:
`make`

without makefile : g++ -Wall -o scheduler scheduler.cpp

# To run:
`./runit.sh myoutput ./scheduler`


Synopsis:  
This is a discrete event simulation of an operating system scheduler. It takes an input file with information about processes to schedule and can run several scheduling algorithms: First Come First Serve, Last Come First Serve, Shortest Job First, Round Robin, Priority

A process is characterized by 4 parameters:
*Arrival Time (AT) 
*Total CPU Time (TC)
*CPU Burst (CB)
*IO Burst (IO). 