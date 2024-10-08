//Adapter pattern
#include <iostream>
#include <algorithm>
/**
 * Целевой класс объявляет интерфейс, с которым может работать клиентский код.
 */
class Target
{
public:
    virtual ~Target() = default;
    virtual std::string Request() const
    {
        return "Target: The default target's behavior.";
    }
};

/**
 * Адаптируемый класс содержит некоторое полезное поведение, но его интерфейс
 * несовместим с существующим клиентским кодом. Адаптируемый класс нуждается в
 * некоторой доработке, прежде чем клиентский код сможет его использовать.
 */

class Adaptee
{
public:
    std::string SpecificRequest() const
    {
        return ".eetpadA eht fo roivaheb laicepS";
    }
};

/**
 * Адаптер делает интерфейс Адаптируемого класса совместимым с целевым
 * интерфейсом с помощью множественного наследования.
 */

class Adapter : public Target, public Adaptee
{
 public:
    Adapter() {}
    std::string Request() const override
    {
        std::string to_reverse = SpecificRequest();
        std::reverse(to_reverse.begin(), to_reverse.end());
        return "Adapter: (TRANSLATED) " + to_reverse;
    }
};

/**
 * Клиентский код поддерживает все классы, использующие целевой интерфейс.
 */

void ClientCode(const Target *target)
{
    std::cout << target->Request();
}

class Adapter2 : public Target
{
private:
    Adaptee *adaptee_;

public:
    Adapter2(Adaptee *adaptee) : adaptee_(adaptee) {}
    std::string Request() const override
    {
        std::string to_reverse = this->adaptee_->SpecificRequest();
        std::reverse(to_reverse.begin(), to_reverse.end());
        return "Adapter: (TRANSLATED) " + to_reverse;
    }
};

void test()
{
    std::cout << "Client: I can work just fine with the Target objects:\n";
    Target *target = new Target;
    ClientCode(target);
    std::cout << "\n\n";
    Adaptee *adaptee = new Adaptee;
    std::cout << "Client: The Adaptee class has a weird interface. See, I don't understand it:\n";
    std::cout << "Adaptee: " << adaptee->SpecificRequest();
    std::cout << "\n\n";
    std::cout << "Client: But I can work with it via the Adapter:\n";
    Adapter *adapter = new Adapter;
    ClientCode(adapter);
    std::cout << "\n";

    delete target;
    delete adaptee;
    delete adapter;
}

void test2()
{
    std::cout << "Client: I can work just fine with the Target objects:\n";
    Target *target = new Target;
    ClientCode(target);
    std::cout << "\n\n";
    Adaptee *adaptee = new Adaptee;
    std::cout << "Client: The Adaptee class has a weird interface. See, I don't understand it:\n";
    std::cout << "Adaptee: " << adaptee->SpecificRequest();
    std::cout << "\n\n";
    std::cout << "Client: But I can work with it via the Adapter:\n";
    Adapter2 *adapter = new Adapter2(adaptee);
    ClientCode(adapter);
    std::cout << "\n";

    delete target;
    delete adaptee;
    delete adapter;
}

int main()
{
    test();
    std::cout << "\n\n\n";
    test2();
    return 0;
}