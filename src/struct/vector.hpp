#include<iostream>
#include<stack>

#define DEFAULT_CAPACITY  3 //默认的初始容量（实际应用中可设置为更大）

template <typename T> 
struct Vector { //向量模板类
    int v_size;
    int capacity;
    T* element; //规模、容量、数据区
    Vector(int c = DEFAULT_CAPACITY, int s=0, T v=0){//容量为c、规模为s、所有元素初始为v s<=c
		element = new T[capacity = c]; 
		for(v_size = 0; v_size <s; element[v_size++] = v); 
	}
    Vector(T const* A,int n){//数组整体复制
        copyFrom(A,0,n);
    }
    Vector(T const* A,int beg,int end){//区间
        copyFrom (A,beg,end);
    }
    Vector(Vector<T> const& V){//向量整体复制
        copyFrom(V.element,0,V.v_size);
    }
    Vector(Vector<T> const& V,int beg,int end){ //区间
        copyFrom(V.element,beg,end);
    }
    ~Vector(){// 析构函数
        delete [] element;
    }

    void copyFrom(T const* A,int beg,int end);//复制数组区间A[beg, end);
    void expand(); //空间不足时扩容
    void shrink(); //装填因子过小时压缩
    bool bubble(int beg,int end);//扫描交换
    void bubble_sort_basic(int beg,int end);//起泡排序算法
    void bubble_sort_terminate_earlier(int beg,int end);//起泡排序算法
    void bubble_sort_advanced(int beg,int end);//起泡排序算法
    int max(int beg,int end);//选取最大元素
    void selection_sort(int beg,int end); //选择排序算法
    void merge(int beg,int mi,int end); //归并算法
    void merge_sort(int beg,int end); //归并排序算法
    void heap_sort(int beg,int end); //堆排序（稍后结合完全堆讲解）
    int partition(int beg,int end); //轴点构造算法
    void quick_sort(int beg,int end); //快速排序算法
    void shell_sort(int beg,int end); //希尔排序算法
    int size()const{
        return v_size;
    }
    bool empty()const{
        return !v_size;
    }
    int find (T const& e,int beg,int end)const; //无序向量区间查找
    int find(T const& e)const {
        return find (e,0,v_size); //无序向量整体查找
    } 
    int search(T const& search_elem, int beg, int end)const;//有序向量区间查找
    int search(T const& search_elem)const {//有序向量整体查找
        return (0 >= v_size) ? -1 : search(search_elem, 0 , v_size); 
    }
    T& operator[] (int r);//重载下标操作符，可以类似于数组形式引用各元素
    const T& operator[] ( int r ) const ;//仅限于做右值的重载版本
    Vector<T> & operator= (Vector<T> const&);//重载赋值操作符，以便直接克隆向量
    T remove(int r);//删除秩为r的元素
    int remove(int beg,int end);//删除秩在区间[beg, end)之内的元素
    int insert (int r,T const& e);//插入元素
    int insert (T const& e) { 
        return insert(v_size,e);
    } 
    void unsort(int beg,int end);//对[beg, end)置乱
    void unsort() { //整体置乱
        unsort(0,v_size); 
    } 
    int deduplicate();//无序去重
    int uniquify(); //有序去重
    void traverse(void (*)(T&));//遍历（使用函数指针，只读或局部性修改）
    template<typename VST> void traverse(VST&); //遍历（使用函数对象，可全局性修改）
  
};
template<typename T> //元素类型
void Vector<T>::copyFrom(T const* A, int beg, int end) { //以数组区间A[beg, hi)为蓝本复制向量
    element = new T[capacity = 2 * (end - beg)];
    v_size = 0; //分配空间，规模清零
    while(beg < end){ //A[beg, hi)内的元素逐一
        element[v_size++] = A[beg++]; //复制至_elem[0, hi - beg)
    }
}

template<typename T>
void Vector<T>::expand(){ //向量空间不足时扩容
    if(v_size < capacity){
        return; //尚未满员时，不必扩容
    }
    if(capacity < DEFAULT_CAPACITY){
        capacity = DEFAULT_CAPACITY; //不低于最小容量
    }
    T* oldelem = element;
    element = new T[capacity <<= 1]; //容量加倍
    for(int i=0; i<v_size; ++i){
        element[i] = oldelem[i]; //复制原向量内容（T为基本类型，或已重载赋值操作符'='）
    }
   delete []oldelem; //释放原空间
}

template<typename T>
void Vector<T>::shrink() {//装填因子过小时压缩向量所占空间
    if(capacity < DEFAULT_CAPACITY << 1){
        return;
    } //不致收缩到DEFAULT_CAPACITY以下
    if(v_size << 2 > capacity) {
        return; //以25%为界
    }
    T* old_elem = element;
    element = new T[capacity >>= 1]; //容量减半
    for(int i=0; i<v_size; ++i){
        element[i] = old_elem[i]; //复制原向量内容
    }
    delete [] old_elem; //释放原空间
}

template <typename T> //向量的起泡排序（基本版）
void Vector<T>::bubble_sort_basic(int beg, int end) { //assert: 0 <= beg < hi <= size
    for(; beg < end ; --end) { //反复起泡扫描
        for(int i=beg; i<end; ++i) {//逐个检查相邻元素
            if(element[i] > element[i+1]){ //若逆序，则
                std::swap(element[i], element[i+1]); //经交换使局部有序
            }
        }
    }
}

template <typename T> //向量的起泡排序（提前终止版）
void Vector<T>::bubble_sort_terminate_earlier(int beg, int end){ 
    for(bool sorted = true; sorted &&  beg < end; --end) {
        sorted = false;
        for(int i=beg; i<end; ++i){
            if(element[i] > element[i + 1]) {//若逆序，则
                std::swap(element[i], element[i+1]);
                sorted = true;//经交换使局部有序——当然，至此还不能确定已整体有序
            }
        }
    }
}

template<typename T> //向量的起泡排序（跳跃版）
void Vector<T>::bubble_sort_advanced(int beg, int end){
    for(int last = end; beg < end; end = last){
        for(int i=last=beg; i<end; ++i) {
            if(element[i] > element[i + 1]) { //若逆序，则
                std::swap(element[last = i], element[i + 1]); //经交换使局部有序
            }
        }
    }
}

template<typename T>
int Vector<T>::max(int beg, int end) { //在[lo, hi]内找出最大者
    int mx=end;
    while(beg<end--){ //逆向扫描
    if(element[end]>element[mx]) //且严格比较
        mx=end; //故能在max有多个时保证后者优先，进而保证selectionSort稳定 
    }
    return mx;
}

template<typename T> //向量选择排序
void Vector<T>::selection_sort(int beg, int end){ //0 <= lo < hi <= size
   while (beg<--end)
      swap(element[max(beg,end)],element[end]); //将[hi]与[lo, hi]中的最大者交换
}

template<typename T> //有序向量（区间）的归并
void Vector<T>::merge(int beg, int mid, int end){ //各自有序的子向量[lo, mi)和[mi, hi)
    T* A = element + beg; //合并后的向量A[0, hi - lo) = _elem[lo, hi)
    int lb = mid - beg;
    T* B = new T[lb]; //前子向量B[0, lb) = _elem[lo, mi)
    for(int i=0; i<lb; ++i){
        B[i] = A[i]; //复制前子向量
    }
    int lc = end - mid;
    T* C = element + mid; //后子向量C[0, lc) = _elem[mi, hi)
    for(int i=0, j=0, k=0; j<lb;) //归并：反复从B和C首元素中取出更小者
        A[i++] = ( lc <= k || B[j] <= C[k] ) ? B[j++] : C[k++]; //将其归入A中
    delete [] B; //释放临时空间B
}

template <typename T> //向量归并排序
void Vector<T>::merge_sort(int beg, int end){ //0 <= lo < hi <= size
    if(end - beg<2){
         return; //单元素区间自然有序，否则...
    }
    int mid = (beg + end) / 2; //以中点为界
    merge_sort(beg, mid);
    merge_sort(mid, end); //分别排序
    merge(beg, mid, end); //归并
}

template<typename T>
void Vector<T>::heap_sort(int beg, int end){ //0 <= lo < hi <= size
    T* A =element+beg;
    int n= beg-end;
    heapify(A,n); //将待排序区间建成一个完全二叉堆，O(n)
    while(0<--n){ //反复地摘除最大元并归入已排序的后缀，直至堆空
        swap(A[0],A[n]);
        percolateDown(A,n,0);
   } //堆顶与末元素对换，再下滤
}

template <typename T> //轴点构造算法：通过调整元素位置构造区间[lo, hi)的轴点，并返回其秩
int Vector<T>::partition (int beg, int end) { //DUP版：可优化处理多个关键码雷同的退化情况
   swap(element[beg], element[beg+rand()%(end-beg)]); //任选一个元素与首元素交换
   end--;
   T pivot = element[beg]; //以首元素为候选轴点——经以上交换，等效于随机选取
   while(beg < end) { //从向量的两端交替地向中间扫描
        while(beg < end){
            if(pivot < element[end]) {//在大于pivot的前提下
                end--;  //向左拓展右端子向量
            }
            else { //直至遇到不大于pivot者
                element[beg++] = element[end];
                break; 
            } //将其归入左端子向量
        }
        while(beg < end){
            if(element[beg] < pivot ) {//在小于pivot的前提下
                beg++; //向右拓展左端子向量
            }
            else { //直至遇到不小于pivot者
                element[end--] = element[beg];
                break; 
            } //将其归入右端子向量
        }
   } //assert: beg == hi
   element[beg] = pivot; //将备份的轴点记录置于前、后子向量之间
   return beg; //返回轴点的秩
}

// template <typename T> //轴点构造算法：通过调整元素位置构造区间[lo, hi)的轴点，并返回其秩
// Rank Vector<T>::partition ( Rank lo, Rank hi ) { //LGU版
//    swap ( _elem[lo], _elem[ lo + rand() % ( hi - lo ) ] ); //任选一个元素与首元素交换
//    T pivot = _elem[lo]; //以首元素为候选轴点——经以上交换，等效于随机选取
//    int mi = lo;
//    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//    //   ---- L < [lo] ----- ] ----- [lo] <= G --- ] [ ----- Unknown -------
//    // X x . . . . . . . . . x . . . . . . . . . . . x . . . . . . . . . . x X
//    // |                     |                       |                       |
//    // lo (pivot candidate)  mi                      k                       hi
//    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//    for ( int k = lo + 1; k < hi; k++ ) //自左向右扫描
//       if ( _elem[k] < pivot ) //若当前元素_elem[k]小于pivot，则
//          swap ( _elem[++mi], _elem[k] ); //将_elem[k]交换至原mi之后，使L子序列向右扩展
//    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//    //   --------- L < [lo] ---------- ] ------------- [lo] <= G ----------]
//    // X x . . . . . . . . . . . . . . x . . . . . . . . . . . . . . . . . x X
//    // |                               |                                     |
//    // lo (pivot candidate)            mi                                    hi
//    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//    swap ( _elem[lo], _elem[mi] ); //候选轴点归位
//    return mi; //返回轴点的秩
// }

// template <typename T> //轴点构造算法：通过调整元素位置构造区间[lo, hi)的轴点，并返回其秩
// Rank Vector<T>::partition ( Rank lo, Rank hi ) { //LUG版本：基本形式
//    swap ( _elem[lo], _elem[ lo + rand() % ( hi - lo ) ] ); //任选一个元素与首元素交换
//    hi--; T pivot = _elem[lo]; //以首元素为候选轴点——经以上交换，等效于随机选取
//    while ( lo < hi ) { //从向量的两端交替地向中间扫描
//       while ( ( lo < hi ) && ( pivot <= _elem[hi] ) ) //在不小于pivot的前提下
//          hi--; //向左拓展右端子向量
//       _elem[lo] = _elem[hi]; //小于pivot者归入左侧子序列
//       while ( ( lo < hi ) && ( _elem[lo] <= pivot ) ) //在不大于pivot的前提下
//          lo++; //向右拓展左端子向量
//       _elem[hi] = _elem[lo]; //大于pivot者归入右侧子序列
//    } //assert: lo == hi
//    _elem[lo] = pivot; //将备份的轴点记录置于前、后子向量之间
//    return lo; //返回轴点的秩
// }

// template <typename T> //轴点构造算法：通过调整元素位置构造区间[lo, hi)的轴点，并返回其秩
// Rank Vector<T>::partition ( Rank lo, Rank hi ) { //LUG1版：与LUG版等价，类似于与DUP版等价的DUP1版
//    swap ( _elem[lo], _elem[lo + rand() % ( hi-lo ) ] ); //任选一个元素与首元素交换
//    hi--; T pivot = _elem[lo]; //以首元素为候选轴点——经以上交换，等效于随机选取
//    while ( lo < hi ) { //从向量的两端交替地向中间扫描
//       while ( ( lo < hi ) && ( pivot <= _elem[hi] ) ) //在不小于pivot的前提下
//          hi--; //向左拓展右端子向量
//       if ( lo < hi ) _elem[lo++] = _elem[hi]; //小于pivot者归入左侧子向量
//       while ( ( lo < hi ) && ( _elem[lo] <= pivot ) ) //在保证不大于pivot的前提下
//          lo++; //向右拓展左端子向量
//       if ( lo < hi ) _elem[hi--] = _elem[lo]; //大于pivot者归入右侧子向量
//    } //assert: lo == hi
//    _elem[lo] = pivot; //将备份的轴点记录置于前、后子向量之间
//    return lo; //返回轴点的秩
// }

// template <typename T> //轴点构造算法：通过调整元素位置构造区间[lo, hi)的轴点，并返回其秩
// Rank Vector<T>::partition ( Rank lo, Rank hi ) { //DUP1版：与DUP版等价，类似于与LUG版等价的LUG1版
//    swap ( _elem[lo], _elem[ lo + rand() % ( hi - lo ) ] ); //任选一个元素与首元素交换
//    hi--; T pivot = _elem[lo]; //以首元素为候选轴点——经以上交换，等效于随机选取
//    while ( lo < hi ) { //从向量的两端交替地向中间扫描
//       while ( ( lo < hi ) && ( pivot < _elem[hi] ) ) //在大于pivot的前提下
//          hi--; //向左拓展右端子向量
//       if ( lo < hi ) _elem[lo++] = _elem[hi]; //不大于pivot者归入左端子向量
//       while ( ( lo < hi ) && ( _elem[lo] < pivot ) ) //在小于pivot的前提下
//          lo++; //向右拓展左端子向量
//       if ( lo < hi ) _elem[hi--] = _elem[lo]; //不小于pivot者归入右端子向量
//    } //assert: lo == hi
//    _elem[lo] = pivot; //将备份的轴点记录置于前、后子向量之间
//    return lo; //返回轴点的秩
// }

void Put(std::stack<int>& task,int s,int t){
    if(t - s > 1) {
        task.push(s);
        task.push(t);
    }
}

void Get(std::stack<int>& task,int& s,int& t){
    t = task.top();
    task.pop();
    s = task.top();
    task.pop();
}

// template <typename T> //向量快速排序
// void Vector<T>::quickSort(int beg, int end) { //0 <= beg < end <= size
//    std::stack<int> Task; 
//    Put(Task, beg, end);
//    while (!Task.empty()) {
//         Get( Task, beg, end );
//         /* DSA */ //printf ( "\tQUICKsort: " ); for ( Rank i = 0; i < Task.size(); i+=2 ) printf ( " " ); printf ( " [%3d, %3d)\n", beg, end );
//         int mi = partition( beg, end ); //在[beg, end)内构造轴点
//         if(mi - beg < end - mi) {
//             Put(Task, mi+1, end);
//             Put(Task, beg, mi);
//         } else {
//             Put(Task, beg, mi);
//             Put(Task, mi+1, end);
//       }
//    } //大任务优先入栈（小任务优先出栈执行），可保证递归深度（空间成本）不过O(logn)
// }

template <typename T> //向量快速排序
void Vector<T>::quick_sort(int beg, int end) { //0 <= beg < end <= size
   if(end - beg < 2) {
        return;
    }//单元素区间自然有序，否则...
   int mi = partition(beg, end); //在[beg, end)内构造轴点
   quick_sort(beg, mi); //对前缀递归排序
   quick_sort(mi + 1, end); //对后缀递归排序
}

template <typename T> //向量希尔排序
void Vector<T>::shell_sort(int beg, int end) { //0 <= beg < end <= size <= 2^30
    for(int d = 0x3FFFFFFF; 0 < d; d >>= 1) //PS Sequence: { 1, 3, 7, 15, ..., 1073741823 }
        for(int j = beg + d; j < end; j++) { //for each j in [beg+d, end)
            T x = element[j];
            int i = j - d;
            while(beg <= i && element[i] > x) { 
                element[i + d] = element[i];
                i -= d; 
            }
        element[i + d] = x; //insert [j] into its subsequence
    }
}

template<typename T> //将e作为秩为r元素插入
int Vector<T>::insert(int r, T const& e) { //assert: 0 <= r <= size
    expand(); //若有必要，扩容
    for(int i=v_size;i>r;--i){
        element[i]=element[i-1];
    } //自后向前，后继元素顺次后移一个单元
    element[r]=e; 
    v_size++; //置入新元素并更新容量
    return r; //返回秩
}

template<typename T> //无序向量的顺序查找：返回最后一个元素e的位置；失败时，返回lo - 1
int Vector<T>::find(T const& e,int beg,int end)const { //assert: 0 <= lo < hi <= _size
   while((beg<end--)&&(e!=element[end])); //从后向前，顺序查找
   return end; //若hi < lo，则意味着失败；否则hi即命中元素的秩
}



template<typename T> static int binSearch(T* S,T const& e,int beg,int end) { // 二分查找算法（版本A）：在有序向量的区间[beg, end)内查找元素e，0 <= beg <= end <= _size
    while(beg < end) { //每步迭代可能要做两次比较判断，有三个分支
        int mi = (beg + end) >> 1; //以中点为轴点（区间宽度的折半，等效于宽度之数值表示的右移）
        if(e < S[mi]){
            end = mi;
        } //深入前半段[beg, mi)继续查找
        else if(S[mi] < e) {
            beg = mi + 1;
        } //深入后半段(mi, end)继续查找
        else {
            return mi;
        }           
    } 
    return -1; 
} //有多个命中元素时，不能保证返回秩最大者；查找失败时，简单地返回-1，而不能指示失败的位置

template <typename T> static int binSearch_type_b(T* S, T const& e, int beg, int end) { // 二分查找算法（版本C）：在有序向量的区间[beg, hi)内查找元素e，0 <= beg <= hi <= _size
   while(beg < end) { //每步迭代仅需做一次比较判断，有两个分支
      int mi = (beg + end) >> 1; //以中点为轴点（区间宽度的折半，等效于宽度之数值表示的右移）
      (e < S[mi]) ? end = mi : beg = mi + 1; //经比较后确定深入[beg, mi)或(mi, end)
   } //成功查找不能提前终止
   return beg - 1; //循环结束时，beg为大于e的元素的最小秩，故beg - 1即不大于e的元素的最大秩
}


template <typename T> static int binSearch_type_c ( T* S, T const& e, int beg, int end ) { // 二分查找算法（版本B）：在有序向量的区间[lo, hi)内查找元素e，0 <= lo < hi <= _size
   while (1 < end - beg) { //每步迭代仅需做一次比较判断，有两个分支；成功查找不能提前终止
      int mi = (beg + end) >> 1; //以中点为轴点（区间宽度的折半，等效于宽度之数值表示的右移）
      (e < S[mi]) ? end = mi : beg = mi; //经比较后确定深入[beg, mi)或[mi, end)
   } //出口时end = beg + 1，查找区间仅含一个元素A[beg]
   return e < S[beg] ? beg - 1 : beg; //返回位置，总是不超过e的最大者
}

template<typename T> //在有序向量的区间[lo, end)内，确定不大于e的最后一个节点的秩
int Vector<T>::search(T const& search_elem, int beg, int end)const { //assert: 0 <= beg < end <= _size
   return (rand() % 2) ? binSearch(element, search_elem, beg, end) : fibSearch(element, search_elem, beg, end);//按各50%的概率随机使用二分查找或Fibonacci查找
}

struct Fib { 
    int f, g; //f = fib(k - 1), g = fib(k)。均为int型，很快就会数值溢出
    Fib(int n) { //初始化为不小于n的最小Fibonacci项
        f = 1;
        g = 0; 
        while(g < n) {
            next(); 
        }
    } //fib(-1), fib(0)，O(log_phi(n))时间
   int get()  { 
       return g;
    } //获取当前Fibonacci项，O(1)时间
   int next() { 
       g += f;
       f = g - f;
       return g;
    } //转至下一Fibonacci项，O(1)时间
   int prev() { 
       f = g - f;
       g -= f;
       return g;
    } //转至上一Fibonacci项，O(1)时间
};

template <typename T> static int fibSearch ( T* S, T const& e, int beg, int end ) {
    for (Fib fib(end - beg); beg < end; ) {  //Fib数列制表备查；此后每步迭代仅一次比较、两个分支
        while (end - beg < fib.get()) {
            fib.prev();
        }//自后向前顺序查找（分摊O(1)）
        int mi = beg + fib.get() - 1; //确定形如Fib(k) - 1的轴点
        if(e < S[mi]) {
            end = mi;
        } //深入前半段[beg, mi)继续查找
        else if(S[mi] < e) {
            beg = mi + 1;
        } //深入后半段(mi, end)继续查找
        else {
            return mi;
        }//在mi处命中
   }
   return -1;
} //有多个命中元素时，不能保证返回秩最大者；失败时，简单地返回-1，而不能指示失败的位置

template <typename T> static int fibSearch_type_B ( T* S, T const& e, int beg, int end ) {
    for(Fib fib(end - beg); beg < end;) { //Fib数列制表备查
        while(end - beg < fib.get()) {
            fib.prev();
        } //自后向前顺序查找（分摊O(1)）
        int mi = beg + fib.get() - 1; //确定形如Fib(k) - 1的轴点
        (e < S[mi]) ?end = mi : beg = mi+ 1; //比较后确定深入前半段[beg, mi)或后半段(mi, hi)
    } //成功查找不能提前终止
    return --beg; //循环结束时，lo为大于e的元素的最小秩，故lo - 1即不大于e的元素的最大秩
} //有多个命中元素时，总能保证返回最秩最大者；查找失败时，能够返回失败的位置

template<typename T> 
Vector<T>& Vector<T>::operator= (Vector<T> const& V) { //深复制
   if (element) delete [] element; //释放原有内容
   copyFrom(V.element,0,V.size() ); //整体复制
   return *this; //返回当前对象的引用，以便链式赋值
}

template<typename T>
T& Vector<T>::operator[] (int r) { //重载下标操作符
    return element[r];
} // assert: 0 <= r < v_size

template<typename T>
const T & Vector<T>::operator[] (int r)const{ //仅限于做右值
    return element[r];
} // assert: 0 <= r < v_size


template <typename T> 
T Vector<T>::remove (int r ) { //删除向量中秩为r的元素，0 <= r < size
   T e = element[r]; //备份被删除元素
   remove(r, r + 1); //调用区间删除算法，等效于对区间[r, r + 1)的删除
   return e; //返回被删除元素
}

template <typename T>
int Vector<T>::remove(int beg, int end) { //删除区间[beg, end)
    if ( beg == end ) {
        return 0;
    } //出于效率考虑，单独处理退化情况，比如remove(0, 0)
    while (end < v_size) {
       element[beg++] = element[end++];
    } //[end, _size)顺次前移end - beg个单元
    v_size = beg; //更新规模，直接丢弃尾部[beg, _size = end)区间
    shrink(); //若有必要，则缩容
    return end - beg; //返回被删除元素的数目
}

template <typename T>
void Vector<T>::unsort (int beg, int end) { //等概率随机置乱区间[beg, end)
   T* V = element + beg; //将子向量element[beg, end)视作另一向量V[0, end - beg)
   for(int i = end - beg; i > 0; i--) //自后向前
        swap(V[i - 1], V[rand() % i]); //将V[i - 1]与V[0, i)中某一元素随机交换
}

template<typename T>
int Vector<T>::deduplicate() { //删除无序向量中重复元素（高效版）
   int old_size=v_size; //记录原规模
   int i=1; //从element[1]开始
   while(i<v_size) {//自前向后逐一考查各元素_elem[i]
      (find(element[i],0,i)<0) ? i++:remove(i); 
      //在其前缀中寻找与之雷同者（至多一个）
    }//若无雷同则继续考查其后继，否则删除雷同者
   return old_size-v_size; //向量规模变化量，即被删除元素总数
}


// template<typename T>
// int Vector<T>::deduplicate() { //删除无序向量中重复元素（繁琐版）
//     int old_size=v_size; //记录原规模
//     int i=-1; //从最前端开始
//     while(v_size-1>++i) { //从前向后，逐一
//         int j=i+1; //assert: _elem[0, i]中不含重复元
//         while(j<v_size){
//             if (element[i]== element[j]){
//                 remove(j);//若雷同，则删除后者
//             }
//         }
//         else j++; //并继续扫描
//    }
//    return oldd_size-v_size; //向量规模变化量，即被删除元素总数
// }


template <typename T>
int Vector<T>::uniquify() { //有序向量重复元素剔除算法（高效版）
   int i = 0, j = 0; //各对互异“相邻”元素的秩
   while ( ++j < v_size ) //逐一扫描，直至末元素
      if ( element[i] != element[j] ) //跳过雷同者
         element[++i] = element[j]; //发现不同元素时，向前移至紧邻于前者右侧
   v_size = ++i; shrink(); //直接截除尾部多余元素
   return j - i; //向量规模变化量，即被删除元素总数
}

template <typename T>
void Vector<T>::traverse(void(*visit)(T&)) { //借助函数指针机制
    for ( int i = 0; i < v_size; i++ ) {
        visit ( element[i] );
    }
} //遍历向量

template<typename T>
template <typename VST> //元素类型、操作器
void Vector<T>::traverse ( VST& visit ) { //借助函数对象机制
    for(int i = 0; i < v_size; i++) {
        visit( element[i] );
    }
} //遍历向量

int main(){
    Vector<int> v1;
}