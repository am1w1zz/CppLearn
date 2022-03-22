#pragma once
#include "binary_search_tree.hpp"

template<typename T>
struct splay_tree : public binary_search_tree<T> {
public:
   binary_tree_node<T>* splay(binary_tree_node<T>* v );
   binary_tree_node<T>* & search(const T& e);
   binary_tree_node<T>* insert( const T& e);
   bool remove(const T& e);
};

template<typename NodePosi>
inline void attachAsLChild(NodePosi p, NodePosi lc) { //在节点*p与*lc（可能为空）之间建立父（左）子关系
    p->lc = lc;
    if(lc) {
        lc->parent = p;
    }
}

template<typename NodePosi> 
inline void attachAsRChild ( NodePosi p, NodePosi rc ) {  //在节点*p与*rc（可能为空）之间建立父（右）子关系
    p->rc = rc;
    if(rc) {
        rc->parent = p;
    }
}

template<typename T> //Splay树伸展算法：从节点v出发逐层伸展
binary_tree_node<T>* splay_tree<T>::splay(binary_tree_node<T>* v) { //v为因最近访问而需伸展的节点位置
    if ( !v ) {
       return NULL; 
    }
    binary_tree_node<T>* p; 
    binary_tree_node<T>* g; //*v的父亲与祖父
    while((p = v->parent) && (g = p->parent)) { //自下而上，反复对*v做双层伸展
        binary_tree_node<T>* gg = g->parent; //每轮之后*v都以原曾祖父（great-grand parent）为父
        if(IsLChild(*v)) {
            if(IsLChild(*p)) { //zig-zig
                // printf ( "\tzIg-zIg :" );print(g);print(p);print(v);printf("\n");
                attachAsLChild(g, p->rc);
                attachAsLChild(p, v->rc);
                attachAsRChild(p, g);
                attachAsRChild(v, p);
            } else { //zig-zag
                // printf("\tzIg-zAg :");print(g);print(p);print(v);printf("\n");
                attachAsLChild(p, v->rc);
                attachAsRChild(g, v->lc);
                attachAsLChild(v, g);
                attachAsRChild(v, p);
            }
        } else if (IsRChild(*p)) { //zag-zag
        //  printf ( "\tzAg-zAg :" );print ( g ); print ( p ); print ( v ); printf ( "\n" );
            attachAsRChild(g, p->lc);
            attachAsRChild(p, v->lc);
            attachAsLChild(p, g);
            attachAsLChild(v, p);
        } else { //zag-zig
        //  printf ( "\tzAg-zIg :" ); print ( g ); print ( p ); print ( v ); printf ( "\n" );
            attachAsRChild(p, v->lc);
            attachAsLChild(g, v->rc);
            attachAsRChild(v, g);
            attachAsLChild(v, p);
        }
        if(!gg) {
            v->parent = NULL;  //若*v原先的曾祖父*gg不存在，则*v现在应为树根
        } else { 
            (g == gg->lc) ? attachAsLChild(gg, v) : attachAsRChild(gg, v);
        }//否则，*gg此后应该以*v作为左或右孩子
        updateHeight(g);
        updateHeight(p);
        updateHeight(v);
    } //双层伸展结束时，必有g == NULL，但p可能非空
    if(p = v->parent) { //若p果真非空，则额外再做一次单旋
        if(IsLChild(*v)) { 
            printf("\tzIg :");print(p);print(v);printf("\n"); 
        }
        else { 
            printf("\tzAg :");print(p);print(v);printf("\n"); 
        }
        if(IsLChild(*v)) { 
            attachAsLChild(p, v->rc);
            attachAsRChild(v, p);
        }
        else {
            attachAsRChild(p, v->lc);
            attachAsLChild(v, p);
        }
        updateHeight(p);
        updateHeight(v);
    }
    v->parent = NULL;
    return v;
} //调整之后新树根应为被伸展的节点，故返回该节点的位置以便上层函数更新树根

template<typename T>
binary_tree_node<T>* & splay_tree<T>::search (const T & e) { //在伸展树中查找e
    binary_tree_node<T>* p = binary_search_tree<T>::search(e);
    this->root = splay(p ? p : this->hot); //将最后一个被访问的节点伸展至根
    return this->root;
} //与其它BST不同，无论查找成功与否，this->root都指向最后被访问的节点

template<typename T>
binary_tree_node<T>* splay_tree<T>::insert ( const T& e ) { //将关键码e插入伸展树中
    if(!this->root) { 
       this->size++;
       return this->root = new binary_tree_node<T> ( e );
    } //处理原树为空的退化情况
    if(e == search(e)->data) {
       return this->root;
    } //确认目标节点不存在
    this->size++;
    binary_tree_node<T>* t = this->root; //创建新节点。以下调整<=7个指针以完成局部重构
    if(this->root->data < e) { //插入新根，以t和t->rc为左、右孩子
        t->parent = this->root = new binary_tree_node<T> (e, NULL, t, t->rc); //2 + 3个
        if(HasRChild(*t)) { 
            t->rc->parent = this->root;
            t->rc = NULL;
        } //<= 2个
    } else { //插入新根，以t->lc和t为左、右孩子
        t->parent = this->root = new binary_tree_node<T> ( e, NULL, t->lc, t ); //2 + 3个
        if(HasLChild(*t)) { 
            t->lc->parent = this->root;
            t->lc = NULL; 
        } //<= 2个
    }
    updateHeightAbove(t); //更新t及其祖先（实际上只有this->root一个）的高度
    return this->root; //新节点必然置于树根，返回之
} //无论e是否存在于原树中，返回时总有this->root->data == e

template<typename T>
bool splay_tree<T>::remove(const T& e) { //从伸展树中删除关键码e
    if(!this->root || ( e != search(e)->data)) {
        return false; 
    }//若树空或目标不存在，则无法删除
    binary_tree_node<T>* w = this->root; //assert: 经search()后节点e已被伸展至树根
    if(!HasLChild(*this->root)) { //若无左子树，则直接删除
        this->root = this->root->rc;
        if(this->root) {
            this->root->parent = NULL;
        }
    } else if(!HasRChild(*this->root)){ //若无右子树，也直接删除
        this->root = this->root->lc;
        if(this->root) {
            this->root->parent = NULL;
        }
    } else { //若左右子树同时存在，则
        binary_tree_node<T>* lTree = this->root->lc;
        lTree->parent = NULL; this->root->lc = NULL; //暂时将左子树切除
        this->root = this->root->rc; this->root->parent = NULL; //只保留右子树
        search ( w->data ); //以原树根为目标，做一次（必定失败的）查找 ///// assert: 至此，右子树中最小节点必伸展至根，且（因无雷同节点）其左子树必空，于是
        this->root->lc = lTree; lTree->parent = this->root; //只需将原左子树接回原位即可
   }
    release(w->data);
    release(w);
    this->size--; //释放节点，更新规模
    if(this->root) {
        updateHeight(this->root);
    } //此后，若树非空，则树根的高度需要更新
   return true; //返回成功标志
} //若目标节点存在且被删除，返回true；否则返回false