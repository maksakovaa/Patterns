//State
#include <iostream>
#include <typeinfo>
class Context;

class State
{
public:
    virtual ~State(){}
    void SetContext(Context* context)
    {
        this->m_context = context;
    }
    virtual void Handle1() = 0;
    virtual void Handle2() = 0;
protected:
    Context* m_context;
};

class Context
{
public:
    Context(State* state): m_state(nullptr)
    {
        this->TransitionTo(state);
    }
    ~Context()
    {
        delete m_state;
    }
    void TransitionTo(State* state)
    {
        std::cout << "Context: Transition to: " << typeid(*state).name() << ".\n";
        if (this->m_state != nullptr)
        {
            delete this->m_state;
        }
        this->m_state = state;
        this->m_state->SetContext(this);
    }
    void Request1()
    {
        this->m_state->Handle1();
    }
    void Request2()
    {
        this->m_state->Handle2();
    }
private:
    State* m_state;
};

class ConcreteStateA: public State
{
public:
    void Handle1() override;
    void Handle2() override
    {
        std::cout << "ConcreteStateA handles request2.\n";
    }
};

class ConcreteStateB: public State
{
public:
    void Handle1() override
    {
        std::cout << "ConcreteStateB handles request1.\n";
    }
    void Handle2() override
    {
        std::cout << "ConcreteStateB handles request2.\n";
        std::cout << "ConcreteStateB wants to change the state of the context.\n";
        this->m_context->TransitionTo(new ConcreteStateA);
    }
};

void ConcreteStateA::Handle1()
{
    std::cout << "ConcreteStateA handles request1.\n";
    std::cout << "ConcreteStateA wants to change the state of the context.\n";
    this->m_context->TransitionTo(new ConcreteStateB);
}

void test()
{
    Context* context = new Context(new ConcreteStateA);
    context->Request1();
    context->Request2();
    delete context;
}

int main()
{
    test();
}