#include "Logger.h"
#include <cstdio>
#include <iostream>

using namespace MemUtil;
using namespace std;


bool Logger::logging = false;

void Logger::Logit(string function, string message)
{
	if(logging)
	cout << function << ": " << message << endl;
}