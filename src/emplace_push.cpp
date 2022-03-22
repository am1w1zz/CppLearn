#include<iostream>
#include<vector>


class Person {
    int _age;
public:
    Person(int age) : _age(age) {
        std::cout << "Construct a person." << _age << std::endl;
    }

    Person(const Person &p) : _age(p._age) {
        std::cout << "Copy-Construct" << _age << std::endl;
    }

    Person(const Person &&p) noexcept: _age(p._age) {
        std::cout << "Move-Construct" << _age << std::endl;
    }
};

int main() {
    std::vector<Person> person1;
    std::vector<Person> person2;
    auto p1 = Person(1);
    auto p2 = Person(2);
    std::cout<<std::endl;
    person1.push_back(std::move(p1));
    std::cout<<std::endl;
    person2.emplace_back(std::move(p2));
    
}