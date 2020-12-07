//	File: main.cpp
//	Author: Anwei

#include<iostream>
#include<string>
using namespace std;
#include"cirDef.h"

string OUTPUTFILE = "out_";

#define HEADER      "A True Path Set"

#include	"readgraph.h"
#include	"updatePIQ.h"
//#include	"traceTimeInterval.cpp"
#include	"forward.h"
#include	"longestpath.h"
#include	"Anwei.h"

int required_time = 30;//隨便給的
int   slack = 5;//隨便給的
string benchmark;
//we don't have traceTimeInterval的.h
void traceTimeInterval();

int main(int argc,char** argv)
{	

  /*//一開始將global變數都給值
  required_time = 30;//隨便給的
  slack = 5;//隨便給的
*/
//#ifndef _slk
	//cout << "[warning] _slk value not  from makefile ..." << endl;
//#else
 	//slack = _slk;
//#endif
 
  if(argc!=3){ cout<<"Makefile has wrong format ! please check again!"<<endl; return 0; }
  
  slack = atoi(argv[2]);
  string File = argv[1];
  
  cout << " read " << File << " now...... " << endl;
  
	if(!readgraph(File)) {cerr<<" readgraph fail. ";return 0;}

  cout << " read " << File << " success! " << endl;
	cout << " slk = " << slack << endl;
	cout << " time constraint = " << required_time << endl;
   

	traceTimeInterval();


	//cout<<"main: printHead start"<<endl;
	string outputfilename = OUTPUTFILE;
	outputfilename.append(benchmark); // out_casex
  string outputfilepath = ("PD_case/"+benchmark+"/output/"+outputfilename+".txt");
  cout<< outputfilepath <<endl;
	ofstream fout(outputfilepath.c_str());
	printHead(fout);
	//cout<<"main: printHead finish..."<<endl;


	size_t PathNumm = 0 ; 
	size_t FailTime=0;
  cout<<" Finding truePath...... "<<endl;
	while(longestpath())
	{	
    /*char c;
    cin>> c;*/
    //cout<<"main: longestPath found..."<<endl;
	#ifdef debugmain
	screenCheck();
	#endif
	/*	for(size_t i=0; i<longestPath.size(); i++)
			longestPath[i]->printtime();
			cout<<endl;*/
	
		for(int i=0; i<2; i++)
		{	//cout<<"main: preparing "<<PathNumm<<"-"<<FailTime<<"th("<<i<<") truepath"<<endl;		
			FailTime++;
			renew();
			//PIQvalue.clear();
			PIQ.push_back(longestPath[0]);
			//PIQvalue.push_back(i);
			
			if(!SetTruePathValue(i))
			{	
		//	#ifdef debugmain
				//cout<<"main: true path value check fail"<<endl;
		//	#endif
			continue;}
			else
			#ifdef debugmain
				//cout<<"main: true path value set..."<<endl;
			#endif
		/*	if(!timeCheck())
			{ 
			#ifdef debugmain
				cout<<"main: time check failed"<<endl;
			#endif
				continue;}

			else
			{	
			#ifdef debugmain
				cout<<"main: time checked...";
				for(size_t i=0; i<longestPath.size(); i++)
					cout<<longestPath[i]->Id();
				cout<<endl;
			#endif
			}*/
			#ifdef debugmain
			cout<<"main: checking true path..."<<endl;
			#endif
			if(checkTruePath())
			{ 
			#ifdef debugmain
				cout<<"main: truePath"<<PathNumm;
				cout<<" longestPath:";
				for(size_t i=0; i<longestPath.size(); i++)
					cout<<longestPath[i]->Id();
				cout<<endl;
				cout<<"main: fout start..."<<endl;
			#endif
				PathNumm++;
				FailTime=0;
				outfile(fout, PathNumm);
				//cout<<"find a path!!!!!!!!!!!!"<<endl;
				//screenCheck();
			}
			/*else
			{	//cout<<"----------------checking true path fault-------------------"<<endl;
				screenCheck();
				cout<<"PIQ: ";
				for(size_t i=0; i<PIQ.size(); i++)
				{	cout<<i<<":"<<PIQ[i]->Id()<<"-"<<PIQ[i]->getvalue()<<"("<<PIQ[i]->getvalue()<<"); ";}
				cout<<"longestPath: ";
				for(size_t i=0; i<longestPath.size(); i++)
				{	cout<<i<<":"<<longestPath[i]->Id()<<"-"<<longestPath[i]->getvalue();	
				}

				cout<<endl;
				
			
			
			}*/

			#ifdef debugmain
			cout<<"going out the loop"<<endl;
			#endif
		}
	}
   cout<<" Success!! Please check outputfile : "<< outputfilename <<endl;
}
