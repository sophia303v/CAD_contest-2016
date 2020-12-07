/****************************************************************************
  FileName     [ forward.cpp ]
  PackageName  [ STA ]
  Synopsis     [ Forward check whether there is conflict ]
  Author       [ Hsiang-Ya CHAO ]
  identity     [ Student of NTUEE, Taiwan ]
  date         [ 2016 , 06 , 09] (last update)
****************************************************************************/
#include <iostream>
#include <string>
#include <vector>
#include "forward.h"

using namespace std;

//PrevValue::PrevValue(GatePin* pin)
//{
//	changed_pin = pin;
//	prev_logic = 5;
//	prev_time = -1;
//}


//forward
bool forward(GatePin* pin , unsigned value , vector<GatePin* >  & record){

    vector<CirGate*> foGate;    //all fanout gate
    bool temp = true;           //�O�����S�������Lfalse�A�����̫ܳ�n�^��false

    //cout << "forwarding   " << endl;
    if(pin -> gettype() == "PI")
    {
        //PrevValue* prevvalue = new PrevValue(pin);       //�O��prev time�Mlogic
        //prevvalue -> prev_logic = pin -> getvalue();
        //prevvalue -> prev_time = pin -> time();
        record.push_back(pin);
        pin -> settime(0);
    }

    pin -> setvalue(value);
    pin -> settraveled();
    //cout << "input pin: " << pin -> Id() << " input pin time: " << pin -> time() << " " << "input pin logic: " << pin -> getvalue() << endl;

    foGate = pin -> _next;
    //cout << "fanout number: " << foGate.size() << endl;
    if(pin -> gettype() != "PO")
    {
    for(int i = 0; i < foGate.size(); i++)
    {

        // output not traveled
        if(!(foGate[i] -> _FANOUT -> istraveled()))
        {
        //cout << "Gate checking: " ;
        foGate[i] -> printGate();
        if(foGate[i] -> getTypeStr() == "NOT")
        {
            //if(foGate[i] -> _FANOUT -> getvalue() == pin -> getvalue() && foGate[i] -> _FANOUT -> istraveled())  return false;    //not gate logic conflict
            //else
            //{

                record.push_back(foGate[i] -> _FANOUT);

                foGate[i] -> _FANOUT -> settime(pin -> time()+1);
                if(!forward(foGate[i] -> _FANOUT , !(pin -> getvalue()), record))
                {
                    temp = false;
                }
            //}

        }

        //propogate time
        else if(foGate[i] -> getOffPin(pin) -> istraveled())         // gate�����input������
        {
            //cout << "offpin time: " << foGate[i] -> getOffPin(pin) -> time() << " offpin logic: " << foGate[i] -> getOffPin(pin) -> getvalue() << endl;

            if(pin -> getvalue() == foGate[i] -> getcvalue() && foGate[i] -> getOffPin(pin) -> getvalue() == foGate[i] -> getcvalue())
            {                                                                                                     //���input���Ocontrolling value
                //if(pin -> time() < foGate[i] -> getOffPin(pin) -> time() && foGate[i] -> getOffPin(pin) -> istruepath() && !(pin -> istruepath()))
                //{
                //    return false;   // time conflict
                //}
                //else if(pin -> time() > foGate[i] -> getOffPin(pin) -> time() && pin -> istruepath())  return false;
                if(pin -> time() <= foGate[i] -> getOffPin(pin) -> time())     //������쪺�M�w�ɶ�
                {

                    record.push_back(foGate[i] -> _FANOUT);

                    foGate[i] -> _FANOUT -> settime(pin -> time()+1);

                }
                else
                {

                    record.push_back(foGate[i] -> _FANOUT);

                    foGate[i] -> _FANOUT -> settime(foGate[i] -> getOffPin(pin) -> time()+1);

                }

            }

            else if(pin -> getvalue() != foGate[i] -> getcvalue() && foGate[i] -> getOffPin(pin) -> getvalue() != foGate[i] -> getcvalue())  //���noncontrol
            {
                //if(pin -> time() > foGate[i] -> getOffPin(pin) -> time() && foGate[i] -> getOffPin(pin) -> istruepath() && !(pin -> istruepath()))
                //{
                //    return false;   // time conflict
                //}
                //else if(pin -> time() < foGate[i] -> getOffPin(pin) -> time() && pin -> istruepath())  return false;
                if(pin -> time() >= foGate[i] -> getOffPin(pin) -> time())    //�ߨ쪺�M�w�ɶ�
                {

                    record.push_back(foGate[i] -> _FANOUT);

                    //cout << " both non control " << endl;
                    foGate[i] -> _FANOUT -> settime(pin -> time()+1);
                }
                else
                {

                    record.push_back(foGate[i] -> _FANOUT);

                    foGate[i] -> _FANOUT -> settime(foGate[i] -> getOffPin(pin) -> time()+1);

                }
            }

            //else if(pin -> getvalue() == foGate[i] -> getcvalue() && foGate[i] -> getOffPin(pin) -> istruepath() && !(pin -> istruepath()))  return false;
            //else if(pin -> getvalue() != foGate[i] -> getcvalue() && pin -> istruepath())  return false;
            else if(pin -> getvalue() == foGate[i] -> getcvalue())  // pin�Ocontrolling value
            {

                 record.push_back(foGate[i] -> _FANOUT);

                 foGate[i] -> _FANOUT -> settime(pin -> time()+1);

            }
            else if(foGate[i] -> getOffPin(pin) -> getvalue() == foGate[i] -> getcvalue())  // offpin�Ocontrolling value
            {

                 record.push_back(foGate[i] -> _FANOUT);

                 foGate[i] -> _FANOUT -> settime(foGate[i] -> getOffPin(pin) -> time()+1);

            }


            //propogate logic
            if(pin -> getvalue() == foGate[i] -> getcvalue() || foGate[i] -> getOffPin(pin) -> getvalue() == foGate[i]-> getcvalue())
            {
                //if(foGate[i] -> _FANOUT -> getvalue() == foGate[i] -> getcvalue() && foGate[i] -> _FANOUT -> istraveled())
                // �䤤�@��input�Ocontrol output�o�Ocontrol
                //{
                //    return false;
                //}
                //else if( !(foGate[i] -> _FANOUT -> istraveled()))
                //{

                    if(!forward(foGate[i] -> _FANOUT, !(foGate[i] -> getcvalue()),record ))
                    {
                        temp = false;
                    }
                //}
            }

            else if(pin -> getvalue() != foGate[i] -> getcvalue() && foGate[i] -> getOffPin(pin) -> getvalue() != foGate[i]-> getcvalue())
            {
                //cout << "fanout " << foGate[i] -> _FANOUT -> Id() << endl;
                //cout << "istraveled" << foGate[i] -> _FANOUT -> istraveled() << endl;
                //if(foGate[i] -> _FANOUT -> getvalue() != foGate[i] -> getcvalue() && foGate[i] -> _FANOUT -> istraveled())
                //{                                                                  // ���input�Ononcontrol output�o�Ononcontrol
                //    return false;
                //}
                //else if( !(foGate[i] -> _FANOUT -> istraveled()))
                //{
                    //cout << "test2 both non control" << endl;
                    if(!forward(foGate[i] -> _FANOUT, foGate[i] -> getcvalue(), record))
                    {
                        temp = false;
                    }
                //}
            }


        }

        //�u��pin���ȦӥB�Ocontrol value�Acheck logic
        //else if(pin -> getvalue() == foGate[i] -> getcvalue())
        //{
        //    if(foGate[i] -> _FANOUT -> getvalue() != foGate[i] -> getcvalue() && foGate[i] -> _FANOUT -> istraveled())    return false;
        //}

        if(temp == false)  return false;
        }


    // output istraveled
    else if(foGate[i] -> _FANOUT -> istraveled())
    {
        //cout << "Gate checking: " ;
        foGate[i] -> printGate();

        if(foGate[i] -> getTypeStr() == "NOT")
        {
            if(foGate[i] -> _FANOUT -> getvalue() == pin -> getvalue() && foGate[i] -> _FANOUT -> istraveled())  return false;    //not gate logic conflict
        }

        // check time conflict
        else if(foGate[i] -> getOffPin(pin) -> istraveled())         // gate�����input������
        {
            //cout << "offpin time: " << foGate[i] -> getOffPin(pin) -> time() << " offpin logic: " << foGate[i] -> getOffPin(pin) -> getvalue() << endl;

            if(pin -> getvalue() == foGate[i] -> getcvalue() && foGate[i] -> getOffPin(pin) -> getvalue() == foGate[i] -> getcvalue())
            {                                                                                                     //���input���Ocontrolling value
                if(pin -> time() < foGate[i] -> getOffPin(pin) -> time() && foGate[i] -> _FANOUT -> time() == foGate[i] -> getOffPin(pin) -> time()+1)
                {
                    return false;   // time conflict
                }
            }

            else if(pin -> getvalue() != foGate[i] -> getcvalue() && foGate[i] -> getOffPin(pin) -> getvalue() != foGate[i] -> getcvalue())  //���noncontrol
            {
                if(pin -> time() > foGate[i] -> getOffPin(pin) -> time() && foGate[i] -> _FANOUT -> time() ==  foGate[i] -> getOffPin(pin) -> time()+1)
                {
                    return false;   // time conflict
                }
                //else if(pin -> time() < foGate[i] -> getOffPin(pin) -> time() && pin -> istruepath())  return false;
            }

            else if(pin -> getvalue() == foGate[i] -> getcvalue() && foGate[i] -> _FANOUT -> time() != pin -> time()+1)  return false;
            else if(pin -> getvalue() != foGate[i] -> getcvalue() && foGate[i] -> _FANOUT -> time() != foGate[i] -> getOffPin(pin) -> time()+1)  return false;

            // check logic conflict
            if(pin -> getvalue() == foGate[i] -> getcvalue() || foGate[i] -> getOffPin(pin) -> getvalue() == foGate[i]-> getcvalue())
            {
                if(foGate[i] -> _FANOUT -> getvalue() == foGate[i] -> getcvalue())
                // �䤤�@��input�Ocontrol output�o�Ocontrol
                {
                    return false;
                }
            }
            else if(pin -> getvalue() != foGate[i] -> getcvalue() && foGate[i] -> getOffPin(pin) -> getvalue() != foGate[i]-> getcvalue())
            {
                //cout << "fanout " << foGate[i] -> _FANOUT -> Id() << endl;
                //cout << "istraveled" << foGate[i] -> _FANOUT -> istraveled() << endl;
                if(foGate[i] -> _FANOUT -> getvalue() != foGate[i] -> getcvalue())
                {                                                                  // ���input�Ononcontrol output�o�Ononcontrol
                    return false;
                }
            }
        }
        else if(pin -> getvalue() == foGate[i] -> getcvalue())
        {
            if(foGate[i] -> _FANOUT -> getvalue() == foGate[i] -> getcvalue())    return false;
        }

    }



    }
    }


    return true;


}



//undo
void undo(vector<GatePin* > record)
{
    for(int i = 0; i < record.size(); i++ )
    {
        //if(record[i] -> prev_time != -1)
        //    record[i] -> changed_pin -> settime(record[i] -> prev_time);
        //if(record[i] -> prev_logic != 5 )
        //    record[i] -> changed_pin -> setvalue(record[i] -> prev_logic);
        record[i] -> setuntraveled();
    }

}
