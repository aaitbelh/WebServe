#include <fstream>
#include <iostream>
int main()
{
	std::ifstream file;
	file.open("resp");
	char buf[1024];
	file.read(buf, 1024);
	std::cout << file.gcount() << std::endl;
	std::cout <<buf << std::endl;

}