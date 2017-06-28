#include <iostream>
#include <functional>
using namespace std;

typedef std::function<void ()> fp;

class A {
public:
    virtual void f(){
        cout << "A::f()" << endl;
    }

    void init(){
        fp f = std::bind(&A::f, this);
        f();
    }
};

class B: public A {
public:
    virtual void f() {
        cout << "B::f()" <<endl;
    }
};

int main(){
    A* pa = new B;
    pa->init();

    return 0;
}