#include "PayManager.h"
#include "Logger.h"

using namespace MemUtil;

void PayManager::Inject()
{
	for(vector<PaySegment>::iterator iter = segments.begin(); iter < segments.end(); iter++)
	{
		Logger::Logit("PayManager::Inject", "Injecting PaySegment");
		Logger::Logit("PayManager::Inject", "Target Process= " + std::to_string(((long long)(target_process))));
		iter->Inject(target_process);
	}
}

void PayManager::AddSegment(PaySegment seg)
{
	segments.push_back(seg);
}
