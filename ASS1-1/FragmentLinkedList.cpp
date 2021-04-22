#include <sstream>
#include <string>
#include <iostream>
#include <type_traits>
using namespace std;

#ifndef ILIST_H
#define ILIST_H

template <class T>
class IList
{
public:
    virtual void add(const T& element) = 0;
    virtual void add(int index, const T& element) = 0;
    virtual T removeAt(int index) = 0;
    virtual bool removeItem(const T& item) = 0;
    virtual bool empty() = 0;
    virtual int size() = 0;
    virtual void clear() = 0;
    virtual T get(int index) = 0;
    virtual void set(int index, const T& element) = 0;
    virtual int indexOf(const T& item) = 0;
    virtual bool contains(const T& item) = 0;
    virtual string toString() = 0;
};

#endif

template <class T>
class FragmentLinkedList : IList<T>
{
public:
    class Node;
    class Iterator;

protected:
    Node **fragmentPointers;
    int fragmentMaxSize;
    int count;

public:
    FragmentLinkedList(int fragmentMaxSize = 5)
    {
        this->count = 0;
        this->fragmentMaxSize = fragmentMaxSize;
        this->fragmentPointers = new Node *[2];
        this->fragmentPointers[0] = NULL;
        this->fragmentPointers[1] = NULL;
    }
    virtual ~FragmentLinkedList()
    {
        this->clear();
    }
    virtual void add(const T& element);
    virtual void add(int index, const T& element);
    virtual T removeAt(int index);
    virtual bool removeItem(const T& item);
    virtual bool empty();
    virtual int size();
    virtual void clear();
    virtual T get(int index);
    virtual void set(int index, const T& element);
    virtual int indexOf(const T& item);
    virtual bool contains(const T& item);
    virtual string toString();
    Iterator begin(int index = 0)
    {
        Iterator a(this,true);
        a.pNode=this->fragmentPointers[index];
        return  a;
    }
    Iterator end(int index = -1)
    {
        Iterator a(this,true);
        if(index==-1)
        {
            a.pNode=NULL;
        }
        else
        {
            a.pNode=this->fragmentPointers[index+1];
        }
        ;
        return  a;
    }

public:
    class Node
    {
    private:
        T data;
        Node *next;
        Node *prev;
        friend class FragmentLinkedList<T>;

    public:
        Node()
        {
            next = 0;
            prev = 0;
        }
        Node(Node *next, Node *prev)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node *next, Node *prev)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    class Iterator
    {
    public:
        FragmentLinkedList<T> *pList;
        Node *pNode;
        int index;

    public:
        Iterator(FragmentLinkedList<T> *pList = 0, bool begin = true)
        {
            this->pList=pList;
            if(begin==true) 
            {
                pNode=pList->fragmentPointers[0]; 
                this->index=0;
            }
            else            
            {
                pNode=NULL;
                this->index=this->pList->size();
            }  
        }
        Iterator(int fragmentIndex, FragmentLinkedList<T> *pList = 0,  bool begin = true)
        {
            this->pList=pList;
           
            if(begin==true)
            {
                this->pNode=this->pList->fragmentPointers[fragmentIndex];
            }
            else
            {
    
                this->pNode=this->pList->fragmentPointers[fragmentIndex+1];
            }
        }
        Iterator &operator=(const Iterator &iterator)
        {
            this->pNode=iterator.pNode;
            return *this;
        }
        T &operator*()
        {
            if(this->pNode==NULL) throw std::out_of_range("Segmentation fault!");
            return pNode->data;
        }
        bool operator!=(const Iterator &iterator)
        {
            if(pNode!=iterator.pNode||index!=iterator.index)
            {
                return true;
            }
            return false;
        }
        void remove()
        {
            if(this->pNode==this->pList->fragmentPointers[0])
            {
                this->pList->removeAt(0);
                this->pNode=NULL;
                this->index=-1;
            }
            else
            {    
                Node *ptr=this->pList->fragmentPointers[0];
                int i=0;
                while(ptr->data!=this->pNode->data){
                    ptr=ptr->next;
                    i++;
                }
                this->pList->removeAt(i);
                this->pNode=ptr->prev;
            }
            
        }
        void set(const T& element)
        {
            pNode->data=element;
        }
        Iterator &operator++()
        {
            if(pNode)
            pNode=pNode->next;
            return *this;
            
        }
        Iterator operator++(int)
        {
            Iterator i = *this; 
            ++*this; 
            return i;
        }
    };
};

template <class T>
string FragmentLinkedList<T>::toString()
{
    stringstream ss;
    ss << "[";
    Node *ptr = this->fragmentPointers[0];

    if (this->count == 0) 
    {
        ss <<"]";
    }
    for(int i=0;i<this->count-2;i++)
    {
        ss << ptr->data << ", ";
        ptr=ptr->next;
    }
    ss << ptr->data;
    ss << "]";
    return ss.str();
}


template <class T>
void FragmentLinkedList<T>::add(const T& element)
{
    Node* temp=new Node(element,NULL,NULL);
    if(this->fragmentPointers[0]==NULL)
    {
        this->fragmentPointers[0]=temp;
        this->fragmentPointers[1]=temp;
    }
    else
    {
        int current=this->count/this->fragmentMaxSize;
        if(this->count%this->fragmentMaxSize==0)
        {
            this->fragmentPointers[current]=temp;
            this->fragmentPointers[current+1]=temp;
            Node* ptr=this->fragmentPointers[current-1];
            for(int i=0;i<this->fragmentMaxSize;i++)
            {
                ptr=ptr->next;
            }
            ptr->next=temp;
            temp->prev=ptr;
            this->count++;
        }
        else
        {
            Node* ptr=this->fragmentPointers[current];
            for(int i=0;i<this->count%this->fragmentMaxSize;i++)
            {
                ptr=ptr->next;
            }
            ptr->next=temp;
            temp->prev=ptr;
            this->fragmentPointers[current+1]=temp;
            this->count++;
        }
        
    }
    
}
template <class T>
void FragmentLinkedList<T>::add(int index, const T& element)
{
    Node *temp= new Node(element,NULL,NULL);
    int counter_fragment=this->count/this->fragmentMaxSize +1;
    if(this->fragmentPointers[0]==NULL){
        this->fragmentPointers[0]=temp;
        this->fragmentPointers[1]=temp;
        this->count++;
        return;
    }
    if(index==0)
    {
        temp->next=this->fragmentPointers[0];
        this->fragmentPointers[0]->prev=temp;
        
    }
    else
    {
        Node* ptr=this->fragmentPointers[0];
        for(int i=0;i<index-1;i++)
        {
            ptr=ptr->next;
        }
        if(ptr->next==NULL)
        {
            ptr->next=temp;
            temp->prev=ptr;
        }
        else
        {
            temp->next=ptr->next;
            ptr->next->prev=temp;
            ptr->next=temp;
            temp->prev=ptr;
        }
        
    }
    if(this->count%this->fragmentMaxSize==0)
    {
        Node* p=this->fragmentPointers[0];
        while(p->next!=NULL)            
        {
                p=p->next;
        }
        this->fragmentPointers[counter_fragment]=p;
        this->fragmentPointers[counter_fragment+1]=p;
    }
    int k=index/this->fragmentMaxSize+1;
    for(int i=k;i<counter_fragment;i++)            
    {
        this->fragmentPointers[i]=this->fragmentPointers[i]->prev;
    } 
    this->count++;
}
template <class T>
T FragmentLinkedList<T>::removeAt(int index)
{
    T a=-1;
    int counter_fragment_remove=this->count/this->fragmentMaxSize;
    if(this->count==1)
    {
        a=this->fragmentPointers[0]->data;
        this->count = 0;
        this->fragmentPointers[0] = NULL;
        this->fragmentPointers[1] = NULL;
        return a;
    }
    if(index==0)
    {
        a=this->fragmentPointers[0]->data;
        Node* ptr=this->fragmentPointers[0];
        ptr=ptr->next;
        ptr->prev=NULL;
        this->fragmentPointers[0]=ptr;
        if(this->count%this->fragmentMaxSize==1)
        {
            for(int i=1;i<counter_fragment_remove;i++)
            {
                this->fragmentPointers[i]=this->fragmentPointers[i]->next;
            }
            this->fragmentPointers[counter_fragment_remove]=this->fragmentPointers[counter_fragment_remove+1];
            this->fragmentPointers[counter_fragment_remove+1]=NULL;
        }
        else
        {
            for(int i=1;i<=counter_fragment_remove;i++)
            {
                this->fragmentPointers[i]=this->fragmentPointers[i]->next;
            }
        }
        
    }
    else if(index==this->count-1)
    {
        Node* ptr=this->fragmentPointers[0];
        for(int i=0;i<index-1;i++)
        {
            ptr=ptr->next;
        }
        a=ptr->next->data;
        ptr->next=NULL;
        if(this->count%this->fragmentMaxSize==1)
        {
            this->fragmentPointers[counter_fragment_remove]=ptr;
            this->fragmentPointers[counter_fragment_remove+1]=NULL;
        }

    }
    else
    {
        Node* ptr=this->fragmentPointers[0];
        for(int i=0;i<index-1;i++)
        {
            ptr=ptr->next;
        }
        a=ptr->next->data;
        ptr->next=ptr->next->next;
        ptr->next->next->prev=ptr;
        for(int i=index/this->fragmentMaxSize+1;i<counter_fragment_remove+1;i++)
        {
            this->fragmentPointers[i]=this->fragmentPointers[i]->next;
        }
    }
    this->count--;
    return a;
}
template <class T>
bool FragmentLinkedList<T>::removeItem(const T& item)
{
    T dk=this->indexOf(item);   
    if(dk!=-1) 
    {
        this->removeAt(dk);
        return true;
    }   
    else
    {
        return false;
    } 
}
template<class T>
bool FragmentLinkedList<T>::empty()
{
    if(this->count==0)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}
template <class T>
int FragmentLinkedList<T>::size()
{
    return this->count;
}
template <class T>
void FragmentLinkedList<T>::clear()
{
    int i= this->count/this->fragmentMaxSize;
    for(int k=0;k<i+2;k++){
        delete fragmentPointers[k];
    }
}
template <class T>
T FragmentLinkedList<T>::get(int index)
{
    if(index>=this->count) return -1;
    Node* ptr=this->fragmentPointers[0];
    for(int i=0;i<index;i++)
    {
        ptr=ptr->next;
    }
    return ptr->data;
}
template<class T>
void FragmentLinkedList<T>::set(int index, const T& element)
{
    Node* ptr=this->fragmentPointers[0];
    for(int i=0;i<index;i++)
    {
        ptr=ptr->next;
    }
    ptr->data=element;
}
template<class T>
int FragmentLinkedList<T>::indexOf(const T& item)
    {
        Node* ptr=this->fragmentPointers[0];
        for(int i=0;i<this->count;i++)
        {
            if(ptr->data==item) return i;
            ptr=ptr->next;
        }
        return -1;
    }
template<class T>
bool FragmentLinkedList<T>::contains(const T& item)
{
    T dk=this->indexOf(item);   
    if(dk!=-1) 
    {
        return true;
    }   
    else
    {
        return false;
    } 
}

int main()
{
    TESTCASE INPUT
    === Example
    for(int i = 0; i < 20 ; i++)
        fList.add(i, i * i);

    for(FragmentLinkedList<int>::Iterator it = fList.begin(); it != fList.end(); it++)
        cout << *it << " ";
    cout << endl;
    

    return 0;
}
