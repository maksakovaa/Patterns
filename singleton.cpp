#include <iostream>
#include <thread>

class Singleton
{
protected:
    Singleton(const std::string value) : value_(value) {}
    static Singleton* singleton_;
    std::string value_;
public:
    Singleton(Singleton& other) = delete;
    void operator=(const Singleton&) = delete;
    static Singleton* GetInstance(const std::string& value);
    std::string value() const
    {
        return value_;
    }
};

Singleton* Singleton::singleton_ = nullptr;

Singleton* Singleton::GetInstance(const std::string& value)
{
    if (singleton_ == nullptr)
    {
        singleton_ = new Singleton(value);
    }
    return singleton_;
}

void ThreadFoo()
{
    Singleton* singleton = Singleton::GetInstance("FOO");
    std::cout << singleton->value() << "\n";
}

void ThreadBar()
{
    Singleton* singleton = Singleton::GetInstance("BAR");
    std::cout << singleton->value() << "\n";
}

int main()
{
    std::cout << "If you see the same value then singleton was reused (correct)\n";
    std::cout << "Else if you see different values then 2 singletones were created (incorrect)\n\n";
    std::thread t1(ThreadFoo);
    std::thread t2(ThreadBar);
    t1.join();
    t2.join();
    return 0;
}