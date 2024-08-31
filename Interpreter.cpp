//Interpreter pattern

#include <iostream>
#include <map>
#include <cstring>

class Context;

class BooleanExp
{
public:
    BooleanExp() = default;
    virtual ~BooleanExp() = default;
    virtual bool evaluate(Context&) = 0;
    virtual BooleanExp* replace(const char*, BooleanExp&) = 0;
    virtual BooleanExp* copy() const = 0;
};

class VariableExp;

class Context
{
public:
    Context() :m() {}
    bool lookup(const VariableExp* key)
    {
        return m.at(key);
    }
    void assign(VariableExp* key, bool value)
    {
        m[key] = value;
    }
private:
    std::map<const VariableExp*, bool> m;
};

class VariableExp : public BooleanExp 
{
public:
    VariableExp(const char* name_) :name(nullptr)
    {
        name = strdup(name_);
    }
    virtual ~VariableExp() = default;
    virtual bool evaluate(Context& aContext)
    {
        return aContext.lookup(this);
    }
    virtual BooleanExp* replace( const char* name_, BooleanExp& exp )
    {
        if (0 == strcmp(name_, name))
        {
            return exp.copy();
        }
        else
        {
            return new VariableExp(name);
        }
    }
    virtual BooleanExp* copy() const
    {
        return new VariableExp(name);
    }
    VariableExp(const VariableExp&) = delete; // rule of three
    VariableExp& operator=(const VariableExp&) = delete;
private:
    char* name;
};

class AndExp : public BooleanExp
{
public:
    AndExp(BooleanExp* op1, BooleanExp* op2):operand1(nullptr), operand2(nullptr)
    {
        operand1 = op1;
        operand2 = op2;
    }
    virtual ~AndExp() = default;
    virtual bool evaluate(Context& aContext)
    {
        return operand1->evaluate(aContext) && operand2->evaluate(aContext);
    }
    virtual BooleanExp* replace(const char* name_, BooleanExp& exp)
    {
        return new AndExp(
            operand1->replace(name_, exp),
            operand2->replace(name_, exp)
        );
    }
    virtual BooleanExp* copy() const
    {
        return new AndExp(operand1->copy(), operand2->copy());
    }
    AndExp(const AndExp&) = delete; // rule of three
    AndExp& operator=(const AndExp&) = delete;
private:
    BooleanExp* operand1;
    BooleanExp* operand2;
};

void test()
{
    BooleanExp* expression;
    Context context;
    VariableExp* x = new VariableExp("X");
    VariableExp* y = new VariableExp("Y");
    expression = new AndExp(x, y);

    context.assign(x, false);
    context.assign(y, true);
    bool result = expression->evaluate(context);
    std::cout << result << '\n';

    context.assign(x, true);
    context.assign(y, true);
    result = expression->evaluate(context);
    std::cout << result << '\n';
}

int main()
{
    test();
}