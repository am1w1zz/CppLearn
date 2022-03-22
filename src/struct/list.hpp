#include<iostream>

template <typename T>
struct ListNode {
    T data;
    ListNode<T>* pred;
    ListNode<T>* succ;
    ListNode() {}
    ListNode(T e, ListNode<T>* p = NULL, ListNode<T>* s = NULL): data(e), pred(p), succ(s) {};
    ListNode<T>* insertAsPred(T const& e) {
        ListNode<T>* x = new ListNode(e, pred, this);
        pred->succ = x;
        pred = x;
        return x;
   }
    ListNode<T>* insertAsSucc(T const& e) {
        ListNode<T>* x = new ListNode(e, this, succ);
        succ->pred = x;
        succ = x;
        return x;
    }
};

template <typename T>
class List {

private:
    int _size;
    ListNode<T>* header;
    ListNode<T>* trailer;

protected:
    void init() {
        header = new ListNode<T>; //创建头哨兵节点
        header->succ = trailer; header->pred = NULL;
        trailer = new ListNode<T>; //创建尾哨兵节点
        trailer->pred = header; trailer->succ = NULL;
        _size = 0; //记录规模
    }

    int clear() {
        int oldSize = _size;
        while(0 < _size) {
            remove(header->succ); //反复删除首节点，直至列表变空
        }
        return oldSize;
    }

    void copyNodes(ListNode<T>* p, int n) {
        init();
        while(n--) { 
            insertAsLast(p->data);
            p = p->succ;
        } //将起自p的n项依次作为末节点插入
    }

    ListNode<T>* merge(ListNode<T>* p, int n, List<T> & L, ListNode<T>* q, int m) {
        ListNode<T>* pp = p->pred; //归并之后p可能不再指向首节点，故需先记忆，以便在返回前更新
        while((0 < m) && (q != p)) {//q尚未出界（或在mergeSort()中，p尚未出界）之前
            if ((0 < n) && (p->data <= q->data)) { //若p尚未出界且v(p) <= v(q)，则
                p = p->succ;
                n--;//p直接后移，即完成归入
            }
            else {//否则，将q转移至p之前，以完成归入
                insertB(p, L.remove((q = q->succ)->pred));
                m--; 
            }
        }
        return pp->succ; //更新的首节点
    }

    void mergeSort(ListNode<T>* & p, int n) {
        if(n < 2) {
            return;
        } //若待排序范围已足够小，则直接返回；否则...
        int m = n >> 1; //以中点为界
        ListNode<T>* q = p;
        for(int i = 0; i < m; i++) {
            q = q->succ;
        } //找到后子列表起点
        mergeSort(p, m);
        mergeSort(q, n - m); //前、后子列表各分别排序
        p = merge(p, m, *this, q, n - m); //归并
    } //注意：排序后，p依然指向归并后区间的（新）起点

    void selectionSort(ListNode<T>* p, int n) {
        ListNode<T>* head = p->pred;
        ListNode<T>* tail = p;
        for(int i = 0; i < n; i++) {
            tail = tail->succ;
        } //待排序区间为(head, tail)
        while(1 < n) { //在至少还剩两个节点之前，在待排序区间内
            ListNode<T>* max = selectMax(head->succ, n); //找出最大者（歧义时后者优先）
            insertB(tail, remove(max)); //将其移至无序区间末尾（作为有序区间新的首元素）
            tail = tail->pred;
            n--;
        }
    }

    void insertionSort(ListNode<T>* p, int n) {
        for(int r = 0; r < n; r++) { //逐一为各节点
            insertA(search(p->data, r, p), p->data); //查找适当的位置并插入
            p = p->succ;
            remove(p->pred); //转向下一节点
        }
    }

    void radixSort(ListNode<T>* p, int n) {
        ListNode<T>* head = p->pred;
        ListNode<T>* tail = p;
        for(int i = 0; i < n; i++) {
            tail = tail->succ;
        } //待排序区间为(head, tail)
        for(unsigned int radixBit = 0x1; radixBit && (p = head); radixBit <<= 1 ){
            for ( int i = 0; i < n; i++ ) {//根据当前基数位，将所有节点
                radixBit & (unsigned int)(p->succ->data) ? insertB(tail, remove(p->succ)) : p = p->succ;  //分拣为后缀（1）与前缀（0）
            }
        }
    }

public:
    List() {
        init();
    }
    List(List<T> const& L) {
        copyNodes(L.first(), L._size);
    }
    List(List<T> const& L, int r, int n) {
        ListNode<T>* p = L.first();
        while (0 < r--) {
            p = p->succ;
        }
        copyNodes(p, n);
    }
    List(ListNode<T>* p, int n) {
        copyNodes(p, n);
    }
    ~List() {
        clear();
        delete header;
        delete trailer; 
    }

    int size() const {
        return _size;
    }
    bool empty() const {
        return _size <= 0;
    }
    T& operator[] (int r) const {
        ListNode<T>* p = first(); //从首节点出发
        while(0 < r--) {
            p = p->succ;
        } //顺数第r个节点即是
        return p->data; //目标节点，返回其中所存元素
    }
    ListNode<T>* first() const {
        return header->succ;
    }
    ListNode<T>* last() const {
        return trailer->pred;
    }
    bool valid (ListNode<T>* p) {
        return p && (trailer != p) && (header != p);
    }
    ListNode<T>* find (T const& e) const {
        return find ( e, _size, trailer );
    }
    ListNode<T>* find (T const& e, int n, ListNode<T>* p ) const {
        while (0 < n--) {
            if(e == ( p = p->pred )->data) {
                return p;
            } //逐个比对，直至命中或范围越界
        }
        return NULL; //p越出左边界意味着区间内不含e，查找失败
    }
    ListNode<T>* search (T const& e) const {
        return search(e, _size, trailer);
    }
    ListNode<T>* search (T const& e, int n, ListNode<T>* p) const { 
        do {
            p = p->pred;
            n--;  //从右向左
         
        } while ((-1 < n) && (e < p->data));
        return p; //返回查找终止的位置
    }
    ListNode<T>* selectMax ( ListNode<T>* p, int n ) {
        ListNode<T>* max = p; //最大者暂定为首节点p
        for(ListNode<T>* cur = p; 1 < n; n--) {//从首节点p出发，将后续节点逐一与max比较
            if(!lt ((cur = cur->succ)->data, max->data)){ //若当前元素不小于max，则
                max = cur; //更新最大元素位置记录
            }
        }
        return max;
    }
    ListNode<T>* selectMax() {
        return selectMax(header->succ, _size);
    }
    ListNode<T>* insertAsFirst(T const& e) {
        _size++;
        return header->insertAsSucc(e);
    }
    ListNode<T>* insertAsLast(T const& e) {
        _size++;
        return trailer->insertAsPred(e);
    }
    ListNode<T>* insertA (ListNode<T>* p, T const& e) {
        _size++; 
        return p->insertAsSucc(e);//e当作p的后继插入（After）
    }
    ListNode<T>* insertB ( ListNode<T>* p, T const& e ) {
        _size++;
        return p->insertAsPred(e); //e当作p的前驱插入（Before）
    }
    T remove ( ListNode<T>* p ) {
        T e = p->data; //备份待删除节点的数值（假定T类型可直接赋值）
        p->pred->succ = p->succ;
        p->succ->pred = p->pred; //后继、前驱
        delete p;
        _size--; //释放节点，更新规模
        return e; //返回备份的数值
    }
    void merge (List<T> & L) { 
        merge(header->succ, _size, L, L.header->succ, L._size);
    } //全列表归并

    void sort ( ListNode<T>* p, int n ) {
        switch(rand() % 4) { //随机选取排序算法。可根据具体问题的特点灵活选取或扩充
            case 1:  insertionSort (p, n); break; //插入排序
            case 2:  selectionSort (p, n); break; //选择排序
            case 3:  mergeSort (p, n); break; //归并排序
            default: radixSort (p, n); break; //基数排序
        }
    }

    void sort() { 
        sort(first(), _size);
    }
    int deduplicate() {
        int oldSize = _size; ListNode<T>* p = first();
        for ( int r = 0; p != trailer; p = p->succ ) {
            if ( ListNode<T>* q = find(p->data, r, p) ){
               remove(q); //此时q与p雷同，但删除前者更为简明
            }
            else {
                r++;
            } //r为无重前缀的长度
        }
        return oldSize - _size; //删除元素总数
    }

    int uniquify() {
         if ( _size < 2 ){ 
             return 0;} //平凡列表自然无重复
        int oldSize = _size; //记录原规模
        ListNode<T>* p = first(); ListNode<T>* q; //p为各区段起点，q为其后继
        while(trailer != (q = p->succ)) { //反复考查紧邻的节点对(p, q)
            if (p->data != q->data){
                p = q;
            } //若互异，则转向下一区段
            else {
                remove(q);
            } //否则（雷同），删除后者
        }
        return oldSize - _size; //列表规模变化量，即被删除元素总数
    }
    void reverse_1() {
        ListNode<T>* p = header;
        ListNode<T>* q = trailer;
        for(int i = 1; i < _size; i += 2) {
            swap ((p = p->succ)->data, (q = q->pred)->data); //交换对称节点的数据项
        }
    }

    void reverse_2() {
        if(_size < 2) { 
            return; //平凡情况
        }
        for(ListNode<T>* p = header; p; p = p->pred){
            swap(p->pred, p->succ); //交换各节点的前驱、后继指针
        }
        swap(header, trailer); //头、尾节点互换
    }

    void reverse_3() {
        if(_size < 2) {
            return;
        }
        ListNode<T>* p; ListNode<T>* q;
        for(p = header, q = p->succ; p != trailer; p = q, q = p->succ){
            p->pred = q;
        }
        trailer->pred = NULL; //单独设置尾节点的前驱指针
        for(p = header, q = p->pred; p != trailer; p = q, q = p->pred) {
            q->succ = p; //自前向后，依次颠倒各节点的后继指针
        }
        header->succ = NULL; //单独设置头节点的后继指针
        swap(header, trailer); //头、尾节点互换
    }
    void traverse(void(*)(T&)) {  //遍历，依次实施visit操作（函数指针，只读或局部性修改）
        for(ListNode<T>* p = header->succ; p != trailer; p = p->succ) 
            visit(p->data);
    } 
    template <typename VST> //操作器
    void traverse(VST&) {
        for(ListNode<T>* p = header->succ; p != trailer; p = p->succ ) {
            visit(p->data);  
        }
    } //遍历，依次实施visit操作（函数对象，可全局性修改）
};


