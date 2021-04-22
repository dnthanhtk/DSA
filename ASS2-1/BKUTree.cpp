#include <iostream>
#include <queue>
#include <vector>
using namespace std;




template <class K, class V>
class BKUTree {
public:
    class AVLTree;
    class SplayTree;

    class Entry {
    public:
        K key;
        V value;

        Entry(K key, V value) : key(key), value(value) {}
    };

private:
    AVLTree* avl;
    SplayTree* splay;
    queue<K> keys;
    int maxNumOfKeys;

public:
    BKUTree(int maxNumOfKeys = 5) {
        this->maxNumOfKeys=maxNumOfKeys;
        this->avl= new AVLTree();
        this->splay= new SplayTree();
    }
    ~BKUTree() { this-> clear(); }

    void add(K key, V value)
    { 
        bool exist=this->avl->exist(key);// kiểm tra xem đã có key chưa
        if(exist==true)
        {
            throw "Duplicate key";
            return;
        }
        push_queue(key);
        this->splay->add(key,value);
        this->avl->add(key,value);
        typename AVLTree::Node* avl_connect=this->avl->search_root(key);
        avl_connect->corr=this->splay->get_root_Splay();
        this->splay->root->corr=this->avl->search_root(key);     
    }
    
    void push_queue(K key)
    {
        if(this->keys.size()==(unsigned int)this->maxNumOfKeys)
        {
            this->keys.pop();
            this->keys.push(key);
        }
        else
        {
            this->keys.push(key);
        }
    }
    void remove(K key)
    {
        this->splay->remove(key);
        this->avl->remove(key);
        if(atqueue(key)==true)
        {
            pop_queue(key);
            push_queue(this->splay->root->entry->key);
        }
        
    }
    void pop_queue(K key)
    {
        queue<K> queue_;
        while (this->keys.empty()!=true)
        {
            if(this->keys.front()!=key)
            {
                queue_.push(this->keys.front());
            }
           this->keys.pop();
 
        }
        this->keys=queue_;
    }
    bool atqueue(K key) //có trong queue hay k
    {
        queue<K> queue_=this->keys;
        if(queue_.empty()==true) return false;
        else
        {
            while(queue_.empty()!=true)
            {
                if(queue_.front()==key) return true;
                queue_.pop();
            }
            return false;
        }
    }
    V search(K key, vector<K>& traversedList)
    {
        if(this->splay->root->entry->key==key) 
        {
            push_queue(key);
            return this->splay->root->entry->value;
        }
        if(atqueue(key)==true)
        {  
             //bool z=this->splay->zig_or_zag(this->splay->root,key,this->splay->root);
             bool z=this->splay->not_parent(this->splay->root,key);
             typename BKUTree::SplayTree::Node* not_queue;
            if(z==true)
            { 
               // not_queue=this->splay->splay_one_z(this->splay->root,key,this->splay->root,traversedList);
                not_queue=this->splay->splay_one_not_parent_list(this->splay->root,key,traversedList);
            
            }
            else
            {
                //not_queue=this->splay->splay_one(this->splay->root,key,this->splay->root,traversedList);
                not_queue=this->splay->splay_one_parent_list(this->splay->root,key,this->splay->root,traversedList);
            }
            V as=not_queue->entry->value;
            push_queue(key);
            return as;
        }
        else
        {
            typename SplayTree::Node* r=this->splay->root;
            typename AVLTree::Node* r_=r->corr;
            typename AVLTree::Node* f_=this->avl->search_root(r_,key,traversedList);
            if(f_==NULL)
            {
                f_=this->avl->search_root(this->avl->root,key,traversedList,r_);
            }
            if(f_==NULL)
            {
                throw "Not found";
            }
            typename SplayTree::Node* f=f_->corr;
            V y=f->entry->value;
            bool z=this->splay->not_parent(this->splay->root,key); 
            if(z==true)
            { 

                this->splay->splay_one_not_parent(this->splay->root,key);
            }
            else
            {
                this->splay->splay_one_parent(this->splay->root,key,this->splay->root);
            }
            push_queue(key);
            return y;
        }
    }
    void traverseNLROnAVL(void (*func)(K key, V value))
    {
        this->avl->traverseNLR(this->avl->root,func);
    }
    void traverseNLROnSplay(void (*func)(K key, V value))
    {
        this->splay->traverseNLR(this->splay->root,func);
    }
    void clear()
    {
        this->avl->clear();
        this->splay->clear(); 
       
    }

    class  SplayTree {
    public:
        class Node {
            public:
            Entry* entry;
            Node* left;
            Node* right;
            typename AVLTree::Node* corr;

            Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL) {
                this->entry = entry;
                this->left = left;
                this->right = right;
                this->corr = NULL;
            }
        };

    public:
        Node* root;

        SplayTree(): root(NULL) {};
        ~SplayTree() { this->clear(); };
        Node* get_root_Splay()
        {
            return this->root;
        }
        void add(K key, V value)
        {
            this->add(this->root,key,value);
            this->splay(this->root,key,this->root);
        }
        void add(Entry* entry)
        {
            this->add(entry->key,entry->value);
        }
        void add(Node* &root,K key,V value){
            if(root==NULL) 
            {
                Entry* E= new Entry(key,value);
                root=new Node(E);
                 
            }
            else if(root->entry->key<key){
                add(root->right,key,value);
            }
            else if(root->entry->key > key){
                add(root->left,key,value);
            }
            else{
                throw "Duplicate key";
            }
        }
        void remove(K key)
        {
            Node* ptr= new Node;
            splay(this->root,key,this->root);
            if(this->root->entry->key != key) throw "Not found";
            if(this->root->left == NULL){
                ptr= this->root;
                this->root=this->root->right;
            }
            else{
                ptr= this->root;
                K max=getMax(this->root->left);
                splay(this->root->left,max,this->root->left);
                this->root=this->root->left;
                this->root->right=ptr->right;
            }     
        }
        V search(K key)
        {
            splay(this->root,key,this->root);
            if(root->entry->key != key)  throw "Not found";
            return this->root->entry->value;
        }
        
        V search(Node* r,K key)
        {
            splay(r,key,this->root);
            if(r->entry->key != key)  throw "Not found";
            return r->entry->value;
        }
        void traverseNLR(void (*func)(K key, V value))
        {
            this->traverseNLR(this->root,func);
        }
        void traverseNLR(Node* root,void (*func)(K key, V value))
        {
            if(root){
                func(root->entry->key,root->entry->value);
                traverseNLR(root->left,func);
                traverseNLR(root->right,func);
            }

        }

        void clear()
        {
            this->clear(this->root);
        }
        void clear(Node* root)
        {
            if(root==NULL)
            {
                return;
            }
            clear(root->left);
            clear(root->right);
            delete root;
        }
        K getMax(Node* root) 
        {
            Node* ptr=root;
            while (ptr->right !=NULL) {
                ptr=ptr->right;
            }
            return ptr->entry->key;
        }
        /// dùng để splay đúng 1 lần
        void Zag(Node* &root,Node* &p,int determine){ ///determine=1 ->left, =2 ->right
            Node* ptr=root->right;
            root->right=ptr->left;
            ptr->left=root;
            root=ptr;
            if(determine==1)
            {
                p->left=root;
            }
            else
            {
                p->right=root;
            }  
        }
        void Zig(Node* &root,Node* &p,int determine){
            Node* ptr=root->left;
            root->left=ptr->right;
            ptr->right=root;
            root=ptr;  
        }
        void ZigZig(Node* &root,Node* &p,int determine){
            Node* ptr=root->left->left;
            root->left->left= ptr->right;
            Zig(root);
            ptr->right=root;
            root=ptr;
            if(determine==1)
            {
                p->left=root;
            }
            else
            {
                p->right=root;
            }
        }
        void ZagZag(Node* &root,Node* &p,int determine){
            Node* ptr=root->right->right;
            root->right->right= ptr->left;
            Zag(root);
            ptr->left=root;
            root=ptr;
            if(determine==1)
            {
                p->left=root;
            }
            else
            {
                p->right=root;
            }
            
           
        }
        void ZigZag(Node* &root,Node* &p,int determine){
            Node* ptr=root->left->right;
            Zig(root);
            root->right->left=ptr->right;
            ptr->right=root->right;
            root->right=ptr->left;
            ptr->left=root;
            root=ptr;
            if(determine==1)
            {
                p->left=root;
            }
            else
            {
                p->right=root;
            }
        }
        void ZagZig(Node* &root,Node* &p,int determine){
            Node* ptr = root->right->left;
            Zag(root);
            root->left->right=ptr->left;
            ptr->left=root->left;
            root->left=ptr->right;
            ptr->right=root;
            root=ptr;
            if(determine==1)
            {
                p->left=root;
            }
            else
            {
                p->right=root;
            }
        }
        void Zag(Node* &root){
            Node* ptr=root->right;
            root->right=ptr->left;
            ptr->left=root;
            root=ptr;
        }
        void Zig(Node* &root){
            Node* ptr=root->left;
            root->left=ptr->right;
            ptr->right=root;
            root=ptr;
        }
        void ZigZig(Node* &root){
            Node* ptr=root->left->left;
            root->left->left= ptr->right;
            Zig(root);
            ptr->right=root;
            root=ptr;
        }
        void ZagZag(Node* &root){
            Node* ptr=root->right->right;
            root->right->right= ptr->left;
            Zag(root);
            ptr->left=root;
            root=ptr;
        }
        void ZigZag(Node* &root){
            Node* ptr=root->left->right;
            Zig(root);
            root->right->left=ptr->right;
            ptr->right=root->right;
            root->right=ptr->left;
            ptr->left=root;
            root=ptr;
        }
        void ZagZig(Node* &root){
            Node* ptr = root->right->left;
            Zag(root);
            root->left->right=ptr->left;
            ptr->left=root->left;
            root->left=ptr->right;
            ptr->right=root;
            root=ptr;
        }
        void splay(Node* &root,K key,Node* ptr,vector<K> &list)
        {
            list.push_back(root->entry->key);
            if(root==NULL || root->entry->key==key) 
            {
                list.pop_back();
                return; 
            }
            if(root->entry->key > key){ //left
                splay(root->left,key,ptr);
                if(root->left==NULL) return;
                if (root->left != NULL) 
                {
                    if(root->left->left != NULL){
                        if(root->left->left->entry->key==key){ //zigzig
                            ZigZig(root);
                            return;
                        }
                    }
                    if(root->left->right !=NULL){
                        if(root->left->right->entry->key==key){ //zigzag
                            ZigZag(root);
                            return;
                        }
                    }
                    
                }
                if(root->left->entry->key==key && root==ptr){ //zig
                    Zig(root);
                }
            }
            else{ //right
                splay(root->right,key,ptr);
                if(root->right==NULL) return;
                if (root->right != NULL){
                    if(root->right->right != NULL){
                        if(root->right->right->entry->key==key){ //zagzag
                            ZagZag(root);
                            return;
                        }
                    }
                    if(root->right->left !=NULL){
                        if(root->right->left->entry->key==key){ //zagzig
                            ZagZig(root);
                            return;
                        }
                    }
                }
                if(root->right->entry->key==key && root==ptr){ //zag
                    Zag(root);
                } 
            }
        }
        bool not_parent(Node *root,K key)
        {
            if(root->entry->key > key)
            { //left
                if(root->left!=NULL)
                {
                    if(root->left->entry->key==key) return true;
                    if(root->left->left!=NULL)
                    {
                        if(root->left->left->entry->key==key) return true;
                    }
                    if(root->left->right!=NULL)
                    {
                        if(root->left->right->entry->key==key) return true;
                    }
                }
            }
            else
            { //right 
                if(root->right!=NULL)
                {
                    if(root->right->entry->key==key) return true;
                    if(root->right->left!=NULL)
                    {
                        if(root->right->left->entry->key==key) return true;
                    }
                    if(root->right->right!=NULL)
                    {
                        if(root->right->right->entry->key==key) return true;
                    }
                }
            }
            return false;     
        }
        void splay(Node* &root,K key,Node* ptr)
        {
            if(root==NULL || root->entry->key==key) return; 
            if(root->entry->key > key){ //left
                splay(root->left,key,ptr);
                if(root->left==NULL) return;
                if (root->left != NULL) {
                    if(root->left->left != NULL){
                        if(root->left->left->entry->key==key){ //zigzig
                            ZigZig(root);
                            return;
                        }
                    }
                    if(root->left->right !=NULL){
                        if(root->left->right->entry->key==key){ //zigzag
                            ZigZag(root);
                            return;
                        }
                    }
                }
                if(root->left->entry->key==key && root==ptr){ //zig
                    Zig(root);
                }
            }
            else
            { //right
                splay(root->right,key,ptr);
                if(root->right==NULL) return;
                if (root->right != NULL){
                    if(root->right->right != NULL){
                        if(root->right->right->entry->key==key){ //zagzag
                            ZagZag(root);
                            return;
                        }
                    }
                    if(root->right->left !=NULL){
                        if(root->right->left->entry->key==key){ //zagzig
                            ZagZig(root);
                            return;
                        }
                    }
                }
                if(root->right->entry->key==key && root==ptr){ //zag
                    Zag(root);
                } 
            }
        }
        void splay_one_not_parent(Node* &root,K key)
        {
            if(root->entry->key > key)
            { //left
                if(root->left!=NULL)
                {
                    if(root->left->entry->key==key)
                    { //zig
                        Zig(root);
                        return;
                    
                    }
                    if(root->left->left!=NULL)
                    {
                        if(root->left->left->entry->key==key)
                        { //zigzig
                            ZigZig(root);
                            return;
                        }
                    }
                    if(root->left->right!=NULL)
                    {
                        if(root->left->right->entry->key==key)
                        { //zigzag
                            ZigZag(root);
                             return;
                        }
                    }
                }
            }
            else
            { //right
                
                if(root->right!=NULL)
                {
                    if(root->right->entry->key==key)
                    { //zag
                        Zag(root);
                        return;
                    }
                    if(root->right->right!=NULL)
                    {
                        if(root->right->right->entry->key==key)
                        { //zagzag
                            ZagZag(root);
                            return;            
                        } 
                    }   
                    if(root->right->left!=NULL)
                    {
                        if(root->right->left->entry->key==key)
                        { //zagzig
                            ZagZig(root);
                            return;
                        }
                    }
                }
                
            }
        }
        void splay_one_parent(Node* root,K key,Node* ptr)//không lưu vết
        {
            bool ok=false;
            Node* parent=NULL;
            int deter=1;
            while(ok==false)
            {
                if(root->entry->key > key)
                { //left
                    if(root->left!=NULL)
                    {
                        if(root->left->left!=NULL)
                        {
                            if(root->left->left->entry->key==key)
                            { //zigzig
                            
                                ZigZig(root,parent,deter);
                                ok=true;
                                return;  
                            }
                        }
                        if(root->left->right!=NULL)
                        {
                            if(root->left->right->entry->key==key)
                            { //zigzag                                
                                ZigZag(root,parent,deter);
                                ok=true;
                                return;
                            }
                        }
                    }
                    parent=root;            
                    root=root->left;
                    deter=1;
                }
                else
                { //right                  
                    if(root->right!=NULL)
                    {
                        if(root->right->right!=NULL)
                        {
                            if(root->right->right->entry->key==key)
                            {//zagzag
                                ZagZag(root,parent,deter);
                                ok=true;
                                return;
                            }
                        }
                        if(root->right->left!=NULL)
                            {
                                if(root->right->left->entry->key==key)
                                { //zagzig
                                    ZagZig(root,parent,deter);
                                    ok=true;
                                    return;
                                }
                            } 
                    }  
                    parent=root;
                    root=root->right;
                    deter=2;
                }
            }
        }
        ////
        Node* splay_one_not_parent_list(Node* &root,K key,vector<K> &list) // có lưu vết
        {
            if(root->entry->key > key)
            { //left
                if(root->left!=NULL)
                {
                    if(root->left->entry->key==key ){ //zig
                        Zig(root);
                        list.push_back(root->entry->key);
                        return root;
                    }
                }
                if(root->left->left!=NULL)
                {
                    if(root->left->left->entry->key==key)
                    { //zigzig
                        list.push_back(root->entry->key);
                        list.push_back(root->left->entry->key);
                        ZigZig(root);
                        return root;
                    }
                }
                if(root->left->right!=NULL)
                {
                    if(root->left->right->entry->key==key)
                    { //zigzag
                        list.push_back(root->entry->key);
                        list.push_back(root->left->entry->key);
                        ZigZag(root);
                        return root;
                    }
                }
            }
            else
            { //right
                if(root->right!=NULL)
                {
                    if(root->right->entry->key==key)
                    { //zag
                        list.push_back(root->entry->key);
                        Zag(root);
                        return root;
                    } 
                }
                if(root->right->right!=NULL)
                {
                    if(root->right->right->entry->key==key)
                    { //zagzag
                        list.push_back(root->entry->key);
                        list.push_back(root->right->entry->key);
                        ZagZag(root);
                        return root;
                    }
                }
                if(root->right->left!=NULL)
                {
                    if(root->right->left->entry->key==key)
                        { //zagzig
                            list.push_back(root->entry->key);
                            list.push_back(root->right->entry->key);
                            ZagZig(root);
                            return root;
                        }
                }
            } 
        }
        Node* splay_one_parent_list(Node* root,K key,Node* ptr,vector<K> &list)//có lưu vết  
        {
            bool ok=false;
            Node* parent=NULL;
            int deter=1;
            while(ok==false)
            {
                if(root->entry->key > key)
                { //left
                    
                    if (root->left != NULL) 
                    {
                        if(root->left->left != NULL)
                        {
                            if(root->left->left->entry->key==key)
                            { //zigzig
                                list.push_back(root->entry->key);
                                list.push_back(root->left->entry->key);
                                ZigZig(root,parent,deter);
                                ok=true;
                                return root;
                            }
                        }
                        if(root->left->right !=NULL){
                            if(root->left->right->entry->key==key)
                            { //zigzag
                                list.push_back(root->entry->key);
                                list.push_back(root->left->entry->key);
                                ZigZag(root,parent,deter);
                                ok=true;
                                return root;
                            }
                        }
                    }
                    parent=root;
                    list.push_back(root->entry->key);
                    root=root->left;
                    deter=1;
                }
                else
                { //right
                    if (root->right != NULL){
                        if(root->right->right != NULL)
                        {
                            if(root->right->right->entry->key==key)
                            { //zagzag
                                list.push_back(root->entry->key);
                                list.push_back(root->right->entry->key);
                                ZagZag(root,parent,deter);
                                ok=true;
                                return root;
                            }
                        }
                        if(root->right->left !=NULL)
                        {
                            if(root->right->left->entry->key==key)
                            { //zagzig
                                list.push_back(root->entry->key);
                                list.push_back(root->right->entry->key);
                                ZagZig(root,parent,deter);
                                ok=true;
                                return root;
                            }
                        }
                    }
                    parent=root;
                    list.push_back(root->entry->key);
                    root=root->right;
                    deter=2;
                }
            }
        }
    };

    class AVLTree {
    public:
        class Node {
        public:
            Entry* entry;
            Node* left;
            Node* right;
            int balance;
            typename SplayTree::Node* corr;

            Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL) {
                this->entry = entry;
                this->left = left;
                this->right = right;
                this->balance = 0;
                this->corr = NULL;
            }
        };

    public:
        Node* root;
        AVLTree(): root(NULL) {};
        ~AVLTree() { this->clear(); };
        void add(Node* &root,K key,V value){
            if(root==NULL){
                Entry* E= new Entry(key,value);
                root= new Node(E); 
            }
            else if(key<root->entry->key)
            {
                add(root->left,key,value);
            }
            else if(key> root->entry->key)
            {
                add(root->right,key,value);
            }
            else
            {
                throw "Duplicate key";
            }
            int balance=checkBalanceAVL(root);
            if(balance>1 && key < root->left->entry->key)
            { //left left
                RotateRight(root);
            }
            else if(balance > 1 && key > root->left->entry->key ) //left right
            {
                RotateLeft(root->left);
                RotateRight(root);
            }
            if(balance < -1 && key > root->right->entry->key)
            { //right right
                RotateLeft(root);
            }
            else if(balance < -1 && key < root->right->entry->key ) // right left 
            {
                RotateRight(root->right);
                RotateLeft(root);
            }
        }
        void add(K key, V value){
            this->add(this->root,key,value);
        }
        void add(Entry* entry){
            this->add(this->root,entry->key,entry->data);
        }

        void remove(Node* &root,K key){
            if(root==NULL) throw "Not found";
            else if(key < root->entry->key)
            {
                remove(root->left,key);
            }
            else if(key > root->entry->key)
            {
                remove(root->right,key);
            }
            else
            {
                if(root->left==NULL)
                {
                    Node* ptr=root;
                    root=root->right;
                }
                else if(root->right==NULL)
                {
                    Node* ptr=root;
                    root=root->left;
                }
                else
                {
                    Node* ptr=root->left;
                    while(ptr->right !=NULL){
                        ptr=ptr->right;
                    }
                    root->entry->key=ptr->entry->key;
                    root->entry->value=ptr->entry->value;
                    remove(root->left,ptr->entry->key);
                }
            }
            int balance=checkBalanceAVL(root);
            if(balance>1 && checkBalanceAVL(root->left)>=0){ //left left
                RotateRight(root);
            }
            else if(balance > 1 && checkBalanceAVL(root->left) <0 ) //left right
            {
                RotateRight(root->left);
                RotateRight(root);
            }
            if(balance < -1 && checkBalanceAVL(root->right) <=0){ //right right
                RotateLeft(root);
            }
            else if(balance < -1 && checkBalanceAVL(root->right) >0 ) // right left 
            {
                RotateRight(root->right);
                RotateLeft(root);
            }
        }
        void remove(K key)
        {
            this->remove(this->root,key);
        }
        Node* search_root(Node* root,K key)
        {
            if(root==NULL) throw "Not found";
            else if(key <root->entry->key)
            {
                return search_root(root->left,key);
            }
            else if(key>root->entry->key)
            {
                return search_root(root->right,key);
            }
            else
            {
                return root;
            }
        }
        Node* search_root(Node* root,K key,vector<K> &list)
        {
            if(root==NULL)
            {
                return NULL;
            }
            else if(key <root->entry->key)
            {
                list.push_back(root->entry->key);
                return search_root(root->left,key,list);
            }
            else if(key>root->entry->key)
            {
                list.push_back(root->entry->key);
                return search_root(root->right,key,list);
            }
            else
            {
                return root;
            }
        }
        Node* search_root(Node* root,K key,vector<K> &list,Node* r_)
        {
            if(root==NULL)
            {
                return NULL;
            }
            else if(root==r_)
            {
                list.push_back(root->entry->key);
                return NULL;
            }
            else if(key <root->entry->key)
            {
                list.push_back(root->entry->key);
                return search_root(root->left,key,list,r_);
            }
            else if(key>root->entry->key)
            {
                list.push_back(root->entry->key);
                return search_root(root->right,key,list,r_);
            }
            else
            {
                
                return root;
            }
        }
        Node* search_root(K key)
        {
            return this->search_root(this->root,key);
        }
        bool exist(K key)
        {
            return this->exist(this->root,key);
        }
        bool exist(Node* root,K key)
        {
            if(root==NULL) return false;
            else if(key <root->entry->key)
            {
                return exist(root->left,key);
            }
            else if(key>root->entry->key)
            {
                return exist(root->right,key);
            }
            else{
                return true;
            }
        }
        V search(Node* root,K key)
        {
            if(root==NULL) throw "Not found";
            else if(key <root->entry->key)
            {
                return search(root->left,key);
            }
            else if(key>root->entry->key)
            {
                return search(root->right,key);
            }
            else
            {
                return root->entry->value;
            }
        }
        V search(K key)
        {
            return this->search(this->root,key);
        }

        void traverseNLR(Node* root,void (*func)(K key, V value))
        {
            if(root)
            {
                func(root->entry->key,this->root->entry->value);
                traverseNLR(root->left,func);
                traverseNLR(root->right,func);
            }

        }
        void traverseNLR(void (*func)(K key, V value))
        {
            this->traverseNLR(this->root,func);
        }

        void clear()
        {
            this->clear(this->root);
        }
        void clear(Node* root)
        {
            if(root==NULL)
            {
                return;
            }
            clear(root->left);
            clear(root->right);
            delete root;
        }
        void RotateLeft(Node* &root)
        {
            if(root->right!=NULL)
            {
                Node* ptr=root->right;
                root->right=ptr->left;
                ptr->left=root;
                root=ptr;
                root->balance=checkBalanceAVL(root);
            }
        }
        void RotateRight(Node* &root)
        {
            if(root->left!=NULL)
            {
                Node* ptr=root->left;
                root->left=ptr->right;
                ptr->right=root;
                root=ptr;
                root->balance=checkBalanceAVL(root);
            }
        }
        int checkBalanceAVL(Node* root)
        {
            if(root==NULL) return 0;
            return getHeightRec(root->left)-getHeightRec(root->right);
        }
        int getHeightRec(Node *node)
        {
            if (node == NULL) return 0;
            int lh = this->getHeightRec(node->left);
            int rh = this->getHeightRec(node->right);
            return (lh > rh ? lh : rh) + 1;
        }
        int getHeight()
        {
            return this->getHeightRec(this->root);
        }
    };
};


int main() {

}
