#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <stack>
#include "avl_tree.hpp"
#include <map>
#include <stack>

void sortt(std::vector<int>& v, int beg, int end) { //assert: 0 <= beg < hi <= size
    for(; beg < end ; end--) { //反复起泡扫描
        std::cout<<beg<<end<<std::endl;
		for(int i=beg; i<end; ++i) {//逐个检查相邻元素
            if(v[i] > v[i+1]){ //若逆序，则
                std::swap(v[i], v[i+1]); //经交换使局部有序
            }
        }
    }
}

void bubble_sort_terminate_earlier(std::vector<int>& v, int beg, int end) {
    for(bool sorted = true; sorted &&  beg < end; --end) {
        std::cout<<beg<<end<<std::endl;
		sorted = false;
        for(int i=beg; i<end; ++i){
            if(v[i] > v[i + 1]) {//若逆序，则
                std::swap(v[i], v[i+1]);
                sorted = true;//经交换使局部有序——当然，至此还不能确定已整体有序
            }
        }
    }
}
void tes(int x,int y) {
    std::cout<< x << y <<std::endl;
}

// void swap_1(int* p1,int* p2){

//     int temp = *p1;
//     *p1 = *p2;
//     *p2 = temp;
//     printf("交换中：a=%d,b=%d \n",*p1,*p2);
//     printf("交换中(地址)：p1=%d \n",p1);
//     printf("交换中(地址)：p2=%d \n",p2);
// }

// void swap_2(int* p1,int* p2){

//     int* temp=p1;
//     p1=p2;
//     p2=temp;
//     printf("交换中：a=%d,b=%d \n",*p1,*p2);
//     printf("交换中(地址)：p1=%d \n",p1);
//     printf("交换中(地址)：p2=%d \n",p2);
// }

struct node { 
public:
    int val;
    node* pre;
    node* next;
};

// struct All {
// public:
//     node* n1;
//     node* next;
//     node* pre;
//     node* next;
// }
// node*& get_node(node* n) {
//     node* & val = n->next;
//     return val;
// }

void chain(node* a, node* b, node* c, node* d) {
    a->pre = nullptr;
    a->next = b;
    b->pre = a;
    b->next = c;
    c->pre = b;
    c->next = d;
    d->pre = c;
    d->next = nullptr ;
}

void offset(node*& n) {
    n = n->next;
}
template<typename T>
void offset(binary_tree_node<T>*& n) {
    n = n->lc;
}

void sift_down(int arr[], int start, int end) {
  // 计算父结点和子结点的下标
    int parent = start;
    int child = parent * 2 + 1;
    while (child <= end) {
    if (child + 1 <= end && arr[child] < arr[child + 1]) {
        child++;
    }
    if (arr[parent] >= arr[child]){
        return;
    } else {  // 否则交换父子内容，子结点再和孙结点比较
        std::swap(arr[parent], arr[child]);
        parent = child;
        child = parent * 2 + 1;
    }
  }
}

void heap_sort(int arr[], int len) {
    for (int i = (len - 1 - 1) / 2; i >= 0; i--) {
        sift_down(arr, i, len - 1);
        for(int  i = 0; i < len; ++i) {
            std::cout<<arr[i];
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    for (int i = len - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        sift_down(arr, 0, i - 1);

        for(int  i = 0; i < len; ++i) {
            std::cout<<arr[i];
        }
        std::cout<<std::endl;
    }
}
void f(int& x){
    std::cout<<"left"<<std::endl;
}
void f(int&& x){
    std::cout<<"right"<<std::endl;
}
void check(int&){
    std::cout<<"left"<<std::endl;
}

void check(int&&){
    std::cout<<"right"<<std::endl;
}
    
template <typename T>
void print(T&& v){
    check(std::forward<T>(v));
    // check(v);
}
int g(){
    int x=9;
    return x;
}

int main(){
	// std::vector<int> v_1 {1,2,3,77,44,33,22};
	// bubble_sort_terminate_earlier(v_1,0,6);
	// for(auto i : v_1) {
	// 	std::cout<<i<<std::endl;
	// }
	// int i=0;
	// int j=1;
	// int k=2;
	// int ii = ( j = k);
	// std::cout<<i<<j<<k<<std::endl;
	// std::cout<<ii<<std::endl;
    // int n = 10;
    // while( 0 <= n--) {
    //     std::cout<< n <<std::endl;
    // }
    // for(int i = 10; i >=0; i--){
    //     std::cout<< i <<std::endl;
    // }
    // std::stack<int> stk;
    // stk.push(1);
    // stk.push(2);
    // stk.push(3);
    // stk.push(4);
    // stk.push(5);
    // tes(stk.top(),stk.top());
    // int val = 1;
    // int val2 = 1;
    // int* x = &val2;
    // int* y = &val;
    // if( y = x ) {
    //     std::cout<< "shit" <<std::endl;
    // }
    // B* val = new B();
    // node* n1 = new node();
    // n1->val = 1;
    // node* n2 = new node();
    // n2->val = 2;
    // node* n3 = new node();
    // n3->val = 3;
    // node* n4 = new node();
    // n4->val = 4;
    // chain(n1, n2, n3, n4);
    // auto a = &(n1->next);
    // offset(n1->next);

    // binary_tree_node<int>* n1 = new binary_tree_node<int>();
    // // binary_tree_node<int>* n2 = new binary_tree_node<int>(a,n1);
    // n1->data = 4;
    // n1->insert_as_lc(2);
    // n1->insert_as_rc(6);
    // n1->lc->insert_as_lc(1);
    // n1->lc->insert_as_rc(3);  
    // n1->rc->insert_as_lc(5);
    // n1->rc->insert_as_rc(7);
    // offset(n1->lc);
    // trav_in_iter_1(n1);


    // avl_tree<int>* t = new avl_tree<int>();
    // t->insert_as_root(5);
    // t->insert_as_lc(t->root, 3);
    // t->insert_as_rc(t->root, 7);
    // t->insert_as_lc(t->root->lc, 2);
    // t->insert_as_rc(t->root->lc, 4);
    // t->insert_as_lc(t->root->rc, 6);
    // t->insert_as_rc(t->root->rc, 8);
    // t->insert_as_lc(t->root->lc->lc, 1);
    // t->insert_as_root(2);
    // t->insert_as_lc(t->root, 1);
    // std::cout<<t->size<<std::endl;
    // trav_in_iter_1(t->root);
    // auto x = t->insert(0);
    // std::cout<<x<<std::endl;
    // std::cout<<std::endl;
    // trav_in_iter_1(t->root);
    // std::map<int,int> map_int;
    // // for(int i = 0; i < 10; ++i){
    // //     map_int[i] += (i+1);
    // // }
    // map_int.emplace(1,2);
    // map_int.emplace(1,3);
    // map_int.emplace(1,10);
    // for(auto m : map_int) {
    //     std::cout<< m.first << " " << m.second << std::endl;
    // }
    // std::vector<int> x(10,0);
    // for(auto& val : x) {
    //     std::cout<< val <<std::endl;
    // }
    // std::vector<int> a {2,2,3,3,3,4};
    // std::map<int, int> sum;
    //     for(auto i : a) {
    //         sum[i] += i;
    //     }
    // for(auto& i : sum) {
    //     std::cout<< i.first << i.second <<std::endl;
    // }
    // auto i = sum.begin();
    // auto j = ++sum.begin();
    // std::cout<< i->first << j->first <<std::endl;
    // std::stack<int> s;
    // std::cout<< s.top() <<std::endl;
//     std::vector<int> x {1,2,3};
//     int i = 1;
//     std::cout<<x[i]<< x[++i] <<std::endl;
// }
    // std::queue<int> re;
    // auto res = re.front();
    // std::cout<<res<<std::endl;
    // int a[6] = {3,2,1,5,6,4};
    // heap_sort(a, 6);
    // double v = (1 + 2) / 2;
    // std::cout<<v<<std::endl;
    // int x=0;
    // print(x);
    // print(std::move(x));
    int z = 0;
    f(std::forward<decltype(0)>(z));
    f(std::forward<decltype((0))>(z));
    f(std::forward<decltype(g())>(z));
    f(std::forward<decltype((g()))>(z));

    person.push_back(std::move(p)); 
    person.emplace_back(move(p));
}