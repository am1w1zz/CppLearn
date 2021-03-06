#include <iostream>

struct Product {
    Product(){};
    virtual void show()=0;
    virtual ~Product(){};
};

struct ProductA : public Product{
    ProductA(){};
    virtual void show() { 
        std::cout << "AbstractFactory : product A create" << std::endl;
    }
    virtual ~ProductA(){};
};

struct ProductB : public Product{
public:
    ProductB(){};
    virtual void show(){ 
        std::cout << "AbstractFactory : product B create" << std::endl;
    }
    virtual ~ProductB(){};
};

struct Factory{
    Factory(){};
    virtual Product* CreateProduct()=0;
    virtual ~Factory(){};
};

struct FactorA: public Factory{
    FactorA(){};
    virtual Product* CreateProduct(){
        return new ProductA();
    }
    virtual ~FactorA(){};
    
};

struct FactorB: public Factory{
    FactorB(){};
    virtual Product* CreateProduct(){
        return new ProductB();
    }
    virtual ~FactorB(){};
};

int main() {
    Product* prod = nullptr;
    Factory* fac = new FactorA();
    prod = fac->CreateProduct();
    prod->show();
    delete prod;
    fac = new FactorB();
    prod = fac->CreateProduct();// 调⽤产品B的⼯⼚来⽣产B产品
    prod->show();
    delete prod;
    delete fac;
    return 0;
}

// 优点：
// 抽象工厂模式隔离了具体类的生成，使得客户并不需要知道什么被创建。由于这种隔离，更换一个具体工厂就变得相对容易。所有的具体工厂都实现了抽象工厂中定义的那些公共接口，因此只需改变具体工厂的实例，就可以在某种程度上改变整个软件系统的行为。另外，应用抽象工厂模式可以实现高内聚低耦合的设计目的，因此抽象工厂模式得到了广泛的应用。
// 当一个产品族中的多个对象被设计成一起工作时，它能够保证客户端始终只使用同一个产品族中的对象。这对一些需要根据当前环境来决定其行为的软件系统来说，是一种非常实用的设计模式。
// 增加新的具体工厂和产品族很方便，无须修改已有系统，符合“开闭原则”。

// 缺点：
// 在添加新的产品对象时，难以扩展抽象工厂来生产新种类的产品，这是因为在抽象工厂角色中规定了所有可能被创建的产品集合，要支持新种类的产品就意味着要对该接口进行扩展，而这将涉及到对抽象工厂角色及其所有子类的修改，显然会带来较大的不便。
// 开闭原则的倾斜性（增加新的工厂和产品族容易，增加新的产品等级结构麻烦）。

// 适用环境：
// 一个系统不应当依赖于产品类实例如何被创建、组合和表达的细节，这对于所有类型的工厂模式都是重要的。
// 系统中有多于一个的产品族，而每次只使用其中某一产品族。
// 属于同一个产品族的产品将在一起使用，这一约束必须在系统的设计中体现出来。
// 系统提供一个产品类的库，所有的产品以同样的接口出现，从而使客户端不依赖于具体实现。