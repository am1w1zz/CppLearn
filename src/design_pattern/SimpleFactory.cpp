#include <iostream>

struct Product {
    Product(){};
    virtual void show()=0;
    virtual ~Product(){};
};

struct productA : public Product{
    productA(){};
    void show() { 
        std::cout << "product A create" << std::endl;
    }
    virtual ~productA(){};
};

struct productB : public Product{
public:
    productB(){};
    void show(){ 
        std::cout << "product B create" << std::endl;
    }
    ~productB(){};
};

struct SimpleFactory {
    Product* Product(std::string product_name) {
        if (product_name == "A") {
            return new productA();
	    } else if(product_name == "B") {
            return new productB();
	    }
	    return  nullptr;
    }
};

int main() {
    SimpleFactory factory;
    Product* pro;
    pro = factory.Product("A");
    pro->show();
    delete pro;
    pro = factory.Product("B");
    pro->show();
    delete pro;
    return 0;
}

// 分析：
// 将对象的创建和对象本身业务处理分离可以降低系统的耦合度，使得两者修改起来都相对容易。
// 在调用工厂类的工厂方法时，由于工厂方法是静态方法，使用起来很方便，可通过类名直接调用，而且只需要传入一个简单的参数即可，在实际开发中，还可以在调用时将所传入的参数保存在XML等格式的配置文件中，修改参数时无须修改任何源代码。
// 简单工厂模式最大的问题在于工厂类的职责相对过重，增加新的产品需要修改工厂类的判断逻辑，这一点与开闭原则是相违背的。
// 简单工厂模式的要点在于：当你需要什么，只需要传入一个正确的参数，就可以获取你所需要的对象，而无须知道其创建细节。

// 简单工厂模式的优点：
// 工厂类含有必要的判断逻辑，可以决定在什么时候创建哪一个产品类的实例，客户端可以免除直接创建产品对象的责任
// 而仅仅“消费”产品；简单工厂模式通过这种做法实现了对责任的分割，它提供了专门的工厂类用于创建对象。
// 客户端无须知道所创建的具体产品类的类名，只需要知道具体产品类所对应的参数即可，对于一些复杂的类名，通过简单工厂模式可以减少使用者的记忆量。
// 通过引入配置文件，可以在不修改任何客户端代码的情况下更换和增加新的具体产品类，在一定程度上提高了系统的灵活性。

// 简单工厂模式的缺点：
// 由于工厂类集中了所有产品创建逻辑，一旦不能正常工作，整个系统都要受到影响。
// 使用简单工厂模式将会增加系统中类的个数，在一定程序上增加了系统的复杂度和理解难度。
// 系统扩展困难，一旦添加新产品就不得不修改工厂逻辑，在产品类型较多时，有可能造成工厂逻辑过于复杂，不利于系统的扩展和维护。
// 简单工厂模式由于使用了静态工厂方法，造成工厂角色无法形成基于继承的等级结构。


// 适用环境：
// 在以下情况下可以使用简单工厂模式：
// 工厂类负责创建的对象比较少：由于创建的对象较少，不会造成工厂方法中的业务逻辑太过复杂。
// 客户端只知道传入工厂类的参数，对于如何创建对象不关心：客户端既不需要关心创建细节，甚至连类名都不需要记住，只需要知道类型所对应的参数。