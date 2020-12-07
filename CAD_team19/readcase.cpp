//	Filename : readcase.cpp
//	Author : Anwei
// line 54.55.80.100 modified byS
#define READCASE_C 
#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<sstream>
#include"readcase.h"

gatetype nowgate=TOT;
size_t poss=0,nposs=0;
string line;
string gatename[3] = { "NOT1", "NAND2", "NOR2" };
string gatearg[3][3]= {{"Y","A"},{"Y","A","B"},{"Y","A","B"}}; 
size_t gatenum[3] = {2,3,3};
vector<string>* readPorts[3]= {&inputNames, &outputNames, &wireNames};
string nowname,nowarg[3];
vector<string> inputNames, outputNames, wireNames;


bool getfirstline()
{	
	string temp;
//to module
	while(1)
	{	getline(fp,line);
		if(fp.eof())return false;
		getfirst(temp);
		if(temp=="module")	
		{	getfirst(benchmark, nposs);
			break;
		}
	}

//get vectors
	readtype linetype;
	size_t fanhao = 0,square = 0;
	bool conti = false;
	
	fanhao = line.find_first_of(";");
	while(fanhao==line.npos)
	{	getline(fp,line);
		if(fp.eof())return false;
		fanhao = line.find_first_of(";");
	}


	while(1)
	{	//cout<<"loop"<<endl;
     int changeline = line.find_last_of(char(13));//modified byS
	  if((fanhao+1)==line.size()||conti||(fanhao+1)==changeline)//modified byS
		{	//cout<<"getline";
			getline(fp,line);
			if(fp.eof())return false;
		}
		else
		{	//cout<<"fanhao"<<fanhao<<"linesize"<<line.size()<<endl;
			line = line.substr(fanhao+1);
		}
		nposs = 0;

		if(!conti)
		{	getfirst(temp);
			if(temp == "input")
				linetype = INPUT;
			else if(temp =="output")
				linetype = OUTPUT;
			else if(temp =="wire")
				linetype = WIRE;
			else
				break;
		}
			
		while(getfirst(temp,nposs))
		{
     if(temp.find_last_of(char(13))==temp.npos){//modified by S
    
         if(temp[0]=='[')
			   {	
				   int mark = (int)line.find_first_of(':',poss);	
			   	 string str = line.substr(poss+1, mark-poss-1);
				   stringstream a;
			   	 a<<str;
				   a>>mark;
				   size_t liter = line.find_first_of("]",poss);
				   //liter = line.find_first_not_of(" ",liter);
		    	//temp = line.substr(liter+1, nposs-liter-1);
		    	getfirst(temp, liter+1);
				  for(;mark>=0;mark--)
				  {	stringstream b;
				  	b<<mark;
				  	b>>str;
				  	readPorts[linetype]->push_back(temp+"["+str+"]");
				  }
		  	}
			  else
				readPorts[linetype]->push_back(temp);
		   }//modified by S
		}

		fanhao = line.find_first_of(";");
		if(fanhao==line.npos)conti = true;
		else conti = false;
	}


//to gate line
	while(1)
	{	if(gettype()) return true;
		getline(fp,line);
		if(fp.eof())return false;	
	}


}
bool readline()
{	if(!gettype()) return false;
	if(!getcont()) throw "missing content!\n";
	getnowname();
	getline(fp,line);
  	return true;
}
 

bool getfirst(string& first, size_t start)//start = 0
{	if(line.empty())return false;
	poss = line.find_first_not_of(",; ", start);
	if(poss == line.npos) return false;
	nposs = line.find_first_of(",; ", poss);
	first = line.substr(poss, nposs-poss);	
	return true;
}

bool getnowname()
{	getfirst(nowname, nposs);}

bool gettype()
{	string first;
	if(!getfirst(first))
		{ nowgate = TOT;return false;}
	for(size_t i =0 ; i < 3; i++)
		if(first==gatename[i]) 
		{	nowgate=gatetype(i); return true;}
	nowgate = TOT; return false; 
}

bool getcont()
{	poss = 0;
	size_t nnposs = 0;
	for(size_t i = 0 ; i< gatenum[nowgate]; i++)
	{	poss = line.find("."+gatearg[nowgate][i],nposs)+3;//
		nnposs = line.find_first_of(")", poss);
		nowarg[i] = line.substr(poss, nnposs-poss);
	}
	return true;
}





