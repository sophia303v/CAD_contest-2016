/****************************************************************************
  FileName     [ readgraph.cpp ]
  PackageName  [ STA ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Sophia Hsu ]
  identity     [ Student of NTUEE, Taiwan ]
  date         [ 2016 , 06 , 09] (last update)
****************************************************************************/
#include <iostream>
#include <fstream>
#include "readcase.h"
#include "readsdc.h"
#include "cirDef.h"
#include "readgraph.h"

using namespace std;
/*
typedef HashMap<HashKey , CirGate*> GateList;
typedef HashMap<HashKey , GatePin*> PinList;
*/
unsigned  _I =0, _P =0, _PO=0, _NA =0, _NOR =0, _NOT =0 , _W=0; //M = I+O+NA+NOR + NOT

//I: #ofinputs , _P:#of Probes , PO:
 PinList PI_List;
 PinList PROBE_List;
 PinList  WIRE_List;
 PinList PO_List;
 GateList AOIG_List;

string FILENAME; //Filepath
fstream fp;


vector <GatePin*> Path_Pin;
vector <GatePin*> PIQ;

unsigned traveled = 5;//�C���@��path�n+1
unsigned truepathmark = 1;//�C���@��path�n+1
unsigned globalmark_PIQ =1;

extern unsigned index_PIQ;
 
//1. �@�}�l�|����input output�M wire
//input�T�w�n���ŧi
//output�]�O
//2.Ū�C�檺�ɭԡA�|����gate type�A�٦����input���W�r�Τ@��output���W�r
bool readgraph(string File){
     //cout<<"start"<<endl;
     string sdcname;
     FILENAME = File;
     sdcname=FILENAME;
     sdcname.append(".sdc");
     fpsdc.open(sdcname.c_str(), ios::in);
     if(!fpsdc){
       cout<<"Fail to open "<<sdcname<<endl;
       return 0;
     }
     
     //cout <<"open"<<sdcname<<endl;
     if(!readsdc()) return 0;
     
     fp.open(FILENAME.c_str(), ios::in);
	   if(!fp){
     	    cerr<<"Fail to open "<<FILENAME<<endl;
          cerr<<"Please check your files position or read readme again! 3Q !"<<endl;
		        return 0;
     }
     //cout<<"open"<<FILENAME<<endl;
	   if(!getfirstline()){
           cerr<<"no information"; return 0;
      }
            //�w�g�o��Ҧ�input�Moutput�Mwire���W�r
	   //cout<<"inputs: ";
       //�Ninput pin�إߨçQ�ΥL�̪��W�r��iHash�̡A�o�ɥL���٨S��output
     _I = inputNames.size();
     int bucketsPI=0;
     if((_I/3)==0) bucketsPI=1;
     else bucketsPI=_I/3;
     PI_List.init(bucketsPI);  //�Ninput ��list��hash initial�@�Ӥj�pw
	   for( size_t i=0; i<inputNames.size(); i++){
         GatePin* PI = new GatePin(inputNames[i],"PI");//�ŧi�@�ӳo�ӦW�r��input
         HashKey k(inputNames[i]);//�Q�γo�ӦW�r���ͤ@��key
         if(!PI_List.insert(k,PI)){ /*cout<<"Error!!";*/ return 0;} //�N�o��PIN�s�P�o��key�s�iPI��hash
 		    // cout<<inputNames[i]<<" "<<endl;
        }
    //cout<<"input parse end;"<<endl;
	   //cout<<endl;
	   //cout<<"outputs: ";
        //�Noutput pin�إߨçQ�ΥL�̪��W�r��iHash�̡A�o�ɥL���٨S��input	
     _P = outputNames.size();
      int bucketsPROBE=0;
     if((_P/3)==0) bucketsPROBE=1;
     else bucketsPROBE=_P/3;
     PROBE_List.init(bucketsPROBE); //�Noutput ��list��hash initial�@�Ӥj�pw
       for(size_t i=0; i<outputNames.size(); i++){
                  GatePin* Probe =new GatePin(outputNames[i],"PROBE");//�ŧi�@�ӳo�ӦW�r��output
                  HashKey k(outputNames[i]);//�Q�γo�ӦW�r���ͤ@��key
                  if(!PROBE_List.insert(k,Probe)) {/*cout <<"Error!";*/ return 0;}
                //  cout<<outputNames[i]<<" ";
       }
       //cout<<"output parse end;"<<endl;
	   //cout<<endl;
       //�b�o�̫ŧiwire�A�Alink gate���ɭԪ��������Y�i
	  //cout<<"wires: ";
     _W = wireNames.size();
     int bucketsW=0;
     if(_W/10==0) bucketsW=1;
     else bucketsW=_W/10;
     WIRE_List.init(bucketsW);//�NPIN ��PINlist��hash initial�@�Ӥj�pw
	  for(size_t i= 0; i<wireNames.size(); i++){
               GatePin* Wire = new GatePin(wireNames[i],"WIRE");
               HashKey k(wireNames[i]);
               if(!WIRE_List.insert(k,Wire)) {/*cout<<"Error!"*/; return 0;}
              // cout<<wireNames[i]<<" ";
    }
    //cout<<"wire parse end;"<<endl;
  
   
    //�}�lŪgate��T
    //cout<<"Gate : "<<endl;
    //cout<<"parse gate....."<<endl;

    AOIG_List.init(10);//��l��AIG_List�o��hash��bucket�j�p
	  while(readline()){
         //   cout<<nowgate<<' '<<nowname<<' '<<nowarg[0]<<' '<<nowarg[1]<<' '<<nowarg[2]<<endl; 
          if(nowgate==NOT)
                create_notgate();
          else if(nowgate ==NAND)
               {
               create_nandgate();
               }
          else if(nowgate ==NOR)
               {create_norgate();}
        
   }
   //cout<<"parse gate end ! "<<endl;
 
   //cout<<"link gates..."<<endl;

   PO_List.init(5);
   setPO();
   //cout<<"link gate end ! "<<endl;

  
 
   //cout<<"do u want to print information?(y/n) "<<endl;
   //cin>>tmp;
   //if(tmp=='y'){
    //cout<<"NOT_Gate numbers = "<<_NOT<<endl;
    //cout<<"NAND_Gate numbers = "<<_NA<<endl;
    //cout<<"NOR_Gate numbers = "<<_NOR<<endl;
    //printallPI();
    //printallPO();
    //printallGate();
   //}
   
   //used for debug
   //printallPI();
   //printallPO();
   //printallGate();
   return true;
   
}

bool create_notgate(){
                //�Ыؤ@��NOT Gate�A��iAIG_List
                _NOT++;//�N_NOT�ƶq+1
                CirGate* _not = new NOT_Gate(nowname); //�Ыؤ@��NOT Gate
                HashKey k(nowname);//�Q��NOT Gate���W�r���ͤ@��hash key
                if(!AOIG_List.insert(k,_not)){/*cout<<"Error";*/return 0;}//�NNOT Gate��iHash�̡A�p�G�L��key�w�g�b�̭��F�hcout error
                //�]�wNOT��input�Moutput
//���]�winput
                //nowarg[0] for output , nowarg[1]for input
                string type; 
                GatePin* inputpin = getPin(nowarg[1],type);
                if(inputpin ==false) {
                            //cout<<"notgate "<<nowname<<' '<<endl;
                            return 0;
                }
                else {//�L�׬OPI�BProbe�٬OWIRE���i�H�o�˥�
                     //gate part
                      _not->_FANIN.push_back(inputpin);//�]�w�o��gate��input pin
                         //wire part
                      inputpin->_next.push_back(_not);//�]�w��inputpin��output gate
                }
                
           
            
                //�]�woutput
                string type2;
                GatePin* outputpin = getPin(nowarg[0],type);
                if(outputpin==false) {
                  //cout<<"notgate "<<nowname<<' '<<endl;
                return 0;
                }
                else{
                     _not->_FANOUT=outputpin;//�]�w�o��gate��output pin //�u�|���@��Gate�֦��o��output pin
                     outputpin->_prev = _not;
                }
          
}

bool create_nandgate(){  
//�Ыؤ@��NAND Gate�A��iAIG_List
                _NA++;//�N_NOT�ƶq+1
                CirGate* _nand = new NAND_Gate(nowname); //�Ыؤ@��NAND Gate
                HashKey k(nowname);//�Q��NAND Gate���W�r���ͤ@��hash key
                if(!AOIG_List.insert(k,_nand)){/*cout<<"Error";*/return 0;}//�NNAND Gate��iHash�̡A�p�G�L��key�w�g�b�̭��F�hcout error
                //�]�wNAND��input�Moutput
                //���]�winput*2
                //nowarg[0] for output , nowarg[1]�B[2]for input 
                for (int i=1;i<=2;i++){
                    string type;
                    GatePin* inputpin =getPin(nowarg[i],type);
                     if(inputpin ==false){ /*cout<<"nandgate "<<nowname<<' '<<nowarg[i]<<" "<<endl;*/ return 0;}
                      else {//�L�׬OPI�BProbe�٬OWIRE���i�H�o�˥�
                     //gate part
                      _nand->_FANIN.push_back(inputpin);//�]�w�o��gate��input pin
                         //wire part
                      inputpin->_next.push_back(_nand);//�]�w��inputpin��output gate
                      }
                }
             
               string type2;
                GatePin* outputpin = getPin(nowarg[0],type2);
                if(outputpin==false) { /*cout<<"nandgate "<<nowname<<' '<<nowarg[0]<<" "<<endl; */return 0;}
                else{
                     _nand->_FANOUT=outputpin;//�]�w�o��gate��output pin //�u�|���@��Gate�֦��o��output pin
                     outputpin->_prev = _nand;
                }
}

bool create_norgate() {
     //�Ыؤ@��NOR Gate�A��iAIG_List
                _NOR++;
                CirGate* _nor = new NOR_Gate(nowname); 
                HashKey k(nowname);
                if(!AOIG_List.insert(k,_nor)){/*cout<<"Error";*/return 0;}
           
               
                 for (int i=1;i<=2;i++){
                    string type;
                    GatePin* inputpin =getPin(nowarg[i],type);
                     if(inputpin ==false) { /*cout<<"norgate "<<nowname<<' '<<nowarg[i]<<" "<<endl;*/ return 0;}
                      else {//�L�׬OPI�BProbe�٬OWIRE���i�H�o�˥�
                     //gate part
                      _nor->_FANIN.push_back(inputpin);//�]�w�o��gate��input pin
                         //wire part
                      inputpin->_next.push_back(_nor);//�]�w��inputpin��output gate
                      }
                }
                
                 string type2;
                GatePin* outputpin = getPin(nowarg[0],type2);
                if(outputpin==false) { /*cout<<"norgate "<<nowname<<' '<<nowarg[0]<<" "<<endl;*/ return 0;}
                else{
                     _nor->_FANOUT=outputpin;//�]�w�o��gate��output pin //�u�|���@��Gate�֦��o��output pin
                     outputpin->_prev = _nor;
                    // cout<<"flag"<<endl;
                }
}
CirGate* getGate(string id, string type="NaN"){
         HashKey k(id);
    
         if (type =="NAND" || type =="OR" || type =="NOT"){
              CirGate* tmp;
              if(AOIG_List.check(k,tmp)) return tmp;
              else {
              //cout <<" we don't have this Gate !"<<endl; 
              return 0;}
         }
         else {
              //cout<<"Error !! we dont have this Gate!"<<endl; 
              return 0;
         } 
         
}

GatePin* getPin(string id){
         HashKey k(id);
        // cout<<"arg = "<<id<<endl;
         GatePin* tmp;
         if(WIRE_List.check(k,tmp)) {
             //cout<<type<<endl;
             return tmp;
         }
         else if(PI_List.check(k,tmp)){
              return tmp;
         }
         else if(PROBE_List.check(k,tmp)){
              return tmp;
         }

         else {/*cout <<"Error !! we don't have this Pin id =["<<id<<"] ! "<<endl; */return 0;}
}
GatePin* getPin(string id,string& type){
         HashKey k(id);
        // cout<<"arg = "<<id<<endl;
         GatePin* tmp;
         if(WIRE_List.check(k,tmp)) {
             type = "WIRE";
             //cout<<type<<endl;
             return tmp;
         }
         else if(PI_List.check(k,tmp)){
              type ="PI";
            //  cout<<type<<endl;
              return tmp;
         }
         else if(PROBE_List.check(k,tmp)){
              type ="PROBE";
            //  cout<<type<<endl;
              return tmp;
         }

         else {
         /*cout <<"Error !! we don't have this Pin id = ["<<id<<"]! "<<endl;*/ return 0;}
}

bool setPO(){
     for(int i=0;i<PROBE_List.numBuckets();i++){
             for(int j=0;j<PROBE_List[i].size();j++){
                     if(PROBE_List[i][j].second->isnooutput()){
                           HashKey k = PROBE_List[i][j].first;
                           GatePin* PO = PROBE_List[i][j].second;
                           PO->settype("PO");
                           //cout<<"PIN: "<<PO->Id() <<"is a PO"<<endl;
                           //cout<<"its key value = "<<k()<<endl;
                           if(!PO_List.insert(k,PO)){ /*cout<<"error!"; */return false;}
                           _PO++;
                     }
             }
     }
     return true;
}
GatePin* getPI(string id){
         HashKey k(id);
         GatePin* tmp;
         if(PI_List.check(k,tmp)) return tmp;
         else {/*cout<<"this pin is not PI!"<<endl;*/ return 0;}    
}     
GatePin* getPO(string id){
          HashKey k(id);
         GatePin* tmp;
         if(PO_List.check(k,tmp)) return tmp;
         else {/*cout<<"this pin is not PO!"<<endl;*/ return 0;}
         
}
//set global reference
void renew(){//�C���@��path�n+1���� 
     traveled++;
     truepathmark++;
     globalmark_PIQ++;
     index_PIQ=0;
     /*cout<<" traveled = "<< traveled<<endl;
     cout<<" t = "<< t<<endl;
     cout<<" globalmark_PIQ = "<< globalmark_PIQ<<endl;*/
     PIQ.clear();
   //  checkTruePathNeg();
}



//check if any pin has no output
//except PO and probe
//it seems no casetest have this problems XD
void checkpinoutput(){
     vector<GatePin*> unused;
      for (int i=0;i<PI_List.numBuckets();i++){
         for(int j=0;j<PI_List[i].size();j++){
                 if(PI_List[i][j].second->isnooutput()) unused.push_back(PI_List[i][j].second);
         }
     }
     for (int i=0;i<WIRE_List.numBuckets();i++){
         for(int j=0;j<WIRE_List[i].size();j++){
                 if(WIRE_List[i][j].second->isnooutput()) unused.push_back(WIRE_List[i][j].second);
         }
     }
     
     for(int i=0;i<unused.size();i++){
         //cout<<"Pin "<<unused[i]->gettype()<<' '<<unused[i]->Id()<<"has no output"<<endl;
     }
}




//used for debug
void checkifinputissame(){
      for(int i =0;i<AOIG_List.numBuckets();i++){
             for(int j=0;j<AOIG_List[i].size();j++){
                     CirGate* gate = AOIG_List[i][j].second;
                     if(gate->inputissame()) {
                     
                        //cout<<"this gate has the same input!"<<endl;
                        gate->printGate();
                     }
                    
             }
     }
     
}


void printallPI(){
     //cout<<"PI :"<<endl;
     /*for (int i=0;i<PI_List.numBuckets();i++){
         for(int j=0;j<PI_List[i].size();j++){
                 //cout<<PI_List[i][j].second->Id()<<' ';
         }
     }*/
     //cout<<endl;
}

void printallPO(){
    /* cout<<"PO :"<<endl;
     for (int i=0;i<PO_List.numBuckets();i++){
         for(int j=0;j<PO_List[i].size();j++){
                 cout<<PO_List[i][j].second->Id()<<' ';
         }
     }
     cout<<endl;
*/}
void printallGate(){
     
    
  /*   cout<<"AOIG :"<<endl;
     for(int i =0;i<AOIG_List.numBuckets();i++){
             for(int j=0;j<AOIG_List[i].size();j++){
                     CirGate* gate = AOIG_List[i][j].second;
                     gate->printGate();
                     unsigned tmp = gate->getcvalue();
                     cout<<tmp<<endl;
             }
     }
     cout<<endl;
*/
}


