#include <sstream>

class BusSystem
{   
    public: 
        class Node;
    protected:
        Node **Pointer;
        int size;
        int count;
    public:
        BusSystem()
        {
           this->count=0;
            this->size=0;
            this->Pointer=new Node *[1];
            this->Pointer[0]=NULL;
        }
        BusSystem(int size)
        {
           this->count=0;
            this->size=size;
            this->Pointer=new Node *[1];
            this->Pointer[0]=NULL;
        }
        ~BusSystem()
        {
            this->count=0;
            this->size=0;
            this->Pointer=new Node *[1];
            this->Pointer[0]=NULL;
        }
        void updateSize(int size)
        {
            this->size=size;
        }
    public:
        class Node
        {
        public:
            string CODE; // mã số tuyến
            string LP; // biển số xe
            short CASE; //: một giá trị nhị phân, nếu 0 tức là tuyến xe đi từ bến đầu đến bến cuối (chiều đi),
                        // ngược lại tức là chuyến xe đi theo chiều ngược lại (chiều về).
            int TIME_A; // thời gian bắt đầu đi
            int TIME_B; // thời gian đến bến
            Node *next;
            Node *prev;
           
            friend class BusSystem;

        public:
            
            Node()
            {
                this->CODE="";
                this->LP="";
                this->CASE=-1;
                this->TIME_A=0;
                this->TIME_B=0;
                this->next = NULL;
                this->prev=NULL;
                
            }
            Node(string &CODE, string &LP, short &CASE, int &TIME_A, int &TIME_B, Node *next,Node *prev)
            {
                this->CODE=CODE;
                this->LP=LP;
                this->CASE=CASE;
                this->TIME_A=TIME_A;
                this->TIME_B=TIME_B;
                this->next = next;
                this->prev=prev;
               
            }
            ~Node()
            {
                this->CODE="";
                this->LP="";
                this->CASE=-1;
                this->TIME_A=0;
                this->TIME_B=0;
                this->next = NULL;
                this->prev=NULL;
                
            }
        };
        
        ////
        string INS(int local, string instruction)
        {
            string CODE="";
            string LP="";
            short CASE=-1;
            string info="";
            int TIME_A=0;
            int TIME_B=0;
            int stt=0;
            
            for(unsigned int i=local;i<instruction.length()+1;i++)
            { 
                
                if(instruction[i]==' '||i==instruction.length())
                {
                    switch (stt)
                    {
                    case 0:
                        CODE=info;
                        stt++;
                        info="";
                        continue;
                    case 1:
                        LP=info;
                        stt++;
                        info="";
                        continue;
                    case 2:
                        if(info=="1") 
                        {
                            CASE=1;
                            stt++;
                            
                        }
                        else if(info=="0") 
                        {
                            CASE=0;
                            stt++;
                            
                        }

                        else
                        {
                            for(unsigned int k=0;k<info.length();k++)
                            {
                                TIME_A=TIME_A*10 +info[k]-'0';
                            }
                            stt=stt+2;
                            
                            
                        }
                        info="";
                        continue;
                    case 3:
                        for(unsigned int k=0;k<info.length();k++)
                            {
                                TIME_A=TIME_A*10 +info[k]-'0';
                            }
                            stt=stt+2;
                            info="";
                        continue;
                    case 4:
                         for(unsigned int i=0;i<info.length();i++)
                        {
                            TIME_B=TIME_B*10 +info[i]-'0';
                        }
                        stt++;
                        info="";
                        continue;
                    default:
                        return "-1";
                        break;
                    }
                }
                info+=instruction[i];
               
            }
           
            if(this->count==0)
            {
                Node* p=new Node(CODE,LP,CASE,TIME_A,TIME_B,NULL,NULL); 
                this->Pointer[0]= p;
                this->count++;
                return "1";
            }
            else
            {
                for(int i=0;i<this->count;i++)
                {
                    Node* ptr=this->Pointer[i];
                    while(ptr->next!=NULL)
                    {
                        if(ptr->LP==LP)
                        {
                            if(ptr->CODE!=CODE) return "-1";
                        }
                        ptr=ptr->next;
                    }
                    delete ptr;
                }
            
                for(int i=0;i<this->count;i++)
                {
                    if(this->Pointer[i]->CODE==CODE)
                    {
                        Node* ptr=this->Pointer[i];
                        int sochuyen=0;
                        while(ptr->next!=NULL)
                        {
                            if(ptr->LP==LP)
                            {
                                if(TIME_A<=ptr->TIME_A) return "-1";
                            }
                            ptr=ptr->next;
                            sochuyen++;
                        }
                        ptr->next=new Node(CODE,LP,CASE,TIME_A,TIME_B,NULL,NULL); 
                        delete ptr;
                        stringstream convert;
                        convert << sochuyen;
                        return convert.str();
                    }
                
                }
                this->Pointer[this->count]=new Node(CODE,LP,CASE,TIME_A,TIME_B,NULL,NULL);
                this->count++;
                return "1";
            }

        }
        string DEL(int local,string instruction)
        {
            if(this->count==0) return "-1";
            string info="";
            int stt=1;
            string CODE="";
            int TIME_A=0;
           int TIME_B=0;
            int choose=0;// 0: A<TIME<B; 1: TIME==A; 2: ALL TIME
            for(unsigned int i=local;i<=instruction.length();i++)
            {
                if(instruction[i]==' '||i==instruction.length())
                {
                    switch (stt)
                    {
                    case 1:
                        CODE=info;
                        info="";
                        stt++;
                        continue;
                    case 2:
                        if(info=="")
                        {
                            choose=2;
                            
                        }
                        else
                        {
                            for(unsigned int k=0;k<info.length();k++)
                            {
                                TIME_A=TIME_A*10 +info[k]-'0';
                            }
                            info="";
                            stt++;
                        }
                        continue;
                    case 3:
                        if(info=="")
                        {
                            choose=1;
                        }
                        else
                        {
                            for(unsigned int k=0;k<info.length();k++)
                            {
                                TIME_B=TIME_B*10 +info[k]-'0';
                            };
                            
                        }
                        stt++;
                        continue;
                        
                    default:
                        break;
                    }
                }
                info+=instruction[i];
            }
            int dem=0;
            for(int k=0;k<this->count;k++)
            {   
                
                if(this->Pointer[k]->CODE==CODE)
                {
                   
                    if(choose==0)
                    {
                        Node* p=this->Pointer[k];
                        
                        while(p->next!=NULL)
                        {
                            if(p->TIME_A>=TIME_A&&p->TIME_A<=TIME_B)
                            {
                                p->prev->next=p->next;
                                p->next->prev=p->prev;
                                dem++;
                            }
                            p=p->next;
                        }
                        while(p->prev!=NULL)
                        {
                            p=p->prev;
                        }
                        this->Pointer[k]=p;
                        delete p;
                    }
                    else if(choose==1)
                    {
                        Node* p=this->Pointer[k];
                        while(p->next!=NULL)
                        {
                            if(p->TIME_A==TIME_A)
                            {
                                p->prev->next=p->next;
                                p->next->prev=p->prev;
                                dem++;
                            }
                            p=p->next;
                        }
                        while(p->prev!=NULL)
                        {
                            p=p->prev;
                        }
                        this->Pointer[k]=p;
                        delete p;
                    }
                    else
                    {
                        Node* p=this->Pointer[k];
                        while(p->next!=NULL)
                        {
                           
                            dem++;
                            p=p->next;
                        }
                        delete[] this->Pointer[k];
                        delete p;
                        this->count--;
                      
                    }
                continue;   
                }
            }
            stringstream convert;
                    convert << dem;
                    
                    return convert.str();
        }
        string SQ(int local, string instruction)
        {
            int n=0;
            for(unsigned int i=local;i<instruction.length();i++)
            {
                if(instruction[i]>='0'||instruction[i]<='9') 
                {
                    int m=instruction[i]-'0';
                    n=n*10+m;
                    
                }
                else return "-1";
            }
            if(n!=0) updateSize(n);
            return "1";
        }
        string CS(int local, string instruction)
        {
            int stt=1;
            string CODE="";
            int TIME=0;
            string info="";
            short CASE=-1;
            for(unsigned int i=local;i<=instruction.length();i++)
            {
                if(instruction[i]==' '||i==instruction.length())
                {
                    switch (stt)
                    {
                    case 1:
                        CODE=info;
                        info="";
                        stt++;
                        continue;
                    case 2:
                       
                            for(unsigned int k=0;k<info.length();k++)
                            {
                                TIME=TIME*10 +info[k]-'0';
                            }
                            info="";
                            stt++;
                        
                        continue;
                    case 3:
                        if(info!="")
                        {
                            CASE=info[0]-'0';
                        }
                        
                        stt++;
                        continue;
                        
                    default:

                        break;
                    }
                }
                info+=instruction[i];
            }
            int dem=0;
            for(int i=0;i<this->count;i++)
            {
                if(this->Pointer[i]->CODE==CODE)
                {
                    Node* p=this->Pointer[i];
                    while(p!=NULL)
                    {
                        if(CASE==0)
                        {
                            if(p->CASE==0&&p->TIME_A<TIME&&p->TIME_B>TIME)
                            {
                                dem++;
                            }
                            
                        }
                        else if(CASE==1)
                        {
                            if(p->CASE==1&&p->TIME_A<TIME&&p->TIME_B>TIME)
                        {
                            dem++;
                        }
                        }   
                        else
                        {
                            if(p->TIME_A<TIME&&p->TIME_B>TIME)
                            {
                                dem++;
                            }
                        }
                        p=p->next;
                    }

                 continue;        
                }
                 
            }
            string answer="";
            ostringstream ss;
            
            ss << dem;
            answer = ss.str();
         
            return answer; 
        }
        string CE(int local, string instruction)
        {
            int stt=1;
            string CODE="";
            int TIME=0;
            string info="";
            short CASE=-1;
            for(unsigned int i=local;i<=instruction.length();i++)
            {
                if(instruction[i]==' '||i==instruction.length())
                {
                    switch (stt)
                    {
                    case 1:
                        CODE=info;
                        info="";
                        stt++;
                        continue;
                    case 2:
                       
                            for(unsigned int k=0;k<info.length();k++)
                            {
                                TIME=TIME*10 +info[k]-'0';
                            }
                            info="";
                            stt++;
                        
                        continue;
                    case 3:
                        if(info!="")
                        {
                            CASE=info[0]-'0';
                        }
                        
                        stt++;
                        continue;
                        
                    default:

                        break;
                    }
                }
                info+=instruction[i];
            }
            int dem=0;
            for(int i=0;i<this->count;i++)
            {
                if(this->Pointer[i]->CODE==CODE)
                {
                    Node* p=this->Pointer[i];
                    while(p!=NULL)
                    {
                        if(CASE==0)
                        {
                            if(p->CASE==0&&p->TIME_B<TIME)
                            {
                                dem++;
                            }
                        }
                        else if(CASE==1)
                        {
                            if(p->CASE==1&&p->TIME_B<TIME)
                            {
                                dem++;
                            }
                        }   
                        else
                        {
                            if(p->TIME_B<TIME)
                            dem++;
                        }
                        p=p->next;
                    }
                  continue;  
                   
                }
            }
             ostringstream convert;
                    convert << dem;
                    return convert.str(); 
        }
        string GS(int local, string instruction)
        {
             int stt=1;
            string CODE="";
            int TIME=0;
            string info="";
            short CASE=-1;
            for(unsigned int i=local;i<=instruction.length();i++)
            {
                if(instruction[i]==' '||i==instruction.length())
                {
                    switch (stt)
                    {
                    case 1:
                        CODE=info;
                        info="";
                        stt++;
                        continue;
                    case 2:
                       
                            for(unsigned int k=0;k<info.length();k++)
                            {
                                TIME=TIME*10 +info[k]-'0';
                            }
                            info="";
                            stt++;
                        
                        continue;
                    case 3:
                        if(info!="")
                        {
                            CASE=info[0]-'0';
                        }
                        
                        stt++;
                        continue;
                        
                    default:

                        break;
                    }
                }
                info+=instruction[i];
            }
            int vitri=0;
            int index=0;
            for(int i=0;i<this->count;i++)
            {
                if(this->Pointer[i]->CODE==CODE)
                {
                    Node* p=this->Pointer[i];
                     int Nearest=-1;
                    if(CASE==0)
                    {
                        while(p!=NULL)
                        {
                            if(p->CASE==0)
                            {
                                if(Nearest>(p->TIME_A-TIME))
                                {
                                    Nearest=p->TIME_A-TIME;
                                    index=vitri;
                                }
                            }
                            vitri++;
                            p=p->next;
                        }


                    }
                    else if(CASE==1)
                    {
                        while(p!=NULL)
                        {
                            if(p->CASE==1)
                            {
                                if(Nearest>(p->TIME_A-TIME))
                                {
                                    Nearest=p->TIME_A-TIME;
                                    index=vitri;
                                }
                            }
                            vitri++;
                            p=p->next;
                        }
                    }
                    else
                    {
                        while (p!=NULL)
                        {
                            if(Nearest>=(p->TIME_A-TIME)&&p->CASE==0)
                            {
                                Nearest=p->TIME_A-TIME;
                                index=vitri;
                            }
                            vitri++;
                            p=p->next;
                        }
                        
                    }
                    delete p;
                    Node* ptr=this->Pointer[i];
                    int k=0;
                    while(k!=index)
                    {
                        ptr=ptr->next;
                        k++;
                    }
                    return ptr->LP;
                    
                    
                }

            }
            return "-1";
        }
        string GE(int local, string instruction)
        {
            int stt=1;
            string CODE="";
            int TIME=0;
            string info="";
            short CASE=-1;
            for(unsigned int i=local;i<=instruction.length();i++)
            {
                if(instruction[i]==' '||i==instruction.length())
                {
                    switch (stt)
                    {
                    case 1:
                        CODE=info;
                        info="";
                        stt++;
                        continue;
                    case 2:
                       
                            for(unsigned int k=0;k<info.length();k++)
                            {
                                TIME=TIME*10 +info[k]-'0';
                            }
                            info="";
                            stt++;
                        
                        continue;
                    case 3:
                        if(info!="")
                        {
                            CASE=info[0]-'0';
                        }
                        
                        stt++;
                        continue;
                        
                    default:

                        break;
                    }
                }
                info+=instruction[i];
            }
            int vitri=0;
            int index=0;
            for(int i=0;i<this->count;i++)
            {
                if(this->Pointer[i]->CODE==CODE)
                {
                    Node* p=this->Pointer[i];
                     int Nearest=-1;
                    if(CASE==0)
                    {
                         while(p!=NULL)
                        {
                            if(p->CASE==0)
                            {
                                if(Nearest>(TIME-p->TIME_B))
                                {
                                    Nearest=TIME-p->TIME_B;
                                    index=vitri;
                                }
                            }
                            vitri++;
                            p=p->next;
                        }

                    }
                    else if(CASE==1)
                    {
                        while(p!=NULL)
                        {
                            if(p->CASE==1)
                            {
                                if(Nearest>(TIME-p->TIME_B))
                                {
                                    Nearest=TIME-p->TIME_B;
                                    index=vitri;
                                }
                            }
                            vitri++;
                            p=p->next;
                        }
                    }
                    else
                    {
                        while (p!=NULL)
                        {
                            if(Nearest>(TIME-p->TIME_B)&&p->CASE==0)
                                {
                                    Nearest=TIME-p->TIME_B;
                                    index=vitri;
                                }
                            vitri++;
                            p=p->next;
                        }
                        
                    }
                    delete p;
                    Node* ptr=this->Pointer[i];
                    int k=0;
                    while(k!=index)
                    {
                        ptr=ptr->next;
                        k++;
                    }
                    return ptr->LP;
                    
                    
                }

            }
            return "-1";
        }
        string query(string instruction)
        {
            int local=0;
            string CODE="";
            for(unsigned int i=0;i<instruction.length();i++)
            {
                if(instruction[i]!=' ')
                {
                    CODE+=instruction[i];
                }
                else
                {
                    local=i; 
                    break;
                }
                
            }
            string answer="-1";
            if(CODE=="SQ") answer=this->SQ(local+1,instruction);
            else if(CODE=="INS") answer=this->INS(local+1,instruction);
            else if(CODE=="DEL") answer=this->DEL(local+1,instruction);
            else if(CODE=="CS") answer=this->CS(local+1,instruction);
            else if(CODE=="CE") answer=this->CE(local+1,instruction);
            else if(CODE=="GS") answer=this->GS(local+1,instruction);
            else if(CODE=="GE") answer=this->GE(local+1,instruction);
            return answer;
        }

    
};
