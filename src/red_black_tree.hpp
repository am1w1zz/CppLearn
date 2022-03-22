#pragma once
#include "binary_search_tree.hpp"
#include <iostream>


template<typename T> 
struct red_black_tree : public binary_tree<T> { //red_black_tree树模板类
protected:
    void solveDoubleRed(binary_tree_node<T>* x ); //双红修正
    void solveDoubleBlack(binary_tree_node<T>* x ); //双黑修正
    int updateHeight(binary_tree_node<T>* x ); //更新节点x的高度
public:
   binary_tree_node<T>* insert ( const T& e ); //插入（重写）
   bool remove ( const T& e ); //删除（重写）
};

template<typename T>
binary_tree_node<T>* red_black_tree<T>::insert ( const T& e ) { //将e插入红黑树
   binary_tree_node<T>* & x = search (e); 
   if(x) {
       return x;
    } //确认目标不存在（留意对this->hot的设置）
   x = new binary_tree_node<T>(e, this->hot, NULL, NULL, 0);
   this->size++; //创建红节点x：以this->hot为父，黑高度0
   binary_tree_node<T>* xOld = x;
   solveDoubleRed(x);
   return xOld; //经双红修正后，即可返回
} //无论e是否存在于原树中，返回时总有x->data == e

template<typename T>
bool red_black_tree<T>::remove ( const T& e ) { //从红黑树中删除关键码e
   binary_tree_node<T>* & x = search (e);
   if (!x) {
       return false;
    } //确认目标存在（留意this->hot的设置）
   binary_tree_node<T>* r = removeAt(x,this->hot);
   if(!(--this->size)) {
       return true;
    } //实施删除
// assert: this->hot某一孩子刚被删除，且被r所指节点（可能是NULL）接替。以下检查是否失衡，并做必要调整
   if(! this->hot) {
       this->root->color = RB_BLACK;
       updateHeight(this->root);
       return true;
    } //若刚被删除的是根节点，则将其置黑，并更新黑高度
// assert: 以下，原x（现r）必非根，this->hot必非空
   if(BlackHeightUpdated(*this->hot)) {
       return true;
    } //若所有祖先的黑深度依然平衡，则无需调整
   if(IsRed(r)) {
       r->color = RB_BLACK;
       r->height++;
       return true; }  //否则，若r为红，则只需令其转黑
    // assert: 以下，原x（现r）均为黑色
   solveDoubleBlack (r);
   return true; //经双黑调整后返回
} //若目标节点存在且被删除，返回true；否则返回false

/******************************************************************************************
 * red_black_tree双黑调整算法：解决节点x与被其替代的节点均为黑色的问题
 * 分为三大类共四种情况：
 *    BB-1 ：2次颜色翻转，2次黑高度更新，1~2次旋转，不再递归
 *    BB-2R：2次颜色翻转，2次黑高度更新，0次旋转，不再递归
 *    BB-2B：1次颜色翻转，1次黑高度更新，0次旋转，需要递归
 *    BB-3 ：2次颜色翻转，2次黑高度更新，1次旋转，转为BB-1或BB2R
 ******************************************************************************************/
template<typename T> void red_black_tree<T>::solveDoubleBlack ( binary_tree_node<T>* r ) {
    binary_tree_node<T>* p = r ? r->parent : this->hot; if(!p) return; //r的父亲
    binary_tree_node<T>* s = ( r == p->lc ) ? p->rc : p->lc; //r的兄弟
    if(IsBlack(s)) { //兄弟s为黑
        binary_tree_node<T>* t = NULL; //s的红孩子（若左、右孩子皆红，左者优先；皆黑时为NULL）
        if (IsRed(s->rc)) {
            t = s->rc;
        }
        if(IsRed(s->lc)) {
            t = s->lc;
        }
        if(t) { //黑s有红孩子：BB-1
            RBColor oldColor = p->color; //备份原子树根节点p颜色，并对t及其父亲、祖父 // 以下，通过旋转重平衡，并将新子树的左、右孩子染黑
            binary_tree_node<T>* b = FromParentTo ( *p ) = rotateAt ( t ); //旋转
            if(HasLChild(*b)) {
                b->lc->color = RB_BLACK; updateHeight ( b->lc );
            } //左子
            if(HasRChild(*b)) { 
                b->rc->color = RB_BLACK;
                updateHeight(b->rc);
            } //右子
            b->color = oldColor;
            updateHeight(b); //新子树根节点继承原根节点的颜色
        }else { //黑s无红孩子
            s->color = RB_RED;
            s->height--; //s转红
            if(IsRed(p)) { //BB-2R
                p->color = RB_BLACK; //p转黑，但黑高度不变
            } else { 
                p->height--; //p保持黑，但黑高度下降
                solveDoubleBlack(p); //递归上溯
            }
        }
   }else { //兄弟s为红：BB-3
        s->color = RB_BLACK;
        p->color = RB_RED; //s转黑，p转红
        binary_tree_node<T>* t = IsLChild(*s) ? s->lc : s->rc; //取t与其父s同侧
        this->hot = p;
        FromParentTo(*p) = rotateAt(t); //对t及其父亲、祖父做平衡调整
        solveDoubleBlack(r); //继续修正r处双黑——此时的p已转红，故后续只能是BB-1或BB-2R
   }
}

/******************************************************************************************
 * red_black_tree双红调整算法：解决节点x与其父均为红色的问题。分为两大类情况：
 *    RR-1：2次颜色翻转，2次黑高度更新，1~2次旋转，不再递归
 *    RR-2：3次颜色翻转，3次黑高度更新，0次旋转，需要递归
 ******************************************************************************************/
template<typename T> void red_black_tree<T>::solveDoubleRed ( binary_tree_node<T>* x ) { //x当前必为红
    if(IsRoot(*x)) {//若已（递归）转至树根，则将其转黑，整树黑高度也随之递增
        this->root->color = RB_BLACK; this->root->height++; 
        return; 
    } //否则，x的父亲p必存在
    binary_tree_node<T>* p = x->parent;
    if(IsBlack(p)) {
        return;
    } //若p为黑，则可终止调整。否则
    binary_tree_node<T>* g = p->parent; //既然p为红，则x的祖父必存在，且必为黑色
    binary_tree_node<T>* u = uncle(x); //以下，视x叔父u的颜色分别处理
    if(IsBlack(u)) { //u为黑色（含NULL）时 //*DSA*/printf("  case RR-1:\n");
        if(IsLChild(*x) == IsLChild(*p)) { //若x与p同侧（即zIg-zIg或zAg-zAg），则
            p->color = RB_BLACK; //p由红转黑，x保持红
        }else { //若x与p异侧（即zIg-zAg或zAg-zIg），则
            x->color = RB_BLACK; //x由红转黑，p保持红
        }
        g->color = RB_RED; //g必定由黑转红
///// 以上虽保证总共两次染色，但因增加了判断而得不偿失
///// 在旋转后将根置黑、孩子置红，虽需三次染色但效率更高
        binary_tree_node<T>* gg = g->parent; //曾祖父（great-grand parent）
        binary_tree_node<T>* r = FromParentTo (*g) = rotateAt(x); //调整后的子树根节点
        r->parent = gg; //与原曾祖父联接
    }else { //若u为红色 //*DSA*/printf("  case RR-2:\n");
        p->color = RB_BLACK;
        p->height++; //p由红转黑
        u->color = RB_BLACK;
        u->height++; //u由红转黑
        if(!IsRoot(*g)) {
            g->color = RB_RED;
        } //g若非根，则转红
        solveDoubleRed(g); //继续调整g（类似于尾递归，可优化为迭代形式）
   }
}

template<typename T> int red_black_tree<T>::updateHeight ( binary_tree_node<T>* x ) { //更新节点高度
   return x->height = IsBlack(x) + max(stature(x->lc), stature(x->rc)); //孩子黑高度通常相等，除非出现双黑
   /*DSA*/// 红黑树中各节点左、右孩子的黑高度通常相等
   /*DSA*/// 这里之所以取更大值，是便于在删除节点后的平衡调整过程中，正确更新被删除节点父亲的黑高度
   /*DSA*/// 否则，rotateAt()会根据被删除节点的替代者（高度小一）设置父节点的黑高度
}