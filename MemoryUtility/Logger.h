#ifndef LOGGER
#define LOGGER

#include <cstdlib>
#include <string>



using namespace std;

namespace MemUtil
{
	class Logger
	{
		public:
			static void Logit(string function, string message);
	};
}

#endif