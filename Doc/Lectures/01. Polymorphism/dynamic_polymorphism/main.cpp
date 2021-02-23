#include <iostream>

struct A
{
    virtual void foo() = 0;
    virtual ~A(){}
};

struct B : A
{
    void foo() final
    {
        std::cout << "B::foo()" << std::endl;
    }
};


int main()
{
    B b;
    b.foo();

    A& a = b;
    a.foo();
}
