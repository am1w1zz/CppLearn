#pragma once
#include <iostream>
#include <queue>
#include <stack>
#include "global.hpp"

template<typename T> 
struct binary_tree_node { 
public:
    T data;
    binary_tree_node<T>* parent;
    binary_tree_node<T>* lc;
    binary_tree_node<T>* rc;
    int height;
    int npl;
    RBColor color;
    binary_tree_node() : parent(nullptr), lc(nullptr), rc(nullptr), height(0), npl(1) {} 
    binary_tree_node<T>(T e, binary_tree_node<T>* p = nullptr, binary_tree_node<T>* lc = nullptr, binary_tree_node<T>* rc = nullptr, int h = 0, int l = 1, RBColor c = RB_RED) :
        data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c){}

    int size();
    // inline bool is_root();
    // inline bool is_left_child();
    // inline bool is_right_child();
    // inline bool has_parrent();
    // inline bool has_left_child();
    // inline bool has_reight_child();
    // inline bool has_both_child();
    // inline bool has_child();
    // inline bool is_leaf();
    void insert_as_lc (T const &);
    void insert_as_rc (T const &);
    binary_tree_node<T>* succ();
    void trav_level();
    void travPre();
    void travIn();
    void travPost();

    inline bool operator< ( binary_tree_node const& bn ) { return data < bn.data; } //小于
    inline bool operator== ( binary_tree_node const& bn ) { return data == bn.data; } //等于
    
    binary_tree_node<T>* zig();
    binary_tree_node<T>* zag();
    binary_tree_node<T>* balance(); //完全平衡化
    binary_tree_node<T>* imitate( const binary_tree_node<T>* );
};

template <typename T> 
int binary_tree_node<T>::size() {
    int s = 1;
    if(lc) {
        s += lc->size();
    }
    if (rc) {
        s += rc->size();
    }
    return s;
}

template <typename T> 
void binary_tree_node<T>::insert_as_lc(T const& e) { 
    lc = new binary_tree_node(e,this); 
}

template <typename T> 
void binary_tree_node<T>::insert_as_rc(T const& e) { 
    rc = new binary_tree_node(e, this);
}

template <typename T> 
binary_tree_node<T>* binary_tree_node<T>::succ() {
    binary_tree_node<T>* s = this;
    if(rc) { 
        s = rc; //右子树中
        while(HasLChild(*s)) { 
            s = s->lc;
        }
    }else { //否则，直接后继应是“将当前节点包含于其左子树中的最低祖先”，具体地就是
        while(IsRChild(*s)) {
            s = s->parent;
        } //逆向地沿右向分支，不断朝左上方移动
        s = s->parent; //最后再朝右上方移动一步，即抵达直接后继（如果存在）
    }
    return s;
}

template<typename T> 
void binary_tree_node<T>::trav_level() {
    std::queue< binary_tree_node<T>* > tree_node_queue;
    tree_node_queue.enqueue(this);
    while (!tree_node_queue.empty()) { 
        binary_tree_node<T>* x = tree_node_queue.dequeue(); 
        std::cout<< x->data <<std::endl;
        if(x->lc) {
            tree_node_queue.enqueue(x->lc); 
        }
        if(x->rc) {
            tree_node_queue.enqueue(x->rc);
        }
    }
}

template<typename T>
void trav_pre_iter_1(binary_tree_node<T>* x) {
    std::stack<binary_tree_node<T>* > tree_node_stack;
    if(x) {
        tree_node_stack.push (x);
    }
    while(!tree_node_stack.empty()) { 
        x = tree_node_stack.top(); 
        tree_node_stack.pop(); 
        std::cout<< x->data <<std::endl;
        if(x->rc) {
            tree_node_stack.push(x->rc);
        } 
        if(x->lc) {
            tree_node_stack.push(x->lc);
        }
    }
}

template<typename T>
static void visit_along_vine(binary_tree_node<T>* x, std::stack<binary_tree_node<T>* >& S) {
    while(x) {
        std::cout<< x->data <<std::endl;
        S.push(x->rc);
        x = x->lc;
    }
}

template<typename T>
void trav_pre_iter_2(binary_tree_node<T>* x) {
    std::stack<binary_tree_node<T>* > S;
    while(true) {
        visit_along_vine(x,S);
        if (S.empty()) {
            break; 
        }
        x = S.pop();
    }
}

template <typename T>
void trav_pre_recursive(binary_tree_node<T>* x) {
    if(!x) {
        return;
    }
    visit(x->data);
    trav_pre_recursive(x->lc);
    trav_pre_recursive(x->rc);
}

template <typename T> //元素类型、操作器
void trav_in_iter_1(binary_tree_node<T>* x) { //二叉树中序遍历算法（迭代版#2）
    std::stack<binary_tree_node<T>*> S; //辅助栈
    while(true) {
        if (x) {
            S.push(x); //根节点进栈
            x = x->lc; //深入遍历左子树
        } else if(!S.empty()) {
            x = S.top(); //尚未访问的最低祖先节点退栈
            S.pop();
            std::cout<< x->data << "  height:" << x->height << " address:"<< x <<std::endl;

            x = x->rc; //遍历祖先的右子树
        } else {
            break; //遍历完成
        }
    }
} 

template<typename T>
static void go_along_vine(binary_tree_node<T>* x, std::stack<binary_tree_node<T>* >& S ) {
    while(x) { 
        S.push(x);
        x = x->lc; 
    }
}

template <typename T>
void trav_in_iter_2(binary_tree_node<T>* x) {
    std::stack<binary_tree_node<T>* > S;
    while(true) {
        go_along_vine(x,S);
        if(S.empty()) {
            break;
        }
        x = S.pop();
        std::cout<< x->data <<std::endl;
        x = x->rc;
    }
}

template <typename T>
void trav_in_iter_3(binary_tree_node<T>* x) { //二叉树中序遍历算法（迭代版#3，无需辅助栈）
    bool backtrack = false;
    while(true) {
        if(!backtrack && HasLChild(*x)){
            x = x->lc;
        }
        else { //否则——无左子树或刚刚回溯（相当于无左子树）
            std::cout<< x->data <<std::endl;
            if(HasRChild(*x)) { //若其右子树非空，则
                x = x->rc; //深入右子树继续遍历
                backtrack = false; //并关闭回溯标志
            }else { //若右子树空，则
                if(!(x=x->succ())) {
                    break;
                } //回溯（含抵达末节点时的退出返回）
                backtrack = true; //并设置回溯标志
            }
        }
    }
}

template<typename T> //元素类型、操作器
void trav_in_iter_4(binary_tree_node<T>* x) { //二叉树中序遍历（迭代版#4，无需栈或标志位）
    while(true) {
        if(HasLChild(*x)){ //若有左子树，则
            x = x->lc; //深入遍历左子树
        }else { //否则
            std::cout<< x->data <<std::endl;
            while(!HasRChild(*x)) { //不断地在无右分支处
                if (! (x = x->succ())) {
                    return;
                }else { 
                    visit(x->data);
                }//访问新的当前节点
            }
            x = x->rc; //（直至有右分支处）转向非空的右子树
        }
    }
}

template<typename T>
void trav_in_recursive(binary_tree_node<T>* x ) {
    if(!x) {
        return;
    }
    trav_in_recursive(x->lc);
    visit(x->data);
    trav_in_recursive(x->rc);
} 

template <typename T> //在以S栈顶节点为根的子树中，找到最高左侧可见叶节点
static void gotoLeftmostLeaf(std::stack<binary_tree_node<T>* >& S) { //沿途所遇节点依次入栈
    while(binary_tree_node<T>* x = S.top()) {//自顶而下，反复检查当前节点（即栈顶）
        if(HasLChild(*x)) { //尽可能向左
            if(HasRChild(*x)) {
                S.push(x->rc);
            } //若有右孩子，优先入栈
            S.push (x->lc); //然后才转至左孩子
        } else { //实不得已
            S.push(x->rc); //才向右
        }
    }
    S.pop(); //返回之前，弹出栈顶的空节点
}

template <typename T>
void trav_post_iter(binary_tree_node<T>* x ) { //二叉树的后序遍历（迭代版）
    std::stack<binary_tree_node<T>* > S;
    if(x) {
      S.push(x);
    }; //根节点入栈
    while(!S.empty()) { //x始终为当前节点
        if(S.top() != x->parent){ ////若栈顶非x之父（而为右兄）
            gotoLeftmostLeaf(S);
        } //则在其右兄子树中找到HLVFL（相当于递归深入）
        x = S.pop();
        visit(x->data); //弹出栈顶（即前一节点之后继），并访问之
    }
}


template <typename T>
void trav_post_recursive(binary_tree_node<T>* x){ 
    if(!x) {
        return;
    }
    travPost_R(x->lc);
    travPost_R(x->rc);
    std::cout<< x->data <<std::endl;
}

template <typename T>
binary_tree_node<T>* binary_tree_node<T>::zig() {
    binary_tree_node<T>* lChild = lc;
    lChild->parent = this->parent;
    if(lChild->parent) {
        ((this == lChild->parent->rc ) ? lChild->parent->rc : lChild->parent->lc ) = lChild;
    }
    lc = lChild->rc;
    if(lc) {
        lc->parent = this;
    }
    lChild->rc = this;
    this->parent = lChild;
    height = 1 + max(stature(lc), stature(rc));
    lChild->height = 1 + max (stature(lChild->lc ), stature(lChild->rc));
    for(binary_tree_node<T>* x = lChild->parent; x; x = x->parent) {
        if(HeightUpdated(*x)) {
            break;
        } else {
            x->height = 1 + max ( stature ( x->lc ), stature ( x->rc ) );
        }
    }
    return lChild;
}

template <typename T>
binary_tree_node<T>*  binary_tree_node<T>::zag() {
    binary_tree_node<T>* rChild = rc;
    rChild->parent = this->parent;
    if(rChild->parent) {
        ((this == rChild->parent->lc) ? rChild->parent->lc : rChild->parent->rc ) = rChild;
    }
    rc = rChild->lc; 
    if(rc) {
        rc->parent = this;
    }
    rChild->lc = this;
    this->parent = rChild;
    height = 1 + max ( stature ( lc ), stature ( rc ) );
    rChild->height = 1 + max ( stature ( rChild->lc ), stature ( rChild->rc ) );
    for(binary_tree_node<T>* x = rChild->parent; x; x = x->parent ) {
        if(HeightUpdated(*x)) {
            break;
        } else {
         x->height = 1 + max ( stature ( x->lc ), stature ( x->rc ) );
        }
    }
    return rChild;
}

//通过zig旋转调整，将BST子树x拉伸成最右侧通路
template <typename T>
void stretchByZig(binary_tree_node<T>* & x) {
    int c = 0;
    binary_tree_node<T>* v = x;
    while(x->lc) { 
        x = x->lc;
    }
    for(; v; v = v->rc) {
        while(v->lc) {
            v = v->zig();
        }
        c++;
    }
    printf("\nsize = %d, height = %d, #zig = %d\n\n", x->size(), x->height, c);
}

//通过zag旋转调整，将BST子树x拉伸成最左侧通路
template <typename T>
void stretchByZag(binary_tree_node<T>* & x) {
    int c = 0;
    binary_tree_node<T>* p = x; 
    while(p->rc) {
        p = p->rc;
    } //最大节点，必是子树最终的根
    while (x->lc) {
        x = x->lc;
    } //转至初始最左侧通路的末端
    for (; x != p; x = x->parent) { //若x右子树已空，则上升一层
        while ( x->rc ) {
            x->zag(); //以x为轴做zag旋转
            c++;
        }
    }
    printf("\nsize = %d, height = %d, #zag = %d\n\n", x->size(), x->height, c);
}