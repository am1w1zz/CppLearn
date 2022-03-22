#include <iostream>
#include <stack>
#include <string>

int main() {
    std::string str;
    std::cin>>str;
    std::stack<char> char_stack;
    for(int i = 0;i < str.size(); ++i) {
        if(str[i] == '(' || str[i] == '{' || str[i] == '[') {
            char_stack.push(str[i]);
        } else {
            if((str[i] == ')' && char_stack.top() == '(') || (str[i] == '}' && char_stack.top() == '{') || (str[i] == ']' && char_stack.top() == '[')) {
                char_stack.pop();
            }   
        }
    }    
    std::string ans = char_stack.size() > 0 ? "false" : "true";
    std::cout<<ans<<std::endl;
}