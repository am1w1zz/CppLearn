#pragma once
#include <iostream>
#include <algorithm>
#include "binary_tree_node.hpp"

template<typename T> 
struct binary_tree {
public:
    int size;
    binary_tree_node<T>* root; //规模、根节点
    int tree_size(binary_tree_node<T>* x);
    virtual int updateHeight(binary_tree_node<T>* x); //更新节点x的高度
    void updateHeightAbove(binary_tree_node<T>* x); //更新节点x及其祖先的高度
    binary_tree() : size(0), root(nullptr) {} //构造函数
    ~binary_tree() { if(0 < size) remove(root);} //析构函数
    
    bool empty() const {return !root;}
    binary_tree_node<T>* get_root() const {return root;}
    binary_tree_node<T>* insert_as_root(T const& e);
    binary_tree_node<T>* insert_as_lc(binary_tree_node<T>* x, T const& e); //e作为x的左孩子（原无）插入
    binary_tree_node<T>* insert_as_rc(binary_tree_node<T>* x, T const& e); //e作为x的右孩子（原无）插入
    binary_tree_node<T>* attach_as_lc(binary_tree_node<T>* x, binary_tree<T>* &Tree); //T作为x左子树接入
    binary_tree_node<T>* attach_as_rc(binary_tree_node<T>* x, binary_tree<T>* &Tree); //T作为x右子树接入
    int remove(binary_tree_node<T>* x); //删除以位置x处节点为根的子树，返回该子树原先的规模
    binary_tree<T>* secede(binary_tree_node<T>* x); //将子树x从当前树中摘除，并将其转换为一棵独立子树
    
    void travLevel() { 
        if(root) {
            root->travLevel();
        }
    }
    
    void travPre() { 
        if(root) {
            root->travPre();
        }
    } 
    
    void travIn() { 
        if(root) {
            root->travIn();
        }
    }
    
    void travPost() {
        if(root) {
            root->travPost();
        }
    }
    bool operator< ( binary_tree<T> const& t ) {
        return root && t.root && lt(root, t.root); 
    }
    bool operator== ( binary_tree<T> const& t ) {
        return root && t.root && ( root == t.root );
    }
};

template<typename T> //统计二叉树子树规模
int binary_tree<T>::tree_size(binary_tree_node<T>* x) {
    return x ? tree_size(x->lc) + tree_size(x->rc) + 1 : 0;
}

template<typename T> 
int binary_tree<T>::updateHeight(binary_tree_node<T>* x ) { 
    return x->height = 1 + std::max(stature(x->lc), stature(x->rc));
}

template<typename T>
void binary_tree<T>::updateHeightAbove(binary_tree_node<T>* x) {
    while(x) {
        updateHeight(x); 
        x = x->parent; 
    } 
} //从x出发，覆盖历代祖先。可优化

template<typename T> 
binary_tree_node<T>* binary_tree<T>::insert_as_root(T const& e) { 
    size = 1; 
    return root = new binary_tree_node<T>(e);
}

template<typename T> 
binary_tree_node<T>* binary_tree<T>::insert_as_lc(binary_tree_node<T>* x, T const& e) {
    size++;
    x->insert_as_lc(e);
    updateHeightAbove(x);
    return x->lc; 
} //e插入为x的左孩子

template<typename T> 
binary_tree_node<T>* binary_tree<T>::insert_as_rc(binary_tree_node<T>* x, T const& e) {
    size++;
    x->insert_as_rc(e);
    updateHeightAbove(x);
    return x->rc;
} //e插入为x的右孩子

template<typename T>
binary_tree_node<T>* binary_tree<T>::attach_as_lc(binary_tree_node<T>* x, binary_tree<T>* &S) { //x->lc == NULL
    if(x->lc = S->root) {
        x->lc->parent = x;
    }
    size += S->size;
    updateHeightAbove(x);
    S->root = NULL;
    S->size = 0;
    release(S);
    S = NULL;
    return x;
}

template<typename T>
binary_tree_node<T>* binary_tree<T>::attach_as_rc(binary_tree_node<T>* x, binary_tree<T>* &S) {
    if(x->rc = S->root) {
        x->rc->parent = x;
    }
    size += S->size;
    updateHeightAbove(x);
    S->root = NULL;
    S->size = 0;
    release(S);
    S = NULL;
    return x;
}

template<typename T>
int binary_tree<T>::remove(binary_tree_node<T>* x) { 
    FromParentTo(*x) = NULL;
    updateHeightAbove(x->parent);
    int n = remove_at(x);
    size -= n;
    return n;
}

template<typename T>
static int remove_at(binary_tree_node<T>* x) {
    if(!x) {
        return 0;
    }
    int n = 1 + remove_at(x->lc) + remove_at(x->rc);
    delete[] x;
    return n;
}

template<typename T>
binary_tree<T>* binary_tree<T>::secede (binary_tree_node<T>* x) {
    FromParentTo(*x) = NULL;
    updateHeightAbove(x->parent);
    binary_tree<T>* S = new binary_tree<T>;
    S->root = x;
    x->parent = NULL;
    S->size = x->size();
    size -= S->size;
    return S;
}