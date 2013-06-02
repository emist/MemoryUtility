#include "Logger.h"
#include <cstdio>
#include <iostream>

using namespace MemUtil;
using namespace std;

void Logger::Logit(string function, string message)
{
	cout << function << ": " << message << endl;
}