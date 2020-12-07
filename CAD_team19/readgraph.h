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
CirGate* getGate(string id); //��Jgate��id���A��gate��pointer
GatePin* getPin(string id,string& type);//��Jpin��id���A��pin��pointer�Atype�|�^�ǥ��O����Pin
GatePin* getPin(string id);//��Jpin��id���A��pin��pointer
GatePin* getPO(string id);//��JPO��pin id ���A��pointer
GatePin* getPI(string id);//��JPI��pin id ���A��pointer

void renew();//set global reference

//optimize
void checkpinoutput();////check if any pin has no output ,except PO and probe

//debug
void printallGate(); //���L�C��gate�A�p�G�ݭn�@�Ӥ@��gate���L�i�H�ݳo��function���g��
void printallPI();//���L�C��input�A�p�G�ݭn�@�Ӥ@��input���L�i�H�ݳo��function���g��
void printallPO();



#endif