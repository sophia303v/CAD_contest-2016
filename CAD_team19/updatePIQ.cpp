/****************************************************************************
  FileName     [ updatePIQ.cpp ]
  PackageName  [ STA ]
  Synopsis     [ do update PI queue . ]
  Author       [ Sophia Hsu ]
  identity     [ Student of NTUEE, Taiwan ]
  date         [ 2016 , 06 , 0*] (last update)
****************************************************************************/
#include "updatePIQ.h"
#include "cirDef.h"
int PInumber =0;
unsigned index_PIQ =0;


int updatePIQ(){
     if(PIQ.empty()) {Path_Pin.push_back(PIQ[0]);}
     if(!PIQ.empty() && PIQ[0]->gettype()=="PI"){PIQ[0]->set_marked_PIQ();}
     //if(index_PIQ>=Path_Pin.size()) cout<<"error"<<endl;
     
   //  cout<<"FLAG1"<<endl;
     
     GatePin* pinprev = Path_Pin[index_PIQ];
     if(pinprev->gettype()=="PO") 
     {
#ifdef debugPIQ
     	cout<<"it is PO now !"<<endl;
#endif
     	return 2;}
     GatePin* pinnext = Path_Pin[index_PIQ+1];
     CirGate* gatenow = pinnext->_prev;
      
    //  cout<<"FLAG2"<<endl;
      
     index_PIQ++;
     PInumber=0;
     if(gatenow->getTypeStr()!="NOT"){
         GatePin* offpin = gatenow->getOffPin(pinprev);
         DFS_PIQ(offpin);
     }
     else {

#ifdef debugPIQ
          cout<<"NOT Gate doesn't have off pin!"<<endl;
#endif
		// return 0;
     }
     
   //   cout<<"FLAG3"<<endl;
    //顯示資訊   

#ifdef debugPIQ
	cout<<"we are in gate "<<gatenow->Id()<<" now."<<endl;
     cout<<"we find "<<PInumber<<" inputs"<<endl;
     cout<<"inputs in PIQ :"<<endl;     
     for(int i=0;i<PIQ.size();i++){
             cout<<PIQ[i]->Id()<<' ';
     }
     cout<<endl;
     cout<<"FLAG5"<<endl;
#endif     
   //回傳值 沒找到 0 有找到 1 //回傳2代表到達PO
     if (PInumber==0) return false;
     else return true;
     

}

bool DFS_PIQ(GatePin* pin){
     if (pin->gettype()=="PI") {
        if(!pin->is_marked_PIQ()){
            PIQ.push_back(pin);
            pin->set_marked_PIQ();
            PInumber++;
            return true;
        }
        return false;
     }

#ifdef debugPIQ
	cout<<"FLAG4"<<endl;
#endif
	CirGate* prevgate = pin->_prev;
     for(int i=0 ;i<prevgate->_FANIN.size(); i++){
             GatePin* inputpin = prevgate->_FANIN[i];
             if(!(inputpin->is_marked_PIQ())){
                 DFS_PIQ(inputpin);
             }
     }

#ifdef debugPIQ
     cout<<"FLAG5"<<endl;
#endif
	pin->set_marked_PIQ();

}

