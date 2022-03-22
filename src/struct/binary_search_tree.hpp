#pragma once

#include "binary_tree.hpp"
#include <iostream>
#include <algorithm>

template <typename T>
struct binary_search_tree : public binary_tree<T> {
public:
    binary_tree_node<T>* hot; //按照“3 + 4”结构，联接3个节点及四棵子树
    binary_tree_node<T>* connect34(binary_tree_node<T>*, binary_tree_node<T>*, binary_tree_node<T>*,binary_tree_node<T>*, binary_tree_node<T>*, binary_tree_node<T>*, binary_tree_node<T>* );
    binary_tree_node<T>* rotateAt(binary_tree_node<T>* x); //对x及其父亲、祖父做统一旋转调整
    virtual binary_tree_node<T>*& search(const T& e); //查找
    virtual binary_tree_node<T>* insert(const T& e); //插入
    virtual bool remove(const T& e); //删除    
    void stretchToLPath() {stretchByZag(this->root);} //借助zag旋转，转化为左向单链
    void stretchToRPath() {stretchByZig(this->root);} //借助zig旋转，转化为右向单链
    void stretch();
};

/******************************************************************************************
 * 按照“3 + 4”结构联接3个节点及其四棵子树，返回重组之后的局部子树根节点位置（即b）
 * 子树根节点与上层节点之间的双向联接，均须由上层调用者完成
 * 可用于AVL和RedBlack的局部平衡调整
 ******************************************************************************************/
template<typename T>
binary_tree_node<T>* binary_search_tree<T>::connect34 (
    binary_tree_node<T>* a, binary_tree_node<T>* b, binary_tree_node<T>* c, binary_tree_node<T>* T0, binary_tree_node<T>* T1, binary_tree_node<T>* T2, binary_tree_node<T>* T3 ) {    
    a->lc = T0;
    if(T0) {
        T0->parent = a;
    }
    a->rc = T1;
    if(T1) {
        T1->parent = a; 
        this->updateHeight(a);
    }
    c->lc = T2;
    if(T2) {
        T2->parent = c;
    }
    c->rc = T3;
    if(T3) {
        T3->parent = c;
        this->updateHeight(c);
    }
    b->lc = a;
    a->parent = b;
    b->rc = c;
    c->parent = b;
    this->updateHeight(b);
    return b; //该子树新的根节点
}

template<typename T>
binary_tree_node<T>* binary_search_tree<T>::rotateAt(binary_tree_node<T>* v) { //v为非空孙辈节点
    if (!v) {
        printf("\a\nFail to rotate a null node\n");
        exit(-1);
    }
    binary_tree_node<T>* p = v->parent;
    binary_tree_node<T>* g = p->parent; //视v、p和g相对位置分四种情况
    if(IsLChild(*p)) {/* zig */
        if(IsLChild(*v)) { /* zig-zig */ //*DSA*/printf("\tzIg-zIg: ");
            p->parent = g->parent; //向上联接        
            return connect34(v, p, g, v->lc, v->rc, p->rc, g->rc);
        } else { /* zig-zag */  //*DSA*/printf("\tzIg-zAg: ");
            v->parent = g->parent; //向上联接
            return connect34(p, v, g, p->lc, v->lc, v->rc, g->rc);
        }
    }else { /* zag */
        if(IsRChild(*v)) { /* zag-zag */ //*DSA*/printf("\tzAg-zAg: ");
            p->parent = g->parent; //向上联接
            return connect34(g, p, v, g->lc, p->lc, v->lc, v->rc );
        } else { /* zag-zig */  //*DSA*/printf("\tzAg-zIg: ");
            v->parent = g->parent; //向上联接
            return connect34(g, v, p, g->lc, v->lc, v->rc, p->rc );
        }
    }
}

template<typename T>
binary_tree_node<T>* & binary_search_tree<T>::search(const T & e) { //在binary_search_tree中查找关键码e
    if(!this->root || e == this->root->data) {
        hot = NULL;
        return this->root;
    } //在树根v处命中
    for(hot = this->root; ;) { //否则，自顶而下
        binary_tree_node<T>* & c = (e < hot->data) ? hot->lc : hot->rc; //确定方向
        if(!c || e == c->data) {
            return c;
        }
        hot = c; //命中返回，或者深入一层
    } //无论命中或失败，hot均指向v之父亲（v是根时，hot为NULL）
} //返回目标节点位置的引用，以便后续插入、删除操作

template<typename T>
binary_tree_node<T>* binary_search_tree<T>::insert(const T& e) { //将关键码e插入binary_search_tree树中
    binary_tree_node<T>*& x = search(e);
    if(x) {
        return x;
    } //确认目标不存在（留意对hot的设置）
    x = new binary_tree_node<T>(e, hot); //创建新节点x：以e为关键码，以hot为父
    this->size++; //更新全树规模
    this->updateHeightAbove(x); //更新x及其历代祖先的高度
    return x; //新插入的节点，必为叶子
} //无论e是否存在于原树中，返回时总有x->data == e

template <typename T> bool binary_search_tree<T>::remove(const T& e) { //从binary_search_tree树中删除关键码e
    binary_tree_node<T>*& x = search(e);
    if(!x) {
        return false;
    }
    removeAt(x,hot);
    this->size--;
    this->updateHeightAbove(hot);
    return true;
} 

/******************************************************************************************
 * binary_search_tree节点删除算法：删除位置x所指的节点（全局静态模板函数，适用于AVL、Splay、RedBlack等各种binary_search_tree）
 * 目标x在此前经查找定位，并确认非NULL，故必删除成功；与searchIn不同，调用之前不必将hot置空
 * 返回值指向实际被删除节点的接替者，hot指向实际被删除节点的父亲——二者均有可能是NULL
 ******************************************************************************************/
template <typename T>
static binary_tree_node<T>* removeAt(binary_tree_node<T>*& x, binary_tree_node<T>*& hot) {
    binary_tree_node<T>* w = x; //实际被摘除的节点，初值同x
    binary_tree_node<T>* succ = NULL; //实际被删除节点的接替者
    if(!HasLChild(*x)) { //若*x的左子树为空，则可
        succ = x->rc; 
        x = x->rc;
    }//直接将*x替换为其右子树
    else if(!HasRChild(*x)) { //若右子树为空，则可
        succ = x->lc;  //对称地处理——注意：此时succ != NULL
        x = x->lc;
    } 
    else { //若左右子树均存在，则选择x的直接后继作为实际被摘除节点，为此需要
        w = w->succ(); //（在右子树中）找到*x的直接后继*w
        std::swap(x->data, w->data); //交换*x和*w的数据元素
        binary_tree_node<T>* u = w->parent;
        ((u == x) ? u->rc : u->lc) = succ = w->rc; //隔离节点*w
    }
    hot = w->parent; //记录实际被删除节点的父亲
    if(succ) {
        succ->parent = hot;
    } //并将被删除节点的接替者与hot相联
    delete[] w;
    return succ; //释放被摘除节点，返回接替者
} //release()负责释放复杂结构，与算法无直接关系，具体实现详见代码包