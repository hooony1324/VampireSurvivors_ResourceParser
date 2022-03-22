#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

int main()
{
	string str = "hello.png";
	istringstream ss(str);
	string sBuffer;
	vector<string> v;
	v.clear();

	while (getline(ss, sBuffer, '.'))
	{
		v.push_back(sBuffer);
	}

	int a = 0;
}