#ifndef LONGEST_PATH_H
#define LONGEST_PATH_H

#include <vector>
#include <cstring>
#include <stdlib.h>
#include "cirDef.h"
#include "graph.h"


using namespace std;


//int required_time =31;
//int slack = 6;           set in makefile


//vector <GatePin*> Path_Pin;  //Path



//func
bool longestpath();
void Print_Path(CirGate *s,CirGate *v);
void Read_Dijtime(CirGate *sourcenode,CirGate *endnode);
void Struct_prefix_tree(Node* s);
void Track_Path(Node *v);
Node* Extract_min(vector<Node *> &Q);
GatePin* Extract_max(vector<GatePin *> &Q);





#endif
