#pragma once
#include <iostream>
#include "binary_search_tree.hpp"

template<typename T>
struct avl_tree : public binary_search_tree<T> {
public:
   binary_tree_node<T>* insert(const T& e);
   bool remove(const T& e);
};

template<typename T>
binary_tree_node<T>* avl_tree<T>::insert(const T& e) { //将关键码e插入AVL树中
   binary_tree_node<T>* & x = this->search(e);
   if(x) {
      return x;
   }
   binary_tree_node<T>* xx = new binary_tree_node<T>(e, this->hot);
   x = new binary_tree_node<T>(e, this->hot);
   this->size++; 
   for(binary_tree_node<T>* g = this->hot; g; g = g->parent) {//从x之父出发向上，逐层检查各代祖先g
      if(!AvlBalanced(*g)) { //一旦发现g失衡，则（采用“3 + 4”算法）使之复衡，并将子树
         auto v = g->parent;
         if(IsLChild(*g)){            
            v->lc = this->rotateAt(tallerChild(tallerChild(g)));
         } else if(IsRChild(*g)){
            v->rc = this->rotateAt(tallerChild(tallerChild(g)));
         } else {
            this->root = this->rotateAt(tallerChild(tallerChild(g)));
         }
         break; //局部子树复衡后，高度必然复原；其祖先亦必如此，故调整结束
      } else
         this->updateHeight(g);
   }
   return xx; //返回新节点位置
} //无论e是否存在于原树中，总有AVL::insert(e)->data == e

template <typename T> bool avl_tree<T>::remove(const T& e) { //从AVL树中删除关键码e
   binary_tree_node<T>* & x = this->search(e); 
   if(!x) {
      return false;
   }
   removeAt(x,this->hot);
   this->size--;
   for(binary_tree_node<T>* g = this->hot; g; g = g->parent) {
      if(!AvlBalanced(*g)) {
         g = FromParentTo(*g) = this->rotateAt(tallerChild(tallerChild(g))); //原父亲
      }
      this->updateHeight(g); //更新高度（注意：即便g未失衡或已恢复平衡，高度均可能降低）
   } //可能需做Omega(logn)次调整——无论是否做过调整，全树高度均可能降低
   return true; //删除成功
}
