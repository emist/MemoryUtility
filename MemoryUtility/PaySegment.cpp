#include "PaySegment.h"
#include "Logger.h"
#include "Payload.h"

using namespace std;
using namespace MemUtil;

void PaySegment::AddPayload(DWORD address, Payload payload)
{
	payloads[address].push_back(payload); 
}

void PaySegment::Inject(HANDLE proc)
{
	Logger::Logit("PaySegment::Inject", "Injecting Segment");
	MemoryUtility::Patch(proc, payloads);
}