/****************************************************************************
  FileName     [ readgraph.h ]
  PackageName  [ STA ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Sophia Hsu ]
  identity     [ Student of NTUEE, Taiwan ]
  date         [ 2016 , 06 , 02] (last update)
****************************************************************************/



#ifndef READ_GRAPH_H
#define READ_GRAPH_H

#include <iostream>
#include "cirGate_S3.h"
using namespace std;

//constructing netlist
bool readgraph(string File);
bool create_notgate();
bool create_nandgate();
bool create_norgate();
bool setPO();


//get PIN or Gate
CirGate* getGate(string id); //輸入gate的id幫你找gate的pointer
GatePin* getPin(string id,string& type);//輸入pin的id幫你找pin的pointer，type會回傳它是哪種Pin
GatePin* getPin(string id);//輸入pin的id幫你找pin的pointer
GatePin* getPO(string id);//輸入PO的pin id 幫你找pointer
GatePin* getPI(string id);//輸入PI的pin id 幫你找pointer

void renew();//set global reference

//optimize
void checkpinoutput();////check if any pin has no output ,except PO and probe

//debug
void printallGate(); //走過每個gate，如果需要一個一個gate走過可以看這個function怎麼寫的
void printallPI();//走過每個input，如果需要一個一個input走過可以看這個function怎麼寫的
void printallPO();



#endif