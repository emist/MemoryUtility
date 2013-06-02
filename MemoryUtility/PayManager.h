#ifndef PAYMANAGER
#define PAYMANAGER

#include <vector>
#include "PaySegment.h"

using namespace std;

/*
/
/	Payload segment utility class	
/
*/

namespace MemUtil
{
	class PayManager
	{
		public:
			HANDLE target_process;
			vector<PaySegment> segments;
			//inject all segments
			PayManager(HANDLE proc)
			{
				target_process = proc;
			}
			void Inject();
			//add segment to list
			void AddSegment(PaySegment seg);
	};
}

#endif