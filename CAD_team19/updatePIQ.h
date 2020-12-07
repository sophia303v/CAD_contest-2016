/****************************************************************************
  FileName     [ updatePIQ.h ]
  PackageName  [ STA ]
  Synopsis     [ do update PI queue . ]
  Author       [ Sophia Hsu ]
  identity     [ Student of NTUEE, Taiwan ]
  date         [ 2016 , 06 , 09] (last update)
****************************************************************************/

#ifndef UPDATEPIQ_H
#define UPDATEPIQ_H
#include "cirGate_S3.h"

int updatePIQ();//PO for 2,no PI for 0 ,has PI for 1 
bool DFS_PIQ(GatePin*);

#endif