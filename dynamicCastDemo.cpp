#include <stdio.h>

class A 
{
public:
	A(int a){
		this->a = a;
	}
	virtual void print()
	{
		printf("a: %d\n", a);
	}
	int a;
};

class B:public A 
{
public:
	B(int a, int b):A(a) {
		this->b = b;
	}
	int b;
	void print(){
		printf("b: %d\n",b);
	}
};

int main(){
	B *pB = new B(2,8);
	A *pA = dynamic_cast<A*>(pB);
//	printf("%d\n", p->b);
	if (pA == NULL) {
		printf("pA is NULL");
	} else {
		pA->print();
	}

	B *pB2 = dynamic_cast<B*>(pA);
        if (pB2 == NULL) {
                printf("pB2 is NULL");
        } else {
                pB2->print();
        }

	

	return 0;
}
