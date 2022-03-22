#include <iostream>
#include <vector>

// C++ Version
const int SIZE = 1000000;
const int M = 999997;

struct HashTable {
    struct Node {
        int next;
        int value;
        int key;
    } data[SIZE];

    int head[M];
    int size;
    int f(int key) { 
        return key % M;
    }
    int get(int key) {
        for (int p = head[f(key)]; p; p = data[p].next){
            if (data[p].key == key) {
                return data[p].value;
            }
        }
        return -1;
    }
    int modify(int key, int value) {
        for (int p = head[f(key)]; p; p = data[p].next){
            if (data[p].key == key) {
                return data[p].value = value;
            }
        }
    }
    int add(int key, int value) {
        if (get(key) != -1) {
            return -1;
        }
        data[++size] = (Node){head[f(key)], value, key};
        head[f(key)] = size;
        return value;
    }
};

const int SZ = 999997;

struct hash_map {  // 哈希表模板
    struct data {
        long long u;
        int v;
        int nex;
    };  // 前向星结构

    data e[SZ << 1];  // SZ 是 const int 表示大小
    int h[SZ];
    int cnt;
    hash_map() {
        cnt = 0;
        memset(h, 0, sizeof(h));
    }

    int hash(long long u) { 
        return u % SZ;
    }

    int& operator[](long long u) {
        int hu = hash(u);  // 获取头指针
        for (int i = h[hu]; i; i = e[i].nex) {
            if (e[i].u == u){
                return e[i].v;
            }
        }
        return e[++cnt] = (data){u, -1, h[hu]}, h[hu] = cnt, e[cnt].v;
    }
};

const int N = 360007;  // N 是最大可以存储的元素数量
class Hash {
private:
    int keys[N];
    int values[N];
public:
    Hash() { 
        memset(values, 0, sizeof(values));
    }
    int& operator[](int n) {
        // 返回一个指向对应 Hash[Key] 的引用
        // 修改成不为 0 的值 0 时候视为空
        int idx = (n % N + N) % N;
        int cnt = 1;
        while (keys[idx] != n && values[idx] != 0) {
            idx = (idx + cnt * cnt) % N;
            cnt += 1;
        }
        keys[idx] = n;
        return values[idx];
    }
};

int main() {

}