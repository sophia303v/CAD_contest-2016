/*用來定義一些整個電路都有的變數*/


#ifndef CIRDEF_H
#define CIRDEF_H

#include<iostream>
#include<cstring>
#include <string>
#include <vector>
#include "cirGate_S3.h"


extern int required_time;
extern int slack;
extern std::string benchmark;

extern unsigned  _I, _P, _PO, _NA, _NOR, _NOT , _W; //M = I+O+NA+NOR + NOT


typedef HashMap<HashKey , CirGate*> GateList;
typedef HashMap<HashKey , GatePin*> PinList;

extern PinList PI_List;
extern PinList PROBE_List;
extern PinList  WIRE_List;
extern PinList PO_List;
extern GateList AOIG_List;
 
extern vector <GatePin*> Path_Pin;
extern vector <GatePin*> PIQ;

#endif