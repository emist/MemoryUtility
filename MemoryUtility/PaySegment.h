#ifndef PAYSEGMENT
#define PAYSEGMENT

#include <Windows.h>
#include <cstdio>
#include <vector>
#include <map>
#include "Modifier.h"
#include "Payload.h"

using namespace std;

/*
/
/	Payload Segments are continuous opcodes begining at a given address
/
*/


namespace MemUtil
{
	class PaySegment
	{
		map<DWORD, vector<Payload>> payloads;

		public:
			PaySegment(DWORD address, Payload payload)
			{
				payloads[address].push_back(payload);
			}

			PaySegment()
			{}

			//DWORD GetAddress();
			//void SetAddress(DWORD address);
			void AddPayload(DWORD address, Payload payload);
			void Inject(HANDLE proc);
	};
}

#endif
