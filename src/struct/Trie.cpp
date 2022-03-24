#include <iostream>
#include <string>

struct trie {
    int nex[100000][26];
    int cnt;
    bool exist[100000];  // 该结点结尾的字符串是否存在

    void insert(const std::string& str) {  // 插入字符串
        int p = 0;
        for (int i = 0; i < str.size(); i++) {
            int c = str[i] - 'a';
            if (!nex[p][c]) {
                nex[p][c] = ++cnt;
            }  // 如果没有，就添加结点
            p = nex[p][c];
        }
        exist[p] = 1;
    }

    bool find(const std::string& str) {  // 查找字符串
        int p = 0;
        for (int i = 0; i < str.size(); i++) {
            int c = str[i] - 'a';
            if (!nex[p][c]) {
                return 0;
            }
            p = nex[p][c];
        }
        return exist[p];
    }
};

int main() {
    trie* t = new trie();
    t->insert("abcde");
    t->insert("afghh");
    if(t->find("abcde")) {
        std::cout<<"yes"<<std::endl;
    }
}