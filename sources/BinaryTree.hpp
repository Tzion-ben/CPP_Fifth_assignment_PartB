/*
* AUTHORS: <Tzion Beniaminov>
* Tests for Binatree
*/
#pragma once

#include <algorithm>
#include <memory>
#include <iostream>
#include <stack>
#include <vector>

// using namespace std;
// enum From{_from_up , _from_right, _from_left};

namespace ariel {
    template<typename T>
    class BinaryTree {
        
    private:
        // inner class(struct), represents each Node in the tree
        struct Node {
            T _value;
            Node* _right;
            Node* _left;

            Node(const T& v)
                : _value(v), _right(nullptr), _left(nullptr){
            }
        };
        
        // private fields
        Node* _root;

        //private methods
        Node* find_elenemt (Node* n , T element){
            if(n == nullptr){return nullptr;}
            if(n->_value == element){return n;}
            Node* returnd_node = nullptr;

            /*if the element in the rigth size*/
            returnd_node = find_elenemt(n->_left, element);
            if(returnd_node != nullptr){            
                if(returnd_node->_value == element){return returnd_node;}
            }
            /*if the element in the left side*/
            returnd_node = find_elenemt(n->_right, element);
            if(returnd_node != nullptr){            
                if(returnd_node->_value == element){return returnd_node;}
            }

            /*no element in the tree*/
            return nullptr;
        }

        /*copy all the tree - for the copy ctor*/
        Node* copy_values(Node* other){
            if(other == NULL){return NULL;}

            Node* copy_node = new Node(other->_value);
            copy_node->_left = copy_values(other->_left);
            copy_node->_right = copy_values(other->_right);
            return copy_node;
        }

        /*delete all the tree - for the copy ctor & dctor*/
        void DestroyRecursive(Node * root){
            if (root != NULL)
            {
                if(root->_left){DestroyRecursive(root->_left);}
                if(root->_right){DestroyRecursive(root->_right);}
                // cout<< "Deleting DEBUG: " << root->_value << endl;
                delete root;
            }
        }

/*******************************************************************************/
        public:
            /*ctor*/
            BinaryTree()
            :_root(nullptr){}

            /*copy ctor*/
            BinaryTree& add_root(T root){
                if(_root == nullptr){this->_root = new Node(root);}       
                else{this->_root->_value = root;}                   
                return *this;
            }
            
            /*
            * if there is no left child - will create a new one
            * other wise - change the value
            */
            BinaryTree& add_left(T exist_in_tree, T new_to_add){

                Node* temp = find_elenemt(_root, exist_in_tree);

                if(temp == nullptr){throw "no have such element in the tree";}
                if(temp->_left == nullptr){
                   temp->_left = new Node(new_to_add); 
                }else{temp->_left->_value = new_to_add;} 
                return *this;
            }
            
            /*
            * if there is no rigth child - will create a new one
            * other wise - change the value
            */
            BinaryTree& add_right(T exist_in_tree, T new_to_add){
                Node* temp = find_elenemt(_root, exist_in_tree);

                if(temp == nullptr){throw "no have such element in the tree";}
                if(temp->_right == nullptr){
                   temp->_right = new Node(new_to_add); 
                }else{temp->_right->_value = new_to_add;} 
                return *this;
            }
            
            /*regular copy ctor and operator=:*/
            BinaryTree(const BinaryTree &other){
                if(other._root != nullptr){
                    _root = new Node(other._root->_value);
                    _root = copy_values(other._root);
                }
            }

            /*operator=:*/
            BinaryTree& operator=(const BinaryTree& other){
                if(this == &other){return *this;}
                DestroyRecursive(_root);
                _root = new Node(other._root->_value);
                _root = copy_values(other._root);
            
                return *this;
            }

            /*dctor*/
            ~BinaryTree(){DestroyRecursive(_root);}
        /***********************************************/
            /*move constructor and operator=:*/
            BinaryTree(BinaryTree&& other) noexcept {
                _root = other._root;
                other._root = nullptr;
            }
            
            /*operator=:*/
            BinaryTree& operator=(BinaryTree&& other) noexcept {
                if(_root){DestroyRecursive (_root);}
                _root = other._root;
                other._root = nullptr;
            }

           
//---------------------------------------------------------------------------------
//InOrder Iterator:	

            //-------------------------------------------------------------------
            // iterator code:
            // inner class and methods that return instances of it
            //
            // Default iterator will by: inOrder
            //-------------------------------------------------------------------
            class iterator {

            private:
                Node* _current_node;
                int look_to_rigth;
                std::stack<Node*> inoder_tra;  

            public:

                /*ctor - will start from the most left child*/
                iterator(Node* ptr)
                    : _current_node(ptr),look_to_rigth(0) {
                    if(ptr != nullptr){
                        while(_current_node->_left != nullptr){
                            inoder_tra.push(_current_node);
                            // cout << inoder_tra.top()->_value << " con";
                            _current_node = _current_node->_left;
                        }
                    }
                }

                // Note that the method is const as this operator does not
                // allow changing of the iterator.
                // Note that it returns T& as it allows to change what it points to.
                // A const_iterator class will return const T&
                // and the method will still be const
                T& operator*() const {return _current_node->_value;}

                T* operator->() const {return &(_current_node->_value);}

                // ++i;
                iterator& operator++() {
                    _current_node = _current_node->_right;
                    
                    look_to_rigth = 1;
                    while(_current_node != nullptr){
                        inoder_tra.push(_current_node);
                        _current_node = _current_node->_left;
                    }

                    if(inoder_tra.empty()){
                        _current_node = nullptr;
                        return *this;
                    }

                    _current_node = inoder_tra.top();
                    inoder_tra.pop();
                    return *this;
                }

                // i++;
                iterator operator++(int) {
                    iterator tmp= *this;
                    _current_node = _current_node->_right;
                    
                    look_to_rigth = 1;
                    while(_current_node != nullptr){
                        inoder_tra.push(_current_node);
                        _current_node = _current_node->_left;
                    }

                    if(inoder_tra.empty()){
                        _current_node = nullptr;
                        return *this;
                    }

                    _current_node = inoder_tra.top();
                    inoder_tra.pop();                    
                    return tmp;
                }

                bool operator==(const iterator& rhs) const {
                    return _current_node == rhs._current_node;
                }

                bool operator!=(const iterator& rhs) const {
                    return _current_node != rhs._current_node;
                }
            };  // END OF CLASS INORDER ITERATOR

//---------------------------------------------------------------------------------
//PostOrder Iterator:

            //-------------------------------------------------------------------
            // iterator code:
            // inner class and methods that return instances of it
            //
            // Default iterator will by: inOrder
            //-------------------------------------------------------------------
            class PostOrderiterator {

            private:
                /*will use stack and vector to store the elements*/
                Node* _current_node;
                std::stack<Node*> postord_tra_private; 
                std::vector<Node*> postord_tra_public;  

                /*store all the elements in post order in the vector*/
                void create_postOrderStack(){
                    while(!postord_tra_private.empty()){
                        /*take the "current root*/
                        Node* temp = postord_tra_private.top();
                        postord_tra_public.push_back(temp);
                        /*delete the "current root" element from the stack*/
                        postord_tra_private.pop();

                        /*push the left and the rigth child*/
                        if(temp->_left != NULL){
                            postord_tra_private.push(temp->_left);
                        }
                        if(temp->_right != NULL){
                            postord_tra_private.push(temp->_right);
                        }
                    }
                }

            public:

                /*ctor - will start from the most left child*/
                PostOrderiterator(Node* ptr)
                    : _current_node(ptr) {
                    if(ptr != nullptr){
                        postord_tra_private.push(_current_node);
                        create_postOrderStack();
                        _current_node = postord_tra_public.back();
                        postord_tra_public.pop_back();
                    }
                }

                // Note that the method is const as this operator does not
                // allow changing of the iterator.
                // Note that it returns T& as it allows to change what it points to.
                // A const_iterator class will return const T&
                // and the method will still be const
                T& operator*() const {return _current_node->_value;}

                T* operator->() const {return &(_current_node->_value);}

                // ++i;
                PostOrderiterator& operator++() {
                    if(!postord_tra_public.empty()){
                        _current_node = postord_tra_public.back();
                        postord_tra_public.pop_back();
                    }else{_current_node = nullptr;}
                    return *this;
                }

                // i++;
                PostOrderiterator operator++(int) {
                    PostOrderiterator tmp= *this;
                    if(!postord_tra_public.empty()){
                        _current_node = postord_tra_public.back();
                        postord_tra_public.pop_back();
                    }else{_current_node = nullptr;} 
                    return tmp;
                }

                bool operator==(const PostOrderiterator& rhs) const {
                    return _current_node == rhs._current_node;
                }

                bool operator!=(const PostOrderiterator& rhs) const {
                    return _current_node != rhs._current_node;
                }
            };  // END OF CLASS POSTORDER ITERATOR
//---------------------------------------------------------------------------------
//PreOrder Iterator:

            //-------------------------------------------------------------------
            // iterator code:
            // inner class and methods that return instances of it
            //
            // Default iterator will by: inOrder
            //-------------------------------------------------------------------
            class preOrderiterator {

            private:
                /*will use a stack to store all the elements*/
                Node* _current_node;
                int root_in;
                std::stack<Node*> preord_tra;       

            public:

                preOrderiterator(Node* ptr)
                    : _current_node(ptr), root_in(0) {
                    preord_tra.push(_current_node);
                }

                // Note that the method is const as this operator does not
                // allow changing of the iterator.
                // Note that it returns T& as it allows to change what it points to.
                // A const_iterator class will return const T&
                // and the method will still be const
                T& operator*() const {return _current_node->_value;}

                T* operator->() const {return &(_current_node->_value);}

                // ++i;
                preOrderiterator& operator++() {
                    if(!preord_tra.empty()){
                        _current_node = preord_tra.top();
                        preord_tra.pop();

                        if(_current_node->_right != nullptr){
                            preord_tra.push(_current_node->_right);
                        }
                        if(_current_node->_left != nullptr){
                            preord_tra.push(_current_node->_left);
                        }
                        if(root_in == 0){
                            _current_node = preord_tra.top();
                            preord_tra.pop();
                             if(_current_node->_right != nullptr){
                            preord_tra.push(_current_node->_right);
                        }
                            if(_current_node->_left != nullptr){
                                preord_tra.push(_current_node->_left);
                            }
                            root_in =1;
                        }
                    }else{_current_node = nullptr;}

                    return *this;
                }

                // i++;
                preOrderiterator operator++(int) {
                    preOrderiterator tmp= *this;
                     if(!preord_tra.empty()){
                        _current_node = preord_tra.top();
                        preord_tra.pop();

                        if(_current_node->_right != nullptr){
                            preord_tra.push(_current_node->_right);
                        }
                        if(_current_node->_left != nullptr){
                            preord_tra.push(_current_node->_left);
                        }
                        if(root_in == 0){
                            _current_node = preord_tra.top();
                            preord_tra.pop();
                             if(_current_node->_right != nullptr){
                            preord_tra.push(_current_node->_right);
                        }
                            if(_current_node->_left != nullptr){
                                preord_tra.push(_current_node->_left);
                            }
                            root_in =1;
                        }
                    }else{_current_node = nullptr;}
                    return tmp;
                }

                bool operator==(const preOrderiterator& rhs) const {
                    return _current_node == rhs._current_node;
                }

                bool operator!=(const preOrderiterator& rhs) const {
                    return _current_node != rhs._current_node;
                }
            };  // END OF CLASS PREORDER ITERATOR
//---------------------------------------------------------------------------------
        //----------------------------------
        // friend global IO operators
        //----------------------------------
        // friend std::ostream& operator<< (std::ostream& output, const BinaryTree<T>& tree);

        /*begin: start of the tree, end:the end of the tree*/
        iterator begin_inorder() {return iterator{_root};}
        iterator end_inorder() {return iterator{nullptr};}
        
        preOrderiterator begin_preorder() {return preOrderiterator{_root};}
        preOrderiterator end_preorder() {return preOrderiterator{nullptr};}

        PostOrderiterator begin_postorder() {return PostOrderiterator{_root};}
        PostOrderiterator end_postorder() {return PostOrderiterator{nullptr};}

    //default iterator - INORDER iterator
        iterator begin() {return iterator{_root};}
        iterator end() {return iterator{nullptr};}
    }; 

    /*display the tree in InOrder*/
    template<typename T>
    std::ostream& operator<< (std::ostream& output, ariel::BinaryTree<T>& tree){
        for(auto it : tree){
            output << (it) << "," ;
        }
        return output;
    }
}