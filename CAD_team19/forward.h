/****************************************************************************
  FileName     [ forward.h ]
  PackageName  [ STA ]
  Synopsis     [ Forward check whether there is conflict ]
  Author       [ Hsiang-Ya CHAO ]
  identity     [ Student of NTUEE, Taiwan ]
  date         [ 2016 , 06 , 09] (last update)
****************************************************************************/



#ifndef FORWARD_H
#define FORWARD_H

#include <iostream>
#include <vector>
#include "cirGate_S3.h"


using namespace std;

//class PrevValue   // 記錄改了甚麼值
//{
//   public:
//    PrevValue(GatePin* pin);
//    GatePin* changed_pin;
//    unsigned prev_time;
//    unsigned prev_logic;

//};

bool forward(GatePin* pin, unsigned value , vector<GatePin* > & record);   //輸入PI
void undo(vector<GatePin* > record);


#endif
