#include <iostream>

struct A
{
    A(int){}

    void foo(int)
    {
        std::cout << "A::foo(int)" << std::endl;
    }
    void foo(short) const
    {
        std::cout << "A::foo(short)" << std::endl;
    }
    void foo(int, int)
    {
        std::cout << "A::foo(int, int)" << std::endl;
    }
};

struct B : A
{
    using A::foo;
    int foo(int)
    {
        std::cout << "B::foo(int)" << std::endl;
    }

    int foo(int) const
    {
        std::cout << "B::foo(int) const" << std::endl;
    }

    void operator*(int) const
    {
        std::cout << "operator*" << std::endl;
    }
};

void operator*(int a, const B& b)
{
    b * a;
}

void operator+(A&,A&)
{
    std::cout<< "Adding A and A" << std::endl;
}

int main()
{
    A a{7};
    a.foo(1);
    //a.foo(static_cast<short>(3));
    a.foo(1,2);

    const B b{7};
    b.foo(1);
    b.foo(static_cast<short>(3));

    A c{1};

    a + c;

    b * 3;

    3 * b;
}
