#ifndef PAYLOAD
#define PAYLOAD

#include <cstdio>
#include <vector>
#include <Windows.h>

using namespace std;

namespace MemUtil
{
	class Payload
	{
		public:
			vector<BYTE> pay;
			Payload(string payload);
			Payload(){};
			void BuildPayload(string payload);	
			void ClearPayload();
	};
}

#endif