#include <iostream>

struct Product {
    std::string A;
    std::string B;
    std::string C;
    void setA(const std::string& str) {
        A = str;
    }
    void setB(const std::string& str) {
        B = str;
    }
    void setC(const std::string& str) {
        C = str;
    }
    void show() {
        std::cout<< A << " " << B << " " << C << std::endl;
    }
};

struct Builder {
    Product* m_prod;
    Builder(){
        m_prod = new Product();
    }
    virtual ~Builder(){}
    virtual void buildPartA(){
	    m_prod->setA("A Style ");
    }
    virtual void buildPartB(){
	    m_prod->setB("B Style ");
    }
    virtual void buildPartC(){
	    m_prod->setC("C style ");
    }
    virtual Product* getResult(){
        return m_prod;
    }
};

struct ConcreteBuilder : public Builder{
    ConcreteBuilder(){}
    virtual ~ConcreteBuilder(){}
    virtual void buildPartA(){
	    m_prod->setA("A Style ");
    }
    virtual void buildPartB(){
	    m_prod->setB("B Style ");
    }
    virtual void buildPartC(){
	    m_prod->setC("C style ");
    }
};

struct Director{
    Builder* m_pbuilder;
    Director(){}
    ~Director(){}
    void setBuilder(Builder* buider){
	    m_pbuilder = buider;
    }
    Product* constuct(){
	    m_pbuilder->buildPartA();
	    m_pbuilder->buildPartB();
	    m_pbuilder->buildPartC();
        return m_pbuilder->getResult();
    }
    
};

int main() {
    ConcreteBuilder * builder = new ConcreteBuilder();
	Director  director;
	director.setBuilder(builder);
	Product * pd = director.constuct();
	pd->show();
	
	delete builder;
	delete pd;
	return 0;
}

// 优点：
// 在建造者模式中， 客户端不必知道产品内部组成的细节，将产品本身与产品的创建过程解耦，使得相同的创建过程可以创建不同的产品对象。

// 每一个具体建造者都相对独立，而与其他的具体建造者无关
// 因此可以很方便地替换具体建造者或增加新的具体建造者， 用户使用不同的具体建造者即可得到不同的产品对象 。

// 可以更加精细地控制产品的创建过程 。将复杂产品的创建步骤分解在不同的方法中，使得创建过程更加清晰，也更方便使用程序来控制创建过程。

// 增加新的具体建造者无须修改原有类库的代码，指挥者类针对抽象建造者类编程，系统扩展方便，符合“开闭原则”。

// 缺点：
// 建造者模式所创建的产品一般具有较多的共同点，其组成部分相似，如果产品之间的差异性很大，
// 则不适合使用建造者模式，因此其使用范围受到一定的限制。

// 如果产品的内部变化复杂，可能会导致需要定义很多具体建造者类来实现这种变化，导致系统变得很庞大。