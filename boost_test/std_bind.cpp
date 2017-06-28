#include <iostream>
#include <functional>
using namespace std;

typedef std::function<void ()> fp;

void g_fun(){
    cout << "g_func" << endl;
}

class A {
public:
    static void A_fun_static(){
        cout << "A_fun_static" << endl;
    }

    void A_fun(){
        cout << "A_fun" << endl;
    }

    void A_fun_init(int i){
        cout << "A_fun_init " << i << endl;
    }

    void init(){
        fp fp1 = std::bind(&A::A_fun, this);
        fp1;
    }

    void init2(){
        typedef std::function<void (int)> fpi;
        fpi f = std::bind(&A::A_fun_init, this, std::placeholders::_1);
        f(5);
    }
};

int main(){
    fp f2 = fp(&g_fun);
    f2();

    fp f1 = fp(&A::A_fun_static);
    f1();

    A().init();
    A().init2();

    return 0;
}

