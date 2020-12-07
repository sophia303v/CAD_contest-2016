//  Filename : readcase.h 
//	author : Anwei

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include"cirDef.h"

using namespace std;

#ifndef READCASE_H
#define READCASE_H

extern vector<string> inputNames, outputNames, wireNames;
extern gatetype nowgate;
extern string nowname, nowarg[3];//nowarg:out, in1, in2; in2 of NOT is brokenXD
bool getfirstline();    	//stop at the first line, return false if file end is reached(no gate information)
							//it will read in  inputNames & outputsNames & wireNames
bool readline();			//try! get information, return false when there's no more, throw if missing
//[helper]
extern fstream fp;
extern string line;
extern string gatename[3];
extern string gatearg[3][3]; 
extern size_t gatenum[3];
enum readtype
{	INPUT	=0,
	OUTPUT	=1,
	WIRE	=2,
	TOT_READ=3
};

bool getfirst(string& tostore, size_t first=0);//if continue, first = nposs;
bool getnowname();
bool gettype();
bool getcont();


extern vector<string>* readPorts[3];


#endif














