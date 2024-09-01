//Memento
#include <string>
#include <chrono>
#include <iostream>
#include <vector>

/*
    Интерфейс снимка
*/

class Memento
{
public:
    virtual ~Memento(){}
    virtual std::string GetName() const = 0;
    virtual std::string date() const = 0;
    virtual std::string state() const = 0;
};

/*
    Конкретный снимок
*/

class ConcreteMemento: public Memento
{
public:
    ConcreteMemento(std::string state): m_state(state)
    {
        this->m_state = state;
        std::time_t now = std::time(0);
        this->m_date = std::ctime(&now);
    }
    std::string state() const override
    {
        return this->m_state;
    }
    std::string GetName() const override
    {
        return this->m_date + " / (" + this->m_state.substr(0,9) + "...)";
    }
    std::string date() const override
    {
        return this->m_date;
    }
private:
    std::string m_state;
    std::string m_date;
};

class Originator
{
public:
    Originator(std::string state): m_state(state)
    {
        std::cout << "Originator: Initial state is: " << this->m_state << std::endl;
    }
    void DoSomething()
    {
        std::cout << "Originator: Doing something important...\n";
        this->m_state = this->GenerateRandomString(30);
        std::cout << "Originator: State chaged to: " << this->m_state << "\n";
    }
    Memento* SaveState()
    {
        return new ConcreteMemento(this->m_state);
    }
    void RestoreState(Memento* memento)
    {
        this->m_state = memento->state();
        std::cout << "Originator: State restored to: " << this->m_state << "\n";
    }
private:
    std::string m_state;
    std::string GenerateRandomString(int length = 10)
    {
        const char alphanum[] = "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        int stringLength = sizeof(alphanum) - 1;

        std::string random_string;
        for (int i = 0; i < length; i++)
        {
            random_string += alphanum[std::rand() % stringLength];
        }
        return random_string;
    }
};

class Caretaker
{
public:
    Caretaker(Originator* originator): m_originator(originator) {}
    ~Caretaker()
    {
        for (auto m: m_mementos)
        {
            delete m;
        }
    }
    void Backup()
    {
        std::cout << "\nCaretaker: saving Originator's state...\n";
        this->m_mementos.push_back(this->m_originator->SaveState());
    }
    void Undo()
    {
        if (!this->m_mementos.size())
        {
            return;
        }
        Memento* memento = this->m_mementos.back();
        this->m_mementos.pop_back();
        std::cout << "Caretaker: Restoring state to: " << memento->GetName() << "\n";
        try
        {
            this->m_originator->RestoreState(memento);
        }
        catch(...)
        {
            this->Undo();
        }
    }
    void ShowHistory() const
    {
        std::cout << "Caretaker: Here's the lis of mementos: \n";
        for (Memento* mem: this->m_mementos)
        {
            std::cout << mem->GetName() << "\n";
        }
    }
private:
    std::vector<Memento*> m_mementos;
    Originator* m_originator;
};

void test()
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    Originator *originator = new Originator("Super-duper-super-puper-super.");
    Caretaker *caretaker = new Caretaker(originator);
    caretaker->Backup();
    originator->DoSomething();
    caretaker->Backup();
    originator->DoSomething();
    caretaker->Backup();
    originator->DoSomething();
    std::cout << "\n";
    caretaker->ShowHistory();
    std::cout << "\nClient: Now, let's rollback!\n\n";
    caretaker->Undo();
    std::cout << "\nClient: Once more!\n\n";
    caretaker->Undo();

    delete originator;
    delete caretaker;
}

int main()
{
    test();
}