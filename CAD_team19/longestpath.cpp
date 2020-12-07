

#include "longestpath.h"

int Path_num = 0;
int PTnode_num = 0; //Prefix_tree有幾個node
int longestlength;  //最長path的arrival time
vector<Node*> Path_Q;  //每一個node代表一個path
CirGate* sourcenode = new SRC_Gate("source");
Graph Prefix_Tree("tree");


GatePin* Extract_max(vector<GatePin *> &Q){
    int num=Q.size();
    GatePin* maxPO;
    int max_id_d=0;
    int max_Qid=0;
    for(int i=0;i<num;i++){
        if(Q[i]->uppertime()>=max_id_d){
            max_id_d=Q[i]->uppertime();
            maxPO=Q[i];
            max_Qid=i;
        }
    }
    Q.erase(Q.begin()+max_Qid);
    return maxPO;
}


Node* Extract_min(vector<Node *> &Q){
    int num=Q.size();
    int min_id=0;
    int min_id_d=DIS_INF;
    int min_Qid=0;
    for(int i=0;i<num;i++){
        if(Q[i]->d<=min_id_d){
            min_id_d=Q[i]->d;
            min_id=Q[i]->id;
            min_Qid=i;
        }
    }
    Q.erase(Q.begin()+min_Qid);
    return Prefix_Tree.getNodeById(min_id);
}

void Relax(CirGate *u,CirGate *v,int weight){
    if(v->d_dij<(u->d_dij+weight)){
        v->d_dij=u->d_dij+weight;
        v->_prev_dij=u;
    }
}


//輸出path
 void Print_Path(CirGate *s,CirGate *v){
     if (!strcmp(s->Id().c_str(),v->Id().c_str())){
        int i=0;
     }
     else{
        Print_Path(s,v->_prev_dij);
        if(!strcmp(v->_prev_dij->Id().c_str(),"source")){
            Path_Pin.push_back(v->_FANIN[0]);
        }
        else{
            Path_Pin.push_back(v->_prev_dij->_FANOUT);
        }

     }

}

//由prefix_tree中的node得出path
void Track_Path(Node *v){
    Path_Pin.clear();
    if(!strcmp(v->gate->Id().c_str(),"source")){
        Path_Pin.push_back(v->pin);
    }
    else{
        Print_Path(sourcenode,v->gate);
        Path_Pin.push_back(v->gate->_FANOUT);
    }


    while(v->prev->id!=0){
        Print_Path(v->next_gate,v->prev->gate);
        Path_Pin.push_back(v->prev->gate->_FANOUT);
        v=v->prev;
    }
    if(strcmp(v->next_gate->Id().c_str(),"sink")){
        Print_Path(v->next_gate,v->prev->gate);
    }

}



void Read_Dijtime(CirGate *sourcenode,CirGate *endnode){

    vector<GatePin*> S;
    //initialize
    int node_num=AOIG_List.size()+2;
    for(int i =0;i<AOIG_List.numBuckets();i++){
             for(int j=0;j<AOIG_List[i].size();j++){
                     CirGate* gate = AOIG_List[i][j].second;
                     gate->d_dij=gate->_FANOUT->uppertime();
                     gate->_prev_dij = gate->_FANOUT->_prev_upper->_prev;
             }
     }
    sourcenode->d_dij=0;
    for (int i=0;i<PO_List.numBuckets();i++){
                for(int j=0;j<PO_List[i].size();j++){
                    S.push_back(PO_List[i][j].second);

            }
    }
    GatePin *maxPOPin = Extract_max(S);
    endnode->_prev_dij=maxPOPin->_prev;
    endnode->d_dij=maxPOPin->uppertime()+1;

}

void Struct_prefix_tree(Node* s){
    GatePin *p;
    CirGate *k;
    if(s->id==0){
       Path_Q.clear();
       int num = s->gate->_FANIN.size();
       for(int i=0;i<num;i++){
            if(strcmp(s->gate->_FANIN[i]->_prev->Id().c_str(),s->gate->_prev_dij->Id().c_str())){
                PTnode_num=PTnode_num+1;
                Prefix_Tree.addEdge(s->id,PTnode_num,0);
                Node *prefix_node = Prefix_Tree.getNodeById(PTnode_num);
                prefix_node->gate=s->gate->_FANIN[i]->_prev;
                prefix_node->prev=s;
                prefix_node->next_gate=s->gate;
                prefix_node->d =(s->gate->d_dij)-(s->gate->_FANIN[i]->_prev->d_dij)-1;
                if((required_time-longestlength+prefix_node->d)<slack){//cout<<(required_time-longestlength+prefix_node->d)<<endl;
                    Path_Q.push_back(prefix_node);
                }
            }
        }
        k=s->gate->_prev_dij;
        while(!strcmp(k->getTypeStr().c_str(),"NOT")){
              k=k->_prev_dij;
        }
    }
    else{
        k=s->gate;
        while(!strcmp(k->getTypeStr().c_str(),"NOT")){
              k=k->_prev_dij;
        }

    }
    while(k->d_dij!=0){
            PTnode_num=PTnode_num+1;

            Prefix_Tree.addEdge(s->id,PTnode_num,0);
            Node *prefix_node = Prefix_Tree.getNodeById(PTnode_num);

            if(!strcmp(k->_prev_dij->Id().c_str(),"source")){
                p=k->_FANIN[1];
            }
            else{
                p=k->getOffPin(k->_prev_dij->_FANOUT);
            }

            prefix_node->gate=p->_prev;
            prefix_node->prev=s;
            prefix_node->next_gate=k;
            prefix_node->pin=p;
            prefix_node->d =(k->d_dij)-(p->_prev->d_dij)-1;
            if(prefix_node->prev!=NULL){
                prefix_node->d = prefix_node->d +prefix_node->prev->d;
            }

            if((required_time-longestlength+prefix_node->d)<slack){//cout<<(required_time-longestlength+prefix_node->d)<<endl;
                Path_Q.push_back(prefix_node);
            }

            k=k->_prev_dij;
            while(!strcmp(k->getTypeStr().c_str(),"NOT")){
              k=k->_prev_dij;
            }

    }
}


bool longestpath(){
    Path_Pin.clear();

    if (Path_num==0){
        Path_num=1;
        //initialize
        Prefix_Tree.nodes.clear();
        Prefix_Tree.edges.clear();
        PTnode_num = 0;

        //set source &sink
        //CirGate sourcenode("source");
        //sourcenode->_FANOUT_PI.clear();
        for (int i=0;i<PI_List.numBuckets();i++){
                for(int j=0;j<PI_List[i].size();j++){
                    sourcenode->FANOUTpush_back(PI_List[i][j].second);
                    PI_List[i][j].second->_prev=sourcenode;
                }
        }

        CirGate* endnode =new SINK_Gate("sink");
        //endnode->_FANIN.clear();
        for (int i=0;i<PO_List.numBuckets();i++){
                for(int j=0;j<PO_List[i].size();j++){
                    PO_List[i][j].second->_next.push_back(endnode);
                    endnode->_FANIN.push_back(PO_List[i][j].second);
                }
        }

        //
        Read_Dijtime(sourcenode,endnode);
        longestlength=endnode->d_dij-1;//cout<<(required_time-longestlength)<<endl;
        if((required_time-longestlength)<slack){
            Print_Path(sourcenode,endnode);
            Node *a;
            map<int, Node *>::iterator it = Prefix_Tree.nodes.find(0);
            if ( it != Prefix_Tree.nodes.end() )
            	a = (*it).second;
            else{
                a = new Node(0);
            	Prefix_Tree.nodes[0] = a;
            }
            Prefix_Tree.nodes[0]->gate = endnode;
            Prefix_Tree.nodes[0]->d = 0;
            Prefix_Tree.nodes[0]->prev = NULL;
            Struct_prefix_tree(Prefix_Tree.getNodeById(0));
            PIQ.push_back(Path_Pin[0]);
            return true;
        }
        else return false;

    }
    else {
        if(Path_Q.empty())return false;
        else{
            Path_num=Path_num+1;
            Node *PTnode = Extract_min(Path_Q);
            Track_Path(PTnode);
            Struct_prefix_tree(PTnode);
            PIQ.push_back(Path_Pin[0]);
            return true;
        }
    }
}



