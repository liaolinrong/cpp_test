//g++ boost_thread_test.cpp -lboost_system -lboost_threaad
#include <boost/thread.hpp>
#include <iostream>

void func1()
{
	for (int i = 0; i < 100; i++)
	{
		std::cout << "hello boost: " << i << std::endl;
		boost::this_thread::sleep(boost::posix_time::seconds(1));
	}
}

int main()
{
	boost::thread t(func1);
	boost::thread t2(func1);

	for(int i = 0; i < 10; i ++){
		std::cout << "main: " << i << std::endl;
		boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(1));
	}
	t.join();
	t2.join();
	return 0;
}
