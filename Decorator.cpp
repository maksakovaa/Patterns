//Decorator pattern

#include <iostream>
#include <string>

using namespace std;

//Component interface - defines the basic ice cream operations

class IceCream
{
public:
    virtual string getDescription() const = 0;
    virtual double coast() const = 0;
};

//Concrete Component - the basic ice cream class:
class VanillaIceCream: public IceCream
{
public:
    string getDescription() const override
    {
        return "Vanilla Ice Cream";
    }
    double coast() const override
    {
        return 160.0;
    }
};

//Decorator - abstract class that extends IceCream:
class IceCreamDecorator: public IceCream
{
public:
    IceCreamDecorator(IceCream* ic): iceCream(ic){}
    string getDescription() const override
    {
        return iceCream->getDescription();
    }
    double coast() const override
    {
        return iceCream->coast();
    }
protected:
    IceCream* iceCream;
};

//Concrete decorator -adds chocolate topping
class ChocolateDecorator: public IceCreamDecorator
{
public:
    ChocolateDecorator(IceCream* ic): IceCreamDecorator(ic) {}
    string getDescription() const override
    {
        return iceCream->getDescription() + " with Chocolate";
    }
    double coast() const override
    {
        return iceCream->coast() + 100.0;
    }
};

//Concrete Decorator -adds caramel topping
class CaramelDecorator: public IceCreamDecorator
{
public:
    CaramelDecorator(IceCream* ic): IceCreamDecorator(ic){}
    string getDescription() const override
    {
        return iceCream->getDescription() + " with Caramel";
    }
    double coast() const override
    {
        return iceCream->coast() + 150.0;
    }
};

void test()
{
        // Create a vanilla ice cream
    IceCream* vanillaIceCream = new VanillaIceCream();
    cout << "Order: " << vanillaIceCream->getDescription()
         << ", Cost: Rs." << vanillaIceCream->coast()
         << endl;
 
    // Wrap it with ChocolateDecorator
    IceCream* chocolateIceCream
        = new ChocolateDecorator(vanillaIceCream);
    cout << "Order: " << chocolateIceCream->getDescription()
         << ", Cost: Rs." << chocolateIceCream->coast()
         << endl;
 
    // Wrap it with CaramelDecorator
    IceCream* caramelIceCream
        = new CaramelDecorator(chocolateIceCream);
    cout << "Order: " << caramelIceCream->getDescription()
         << ", Cost: Rs." << caramelIceCream->coast()
         << endl;
 
    delete vanillaIceCream;
    delete chocolateIceCream;
    delete caramelIceCream;
}
// Component interface - defines the basic cake operations.
class Cake {
public:
    virtual string getDescription() const = 0;
    virtual double cost() const = 0;
};
 
// Concrete Component - the basic cake class.
class PlainCake : public Cake {
public:
    string getDescription() const override
    {
        return "Plain Cake";
    }
 
    double cost() const override { return 300.0; }
};
 
// Decorator - abstract class that extends Cake.
class CakeDecorator : public Cake {
protected:
    Cake* cake;
 
public:
    CakeDecorator(Cake* c)
        : cake(c)
    {
    }
 
    string getDescription() const override
    {
        return cake->getDescription();
    }
 
    double cost() const override { return cake->cost(); }
};
 
// Concrete Decorator - adds chocolate topping.
class ChocolateCakeDecorator : public CakeDecorator {
public:
    ChocolateCakeDecorator(Cake* c)
        : CakeDecorator(c)
    {
    }
 
    string getDescription() const override
    {
        return cake->getDescription() + " with Chocolate";
    }
 
    double cost() const override
    {
        return cake->cost() + 200.0;
    }
};
 
// Concrete Decorator - adds fruit decorations.
class FruitCakeDecorator : public CakeDecorator {
public:
    FruitCakeDecorator(Cake* c)
        : CakeDecorator(c)
    {
    }
 
    string getDescription() const override
    {
        return cake->getDescription() + " with Fruits";
    }
 
    double cost() const override
    {
        return cake->cost() + 150.0;
    }
};

void test2()
{
        // Create a plain cake
    Cake* plainCake = new PlainCake();
    cout << "Plain Cake:" << plainCake->getDescription()
         << "\nCost:Rs." << plainCake->cost() << endl;
 
    // Wrap it with ChocolateCakeDecorator
    Cake* chocolateCake = new ChocolateCakeDecorator(plainCake);
    cout << "\nChocolate Cake:"
         << chocolateCake->getDescription() << "\nCost:Rs."
         << chocolateCake->cost() << endl;
 
    // Wrap it with FruitDecorator
    Cake* fruitCake1 = new FruitCakeDecorator(chocolateCake);
    cout << "\nFruit Cake:" << fruitCake1->getDescription()
         << "\nCost:Rs." << fruitCake1->cost() << endl;
 
    // Clean up memory
    delete chocolateCake;
    delete fruitCake1;
 
    // Wrap plain cake with FruitDecorator
    Cake* fruitCake2 = new FruitCakeDecorator(plainCake);
    cout << "\nFruit Cake:" << fruitCake2->getDescription()
         << "\nCost:Rs." << fruitCake2->cost() << endl;
 
    // Clean up memory
    delete plainCake;
    delete fruitCake2;
}
int main()
{
    test();
    cout << "\n\n";
    test2();
    return 0;
}