//Command
#include <string>
#include <iostream>
class Command
{
public:
    virtual ~Command(){}
    virtual void Execute() const = 0;
};

class SimpleCommand: public Command
{
public:
    explicit SimpleCommand(std::string pay_load): _pay_load(pay_load){}
    void Execute() const override
    {
        std::cout << "SimpleCommand: See, i can do simle things like printing(" << this->_pay_load << ")\n";
    }
private:
    std::string _pay_load;
};

class Receiver
{
public:
    void DoSomething(const std::string &a)
    {
        std::cout << "Receiver: Working on (" << a << ").\n";
    }
    void DoSomethingElse(const std::string &b)
    {
        std::cout << "Receiver: Also working on (" << b << ").\n";
    }
};

class ComplexCommand: public Command
{
public:
    ComplexCommand(Receiver* receiver, std::string a, std::string b):
        _receiver(receiver), _a(a), _b(b) {}
    void Execute() const override
    {
        std::cout << "ComplexCommand: Complex stuff shoul be done by a receiver object.\n";
        this->_receiver->DoSomething(this->_a);
        this->_receiver->DoSomethingElse(this->_b);
    }
private:
    Receiver* _receiver;
    std::string _a;
    std::string _b;
};

class Invoker
{
public:
    ~Invoker(){ delete _onStart; delete _onFinish; }
    void SetOnStart(Command* command)
    {
        this->_onStart = command;
    }
    void SetOnFinish(Command* command)
    {
        this->_onFinish = command;
    }
    void DoSomethingImportant()
    {
        std::cout << "Invoker: Does anybody want something done before i begin?\n";
        if (this->_onStart)
        {
            this->_onStart->Execute();
        }
        std::cout << "Invoker: ...doing something really important...\n";
        std::cout << "Invoker: Does anybody want something done after i finish?\n";
        if (this->_onFinish)
        {
            this->_onFinish->Execute();
        }
        
    }
private:
    Command* _onStart;
    Command* _onFinish;
};

void test()
{
    Invoker* invoker = new Invoker;
    invoker->SetOnStart(new SimpleCommand("Say Hi!"));
    Receiver* receiver = new Receiver;
    invoker->SetOnFinish(new ComplexCommand(receiver, "Send email", "Save report"));
    invoker->DoSomethingImportant();

    delete invoker;
    delete receiver;
}

int main()
{
    test();
}