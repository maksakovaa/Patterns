//TemplateMethod
#include <iostream>

class AbstractClass
{
public:
    void TemplateMethod() const
    {
        this->BaseOperation1();
        this->RequiredOperations1();
        this->BaseOperation2();
        this->Hook1();
        this->RequiredOperations2();
        this->BaseOperation3();
        this->Hook2();
    }
protected:
    void BaseOperation1() const
    {
        std::cout << "AbstractClass: doing the bulk of the work\n";
    }
    void BaseOperation2() const
    {
        std::cout << "AbstractClass: let subclasses override some operations\n";
    }
    void BaseOperation3() const
    {
        std::cout << "AbstractClass: doing the bulk of the work anyway\n";
    }
    virtual void RequiredOperations1() const = 0;
    virtual void RequiredOperations2() const = 0;
    virtual void Hook1() const {}
    virtual void Hook2() const {}
};

class ConcreteClass1: public AbstractClass
{
protected:
    void RequiredOperations1() const override
    {
        std::cout << "ConcreteClass1 says: Implemented Operation1\n";
    }
    void RequiredOperations2() const override
    {
        std::cout << "ConcreteClass1 says: Implemented Operaion2\n";
    }
};

class ConcreteClass2: public AbstractClass
{
protected:
    void RequiredOperations1() const override
    {
        std::cout << "ConcreteClass2: Implemeted Operation1\n";
    }
    void RequiredOperations2() const override
    {
        std::cout << "ConcreteClass2: Implemented Operation2\n";
    }
    void Hook1() const override
    {
        std::cout << "ConcreteClass2: Overriden Hook1\n";
    }
};

void ClientCode(AbstractClass* _class)
{
    _class->TemplateMethod();
}

void test()
{
    std::cout << "Same client code can work with different subclasses:\n";
    ConcreteClass1* concreteClass1 = new ConcreteClass1;
    ClientCode(concreteClass1);
    std::cout << "\n Same client code can work with different subclasses:\n";
    ConcreteClass2* concreteClass2 = new ConcreteClass2;
    ClientCode(concreteClass2);
    delete concreteClass1;
    delete concreteClass2;
}

int main()
{
    test();
}