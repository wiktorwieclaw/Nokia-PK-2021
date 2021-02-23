#include <iostream>
#include <memory>

struct A
{
    A()
    {
        std::cout << "A()" << std::endl;
    }
    virtual ~A()
    {
        std::cout << "~A()" << std::endl;
    }
};

struct Base
{
    Base()
    {
        std::cout << "Base()" << std::endl;
    }
    virtual ~Base()
    {
        std::cout << "~Base()" << std::endl;
    }
};

struct Derived : Base
{
    std::shared_ptr<A> a = std::make_shared<A>();
    Derived()
    {
        std::cout << "Derived()" << std::endl;
    }
    ~Derived()
    {
        std::cout << "~Derived()" << std::endl;
    }
};

//int foo(std::unique_ptr )

int main()
{
    std::unique_ptr<Base> d = std::make_unique<Derived>();
    std::unique_ptr<Base> c = std::move(d);

}
