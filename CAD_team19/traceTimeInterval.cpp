/****************************************************************************
  FileName     [ traceTimeInterval.cpp ]
  PackageName  [ STA ]
  Synopsis     [ DO time interval trace . ]
  Author       [ Sophia Hsu ]
  identity     [ Student of NTUEE, Taiwan ]
  date         [ 2016 , 06 , 08] (last update)
****************************************************************************/



#ifndef TRACETIMEINTERVAL_C
#define TRACETIMEINTERVAL_C

#include <queue>
#include "cirDef.h"

using namespace std;


bool DFS_VISIT(GatePin* pin);

void traceTimeInterval(){
     for (int i=0;i<PO_List.numBuckets();i++){
         for(int j=0;j<PO_List[i].size();j++){
                GatePin* pin = PO_List[i][j].second;
                if(pin->color()==WHITE){
                 //   cout<<"FLAG1"<<endl;
                    DFS_VISIT(pin);
                }
         }
     }

}

bool DFS_VISIT(GatePin* pin){
     pin->setcolor(GRAY);
     CirGate* gatenow = pin->_prev;
    // cout<<"FLAG2"<<endl;
     if(pin->gettype()=="PI"){ //到底了
         pin->setuppertime(0);
         pin->setlowertime(0);
         pin->settime(0);
         pin->printtime();
         return 0;
     }
     //如果不是PI
     for(int i=0;i<gatenow->_FANIN.size();i++){
             GatePin* inputpin = gatenow->_FANIN[i];
             // cout<<"FLAG3"<<endl;
             if(inputpin->color()==WHITE){
                  inputpin->_prev_dfs = pin;
                  DFS_VISIT(inputpin);
             }
     }
    // cout<<"FLAG4"<<endl;
     pin->setcolor(BLACK);
     
    // cout<<"FLAG5"<<endl;
     if(gatenow->getTypeStr()=="NOT"){
             GatePin* inputpin=gatenow->_FANIN[0];
             pin->setuppertime(inputpin->uppertime()+1);
             pin->_prev_upper = inputpin;
             pin->setlowertime(inputpin->lowertime()+1);
     }
     else {//NAND oR NOR
             GatePin* inputpin_0 = gatenow->_FANIN[0];
             GatePin* inputpin_1 = gatenow->_FANIN[1];
             if(inputpin_0->uppertime() >inputpin_1-> uppertime()){
                   pin->setuppertime(inputpin_0->uppertime()+1);
                   pin->_prev_upper = inputpin_0;
                   }
             else  {pin->setuppertime(inputpin_1->uppertime()+1);
                   pin->_prev_upper = inputpin_1;
             }
             
             if(inputpin_0->lowertime() <inputpin_1-> lowertime())
                  pin->setlowertime(inputpin_0->lowertime()+1);
             else pin->setlowertime(inputpin_1->lowertime()+1);
     }
     
     
     pin->printtime();


}
//void printalltime_inBFS();
/**
void traceTimeInterval(){
   //starts BFS
  //先將所有的PI放入queue
  cout<<"Start to trace Time Interval:...."<<endl;
  queue<GatePin*> Q;
  for (int i=0;i<PI_List.numBuckets();i++){
         for(int j=0;j<PI_List[i].size();j++){
                 GatePin* PI = PI_List[i][j].second;
                 Q.push(PI);
                 PI->settime(0);
                 PI->setlowertime(0);
                 PI->setuppertime(0);
                 PI->setcolor(GRAY);
                 PI->printtime();
                 
         }
         cout<<"FLAG1";
     }
  while(!Q.empty()){
        GatePin* u = Q.front();
        Q.pop();
        vector<CirGate*> u_output = u->_next;
        for(int i=0;i<u_output.size();i++){
                CirGate* gatenow = u_output[i];
                GatePin* pinnow = u_output[i]->_FANOUT;
                if(pinnow->color() == WHITE){
                    pinnow->setcolor(GRAY);
                    unsigned input0_L=gatenow->_FANIN[0]->lowertime();
                    unsigned input1_L=gatenow->_FANIN[1]->lowertime();
                    //setlowertime
                    if(input0_L<input1_L)
                        pinnow ->setlowertime(input0_L+1);
                    else pinnow->setlowertime(input1_L+1);
                    //setuppertime
                    unsigned input0_U=gatenow->_FANIN[0]->uppertime();
                    unsigned input1_U=gatenow->_FANIN[1]->uppertime();
                    if(input0_U>input1_U)
                        pinnow ->setuppertime(input0_U+1);
                    else pinnow ->setuppertime(input1_U+1);
                    
                    
                    //如果想要用成BFSgraph
                    //這裡要加把這個pin的BFSgraphprev設成上一個
                    pinnow->printtime();
                    Q.push(pinnow);
                }
                
        }
        u->setcolor(BLACK);
  }
        
  cout<<"End Trace !"<<endl;
  
                        
}

*/
/*
//used for debug
void printalltime_inBFS(){
    //會把所有的BLACK重設回WHITE
    //starts BFS
  //先將所有的PI放入queue
  cout<<"Start to print Time Interval:...."<<endl;
  queue<GatePin*> Q;
  for (int i=0;i<PI_List.numBuckets();i++){
         for(int j=0;j<PI_List[i].size();j++){
                 GatePin* PI = PI_List[i][j].second;
                 Q.push(PI);
                 PI->printtime();
                 PI->setcolor(GRAY);
         }
     }
  while(!Q.empty()){
        GatePin* u = Q.front();
        Q.pop();
        vector<CirGate*> u_output = u->_next;
        for(int i=0;i<u_output.size();i++){
                CirGate* gatenow = u_output[i];
                GatePin* pinnow = u_output[i]->_FANOUT;
                if(pinnow->color() == BLACK){
                    pinnow->setcolor(GRAY);
                    unsigned input0_L=gatenow->_FANIN[0]->lowertime();
                    unsigned input1_L=gatenow->_FANIN[1]->lowertime();
                    //setlowertime
                    if(input0_L<input1_L)
                        pinnow ->setlowertime(input0_L+1);
                    else pinnow->setlowertime(input1_L+1);
                    //setuppertime
                    unsigned input0_U=gatenow->_FANIN[0]->uppertime();
                    unsigned input1_U=gatenow->_FANIN[1]->uppertime();
                    if(input0_U>input1_U)
                        pinnow ->setuppertime(input0_U+1);
                    else pinnow ->setuppertime(input1_U+1);
                    
                    
                    //如果想要用成BFSgraph
                    //這裡要加把這個pin的BFSgraphprev設成上一個
                    
                    Q.push(pinnow);
                }
               
        }
         u->setcolor(WHITE);
  }
        
  cout<<"End print !"<<endl;


}
*/

#endif
