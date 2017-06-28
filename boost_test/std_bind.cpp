#include <iostream>
#include <functional>
using namespace std;

typedef std::function<void ()> fp;

void g_fun(){
    cout << "g_func" << endl;
}