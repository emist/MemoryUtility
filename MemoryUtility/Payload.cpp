#include "Payload.h"
#include "hexoperations.h"
#include "Logger.h"

namespace MemUtil
{

	Payload::Payload(string payload)
	{
		BuildPayload(payload);
	}

	void Payload::BuildPayload(string payload)
	{
		for(unsigned int i = 0; i<payload.size(); i+=2)
		{
			Logger::Logit("Payload::BuildPayload", "Payload=" + payload.substr(i,2));
			pay.push_back(HexOperations::str2byte(payload.substr(i,2)));
		}
	}
	void Payload::ClearPayload()
	{
		pay.clear();
	}
}