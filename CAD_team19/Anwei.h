#include<iostream>
#include<vector>
#include<iomanip>
#include<cassert>
#define		ANWEI
#include "readcase.h"
#define longestPath 	Path_Pin
using namespace std;

//temperary
//vector<GatePin*> longestPath;
//vector<GatePin*>PIQ;
//bool updatePIQ(){return false;}

//void undo(vector<GatePin*>){return;}
//bool forward(GatePin*, size_t, vector<GatePin*>&){return true;}


//vector<size_t> PIQvalue;
void screenCheck()
{
	cout<<"PIQ: ";
	for(size_t i=0; i<PIQ.size(); i++)
	{   cout<<i<<":"<<PIQ[i]->Id()<<"-"<<PIQ[i]->getvalue()<<"("<<PIQ[i]->getvalue()<<"); ";}
	cout<<"longestPath: ";
	for(size_t i=0; i<longestPath.size(); i++)
	{   cout<<i<<":"<<longestPath[i]->Id()<<"-"<<longestPath[i]->getvalue();
	}
	cout<<endl;
}



bool SetTruePathValue(unsigned newValue );

bool timeCheck();

bool checkTruePath(size_t startPIQorder);//size_t =0;
bool checkTruePathNeg(size_t);//size_t = UINT_MAX   (->initial backtrack

void printHead(ofstream&);
void outfile(ofstream&, size_t);



bool SetTruePathValue(unsigned newValue=0)//TODO maybe need renew	PO don't need value evaluate
{	bool value = (newValue!=0);
	bool invert = false;
	unsigned t=0;
	for( ; t<longestPath.size(); t++)
	{	longestPath[t]->settime(t);
		longestPath[t]->settruepath();
		longestPath[t]->setvalue(((value!=invert)?1:0));
		longestPath[t]->settraveled();
		invert = (!invert);
//		cout<<"newvalue= "<<value<<endl;
//		cout<<"invert = "<<invert<<endl;
//		cout<<"push"<<((value!=invert)?1:0)<<endl;
//		cout<<"value"<<longestPath[t]->getvalue()<<endl;
	}	
//	longestPath[t]->settime(t-1);
//	longestPath[t]->setvalue(((value==invert)?1:0));
//	longestPath[t]->settruepath();
//	longestPath[t]->settraveled();

	value = (newValue!=0);
	invert = false;
	vector<GatePin*> undoList;
	for(size_t i =0; i<longestPath.size(); i++)
	{//	cout<<"ini"<<longestPath[i]->getvalue()<<endl;
		if(!forward(longestPath[i], ((value!=invert)?1:0), undoList))
		{/*	cout<<longestPath[i]->getvalue()<<"false"<<endl;*/ return false; }
		//else cout<<longestPath[i]->getvalue()<<"true"<<endl;
		invert = (!invert);
	}
	return true;


//settruepath for graph
//	for(t=1; t<longestPath.size()-1; t++)
//	{	longestPath[t]->prev()->settruepath();
//	}
}

bool timeCheck()
{	CirGate* nextGate=longestPath[longestPath.size()-1]->prev();
	GatePin* nowPin=0;
	for(int t=longestPath.size()-2; t>=0;t--)
	{	nowPin = longestPath[t];
		assert(nowPin->istraveled());
		if(nowPin->getvalue()!=nextGate->getcvalue())
		{	if(nextGate->getType()==NOR||nextGate->getType()==NAND)
				if(nextGate->getOffPin(nowPin)->lowertime() > nowPin->time())
					return false;
		}	
		nextGate = nowPin->prev();
	}

	return true;
}


//renew:
//	checkTruePathNeg(UINT_MAX)
//	reset time & logicvalue

bool checkTruePath(size_t start=1)
{	
#ifdef debugdel
	cout<<"checkTruePath: PIQ.size()"<<PIQ.size()<<endl;
#endif
	if(start==PIQ.size())
	{
#ifdef debugdel
		cout<<"checkTruePath: updating PIQ..."<<endl;
#endif
		int u ;
		do 
		{	u = updatePIQ();
#ifdef debugdel
		cout<<"checkTruePath: update code: "<<u<<endl;
#endif
		}
		while(u==0);
		if(u==2){return true;}//all related PI vertify;
	}

//0 vertify
	//set PIQ value 0
#ifdef debugdel
	cout<<"checkTruePath: assigning "<<start<<"th PIQ("<<PIQ[start]->Id()<<") 0"<<endl;
#endif
/*	if(start==PIQvalue.size())
		PIQvalue.push_back(0);
	else PIQvalue[start] = 0;*/
//	cout<<"assigned"<<endl;

	//vertify
	vector<GatePin*> undoList;
	if(forward(PIQ[start],0,undoList))				//itself is ok
	{
#ifdef debugdel
		cout<<"checkTruePath: forward(0) "<<start<<"th PIQ("<<PIQ[start]->Id()<<") success"<<endl;
		screenCheck();
#endif
		if(!checkTruePath(start+1))			//however it's all children are not
		{	undo(undoList);
#ifdef debugdel
	cout<<"checkTruePath: UNDO(0) "<<start<<"th PIQ("<<PIQ[start]->Id()<<")"<<endl;

			screenCheck();
#endif
			return checkTruePathNeg(start);// so it give out the chance to its partner
		}
		else return true;
	}
	else 
	{
#ifdef debugdel
		cout<<"checkTruePath: forward(0) "<<start<<"th PIQ("<<PIQ[start]->Id()<<") fail"<<endl;
		screenCheck();
#endif
		undo(undoList);
#ifdef debugdel
		cout<<"checkTruePath: UNDO(0) "<<start<<"th PIQ("<<PIQ[start]->Id()<<")"<<endl;
		screenCheck();
#endif
		return checkTruePathNeg(start);	//itself have trouble so give out the chance
		
	}
}
bool checkTruePathNeg(size_t start = UINT_MAX)			//I'm the last hope of this situation
{	static size_t backtrack=0;
	if(start == UINT_MAX) backtrack = 0;	//TODO i support backtrack limit
	backtrack++;
	//if(backtrack>100)return false;
//	PIQvalue[start]	= 1;
	
	vector<GatePin*> undoList;
	if(forward(PIQ[start],1,undoList))
	{	
#ifdef debugdel
		cout<<"checkTruePathNeg: forward(1) "<<start<<"th PIQ("<<PIQ[start]->Id()<<") success"<<endl;
		screenCheck();
#endif
		if(!checkTruePath(start+1))
		{	undo(undoList);
#ifdef debugdel
			cout<<"checkTruePathNeg: UNDO(1) "<<start<<"th PIQ("<<PIQ[start]->Id()<<")"<<endl;
			screenCheck();
#endif
			return false;}
		else return true;
	}
	else 
	{
#ifdef debugdel
		cout<<"checkTruePathNeg: forward(1) "<<start<<"th PIQ("<<PIQ[start]->Id()<<") fail"<<endl;
		screenCheck();

		cout<<"checkTruePathNeg: UNDO(1) "<<start<<"th PIQ("<<PIQ[start]->Id()<<")"<<endl;
#endif
		undo(undoList);
#ifdef debugdel
		screenCheck();
#endif
		return false;
	}

}


void printHead(ofstream& fout)
{	fout<<"Header { "<<HEADER<<" }"<<endl<<endl;
	fout<<"  BenchMark { "<<benchmark<<" }"<<endl<<endl;
}

void outfile(ofstream& fout, size_t PathNum)
{	//PI
	//cout<<"--------------------------------outfiling------------------------------------"<<endl;
	fout<<"  Path  {  "<<PathNum<<"  }"<<endl;
	fout<<"  "HEADER<<endl;
	fout<<"  {"<<endl;
	fout<<"  ---------------------------------------------------------------------------"<<endl;
	fout<<"  Pin    type                                Incr       Path delay"<<endl;
	fout<<"  ---------------------------------------------------------------------------"<<endl;
	fout<<left<<setw(45)<<"  "+longestPath[0]->Id()+" (in)"
		<<"0          0 ";
	if(longestPath[0]->getvalue()==0)fout<<"f"; else fout<<"r"; fout<<endl;
	
	size_t i=1;
	for(i =1; i<longestPath.size(); i++)
	{	CirGate* nowG = longestPath[i]->prev();	
		fout<<left<<setw(45)<<"  "+nowG->Id()+"/"+nowG->whichpin(longestPath[i-1])
			+" ("+gatename[nowG->getType()]+")"
			<<"0          "<<i-1<<" "<<((longestPath[i-1]->getvalue()==0)?"f":"r")<<endl;
		fout<<left<<setw(45)<<"  "+nowG->Id()+"/"+"Y"
			+" ("+gatename[nowG->getType()] + ")"
			<<"1          "<<i<<" "<<((longestPath[i]->getvalue()==0)?"f":"r")<<endl;	
	}
	fout<<left<<setw(45)<<"  "+longestPath[i-1]->Id()+" (out)"
		<<"0          "<<i-1<<" "<<((longestPath[i-1]->getvalue()==0)?"f":"r")<<endl;
	fout<<"  --------------------------------------------------------------------------"<<endl;
	fout<<left<<setw(40)<<"    Data Required Time "<<required_time<<endl;
	fout<<left<<setw(40)<<"    Data Arrival Time  "<<longestPath.back()->time()<<endl;
	fout<<"  --------------------------------------------------------------------------"<<endl;
	fout<<left<<setw(40)<<"    Slack"<<int(required_time)-int(longestPath.back()->time())<<endl;
	fout<<"  }"<<endl<<endl;	


	fout<<"  Input Vector"<<endl;
	fout<<"  {"<<endl;

//	for(size_t i=0; i<PIQ.size(); i++)
//		fout<<PIQ[i]->Id()<<" = "<<PIQ[i]->getvalue()<<endl;
	
	for (int i=0;i<PI_List.numBuckets();i++)
	{
    	for(int j=0;j<PI_List[i].size();j++)
    	{
        	    fout<<"    "<<PI_List[i][j].second->Id()<<"  =  ";
        	    if(PI_List[i][j].second==PIQ[0])fout<<((PIQ[0]->getvalue()==0)?"f":"r")<<endl;
        	    else fout<<PI_List[i][j].second->getvalue()<<endl;
    	}
	}
	fout<<"  }"<<endl<<endl;
}
