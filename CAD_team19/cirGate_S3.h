/****************************************************************************
  FileName     [ cirGate_S3.h ]
  PackageName  [ STA ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Sophia Hsu ]
  identity     [ Student of NTUEE, Taiwan ]
  date         [ 2016 , 06 , 09] (last update)
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include <climits>
#include "myHashMap_STA.h"


using namespace std;
//used in GatePin
enum COLOR{
     WHITE=0,
     GRAY=1,
     BLACK=2
};

enum gatetype
{	NOT 	= 0,
	NAND 	= 1,
	NOR		= 2,
	
	TOT		= 5
};

extern unsigned traveled;//每換一條path要+1
extern unsigned truepathmark;//每換一條path要+1
extern unsigned globalmark_PIQ;


class CirGate;
class GatePin;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
// Define our data members and member functions, or classes
/* [ CirGate ] 每個Gatetype()都繼承這個class，這裡會定義每個gate都有的function以及variable。*/
//printgate待補 ，希望可以cout這個gate的所有資訊
class CirGate
{

 public:
        CirGate(string id):    //將這個gate的名字傳進來，_gateID就是這個gate的名字
                    _gateID(id),_usedtimes(0),_truepath(0),_FANOUT(0),_prev_dij(0) {}
        virtual ~CirGate() {}

        virtual string getTypeStr() const =0;
        virtual gatetype getType() const = 0;
        string Id() const {return _gateID;}
        virtual GatePin* getOffPin(GatePin* Pinnow) {}

        virtual unsigned getOffPinvalue(GatePin* Pinnow)  {}
        virtual int getcvalue()const {} //getcontrolling value
        void plusUsedTimes(){ _usedtimes++;}
        unsigned getusedtimes(){ return _usedtimes;};

        virtual void undo(){}//待補
        bool inputissame() {return (_FANIN[0]==_FANIN[1]);}
        bool isnooutput (){return (_FANOUT==0);}//給PIGate用的，可刪掉

        //used for dijkstra
        CirGate* _prev_dij;
        int  d_dij;

        //used for debug
        virtual void printGate() const {} //印出此gate的資訊

        //used for source  //其他的不用這個
        virtual vector<GatePin*> getFANOUT() {}
        virtual void FANOUTpush_back(GatePin* pin){}

		//used for outputfile
		virtual string whichpin(GatePin* pinnow){}
		
        //變數
        vector <GatePin*> _FANIN;
        GatePin* _FANOUT;
 protected:
        string _gateID;
        unsigned _usedtimes;
        bool _truepath;

};


class GatePin   //used to fanin
{
   //friend ostream& operator << (ostream&,const GatePin&);

   public:
      GatePin(string name,string type):  _id(name), _prev(0),_logicvalue(2),_type(type),_time(INT_MAX),_lowertime(INT_MAX),_uppertime(INT_MAX),_color(WHITE),_trvelmark(0),_prev_dfs(0),_truepathmark(0),_locmark_PIQ(0),_prev_upper(0){}
       ~GatePin() {}
      // used in getting information about this pin
      string Id() const { return _id; }
      CirGate* prev() const { return _prev; }

      string getprevname() {return _previd;}

      string gettype(){return _type;} //"PI"、"PO"、"WIRE、"PROBE"

      //used in constructing netlist
      void settype(string ty){_type = ty;}
      void setprevname(string name){_previd = name;}
      void setprev(CirGate* prev) {
           _prev = prev;
           setprevname(prev->Id());
           }
      //used in get Time
      unsigned uppertime(){return _uppertime;}
      unsigned lowertime(){return _lowertime;}
      unsigned time() {return _time;}

      //used in set Time
      void setuppertime(unsigned t){_uppertime = t;}
      void settime(unsigned t){_time =t;}
      void setlowertime(unsigned t){_lowertime =t;}

      //used in confirm whether this pin is PO
      bool isnooutput() {return (_next.size()==0);}
      //used in logic value
      //not every function will be used , i just write those i think u may use .
      unsigned getvalue() const { return _logicvalue;}
      void setvalue(unsigned v) {_logicvalue = v;}
//here are those should be in renew
//global marks vs local mark
      //used in traveled and untraveled
      bool istraveled()  { return (_trvelmark == traveled);}
      void settraveled() { _trvelmark = traveled;}
      void setuntraveled() { _trvelmark = traveled-1;}

      //used in truepath
      void settruepath(){ _truepathmark = truepathmark;}
      bool istruepath(){return (_truepathmark==truepathmark);}
      
      //used in updatePIQ
      void set_marked_PIQ(){ _locmark_PIQ = globalmark_PIQ;}
      bool is_marked_PIQ() { return (_locmark_PIQ==globalmark_PIQ);}
      
      
      //used in graph color
      COLOR color(){return _color;}
      COLOR setcolor(COLOR c){_color=c;}


      //used for debug
      void printtime(){
           /*cout<<"Pin :"<<_type<<" "<<_id<<endl;
           cout<<" lowertime = " <<_lowertime<<endl;
           cout<<" uppertime = " <<_uppertime<<endl;
           if(_type!= "PI"){
           cout<<" uppertime's pin = "<<_prev_upper->Id()<<"time = "<<_prev_upper->uppertime()<<endl;}
           cout<<" time = " <<_time<<endl;*/
      }
      

      
      GatePin* _prev_dfs;
      GatePin* _prev_upper;
      CirGate* _prev;
      vector<CirGate*> _next;

   private:
      string _id; //id for identification , means name
      string _previd;
      string _type;
      COLOR _color;
      unsigned _logicvalue;
      unsigned _uppertime;
      unsigned _lowertime;
      unsigned _time;
      unsigned _trvelmark;
      unsigned _truepathmark;
      unsigned _locmark_PIQ;

};


class NAND_Gate : public CirGate
{
   public:
      NAND_Gate(string ID): CirGate(ID){}
      virtual ~NAND_Gate() {}

      virtual string getTypeStr() const {return "NAND";}
      virtual gatetype getType() const {return NAND;}
      virtual void printGate() const
      {
#ifdef debugGate
         cout<<"NAND :"<<_gateID<<' '<<_FANOUT->Id()<<' '<<_FANIN[0]->Id()<<' '<<_FANIN[1]->Id()<<' '<<endl;
#endif
      }
      virtual int getcvalue() const{ return 0;}
      //virtual bool is_floating() const {
        // return (_FANIN[0].is_floating()||_FANIN[1].is_floating()); }
     // virtual bool is_unused() const {
       //  return _FANOUT.size()==0; }

      virtual GatePin* getOffPin(GatePin* Pinnow) {
              if(Pinnow == _FANIN[0]) return _FANIN[1];
              else if (Pinnow == _FANIN[1]) return _FANIN[0];
              else {
#ifdef debugGate
     				cout <<"error! this Pin is not in this gate!";
#endif
              		}
              }

      virtual unsigned getOffPinvalue(GatePin* Pinnow)  {
              GatePin* pin = getOffPin(Pinnow);
              return pin->getvalue();
      }
	  //used for outputfile
	  virtual string whichpin(GatePin* pinnow){
		  if(_FANIN[0]==pinnow) return "A";
		  else if(_FANIN[1]==pinnow) return "B";
		  else { 
#ifdef debugGate
		  cout<<"error! this Pin is not in this gate!"<<endl;
#endif
			return 0;}
	  
	  }
	  
	  
};

class NOR_Gate : public CirGate
{
 public:
        NOR_Gate(string Id): CirGate(Id){}
        virtual ~NOR_Gate() {}

        virtual string getTypeStr() const {return "NOR";}
         virtual gatetype getType() const {return NOR;}
        virtual void printGate() const
        {
#ifdef debugGate
                cout<<"NOR :"<<_gateID<<' '<<_FANOUT->Id()<<' '<<_FANIN[0]->Id()<<' '<<_FANIN[1]->Id()<<' '<<endl;
#endif
		}
        virtual int getcvalue() const{ return 1;}
          //virtual bool is_floating() const {
        // return (_FANIN[0].is_floating()||_FANIN[1].is_floating()); }
     // virtual bool is_unused() const {
       //  return _FANOUT.size()==0; }

      virtual GatePin* getOffPin(GatePin* Pinnow) {
              if(Pinnow == _FANIN[0]) return _FANIN[1];
              else if (Pinnow == _FANIN[1]) return _FANIN[0];
              else {
                   cout <<"error! this Pin is not in this gate!";
                   }
      }

      virtual unsigned getOffPinvalue(GatePin* Pinnow) {
              GatePin* pin = getOffPin(Pinnow);
              return pin->getvalue();
      }
	   virtual string whichpin(GatePin* pinnow){
		  if(_FANIN[0]==pinnow) return "A";
		  else if(_FANIN[1]==pinnow) return "B";
		  else { cout<<"error! this Pin is not in this gate!"<<endl;return 0;}
		  
	  }
	  
};
class NOT_Gate : public CirGate
{
   public:
      NOT_Gate(string Id): CirGate(Id) {}
      virtual ~NOT_Gate() {}

      virtual string getTypeStr() const {return "NOT";}
       virtual gatetype getType() const {return NOT;}
      virtual void printGate() const
      {
#ifdef debugGate
         cout<<"NOT :"<<_gateID<<' '<<_FANOUT->Id()<<' '<<_FANIN[0]->Id()<<' '<<endl;
#endif
		}
      //virtual bool is_floating() const {
        // return (_FANIN[0].is_floating()||_FANIN[1].is_floating()); }
     // virtual bool is_unused() const {
       //  return _FANOUT.size()==0; }
       virtual int getcvalue() {
       cout<<"NOT doesn't have cvalue !"<<endl; return INT_MAX;}
		virtual GatePin* getOffPin(GatePin* Pinnow) {
                   cout <<"error! Not_Gate doesn't have offPin!"<<endl;
                   return 0;
      }

      virtual unsigned getOffPinvalue(GatePin* Pinnow)  {
                       cout<<"error! Not_Gate doesn't have offPin!"<<endl;
                       return 0;
      }
	   virtual string whichpin(GatePin* pinnow){
		  if(_FANIN[0]==pinnow) return "A";
		  else { cout<<"error! this Pin is not in this gate!"<<endl;return 0;}
		  
	  }
	  
};

class SRC_Gate : public CirGate
{
      public:
      SRC_Gate(string Id): CirGate(Id) {}
      virtual ~SRC_Gate() {}

      virtual string getTypeStr() const {return "SRC";}
      virtual gatetype getType() const {return TOT;}
       virtual void printGate() const
      {
         cout<<_gateID<<' ';
         for(int i=0;i<_FANOUT_PI.size();i++)
            cout<<"FANOUT ["<<i<<"] = "<<_FANOUT_PI[i]->Id()<<endl;

         cout<<endl;
      }

      virtual vector<GatePin*> getFANOUT() { return _FANOUT_PI;}
      virtual void FANOUTpush_back(GatePin* pin){_FANOUT_PI.push_back(pin);}
      vector< GatePin*> _FANOUT_PI;

};


class SINK_Gate : public CirGate
{
      public:
      SINK_Gate(string id): CirGate(id) {}
      virtual ~SINK_Gate() {}
      
      virtual string getTypeStr() const {return "SINK";}
      virtual gatetype getType() const {return TOT;}
      virtual void printGate() const
      {
         cout<<_gateID<<' ';
         for(int i=0;i<_FANIN.size();i++)
            cout<<"FANIN ["<<i<<"] = "<<_FANIN[i]->Id()<<endl;

         cout<<endl;
      }

     //原本就有一個vecotor <GatePin*> _FANIN;

};


 #endif
