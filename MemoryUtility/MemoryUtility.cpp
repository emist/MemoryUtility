#include "MemoryUtility.h"
#include "PayManager.h"
#include "PaySegment.h"
#include "Modifier.h"
#include <sstream>
#include <cstdio>

using namespace std;
using namespace MemUtil;

int main(int argc, char * argv[])
{	
	std::string processName("MagmaCompiler.exe");
	
	DWORD processID = MemoryUtility::FindProcessId(processName);

	if ( processID == 0 )
		std::wcout << "Could not find " << processName.c_str() << std::endl;
	else
		std::wcout << "Process ID is " << processID << std::endl;
	
	stringstream ss;
	string offset;
	ss << hex << MemoryUtility::GetModuleBase(processName.c_str(), processID);
	ss >> hex >> offset;

	HANDLE proc = MemoryUtility::AttachToProcess(processID, false);
	if(proc == NULL)
	{
		cout << "Could not get handle to process" << endl;
		system("PAUSE");
	}
	
	//Scroll speedcheck
	PayManager payManager(proc);
	PaySegment speedCheckSegment;
	Payload speedCheckPayload;
	speedCheckPayload.BuildPayload("EB05"); //JMP SHORT 0042DCE0
	speedCheckSegment.AddPayload(0x042DCD9, speedCheckPayload);
	
	//Song length check
	Payload songTooLongOne("EB2A"); //JMP SHORT 004294FF
	Payload songTooLongTwo("EB41"); //JMP SHORT 0041F794
	PaySegment songLengthSegment;
	songLengthSegment.AddPayload(0x04294D3, songTooLongOne);
	songLengthSegment.AddPayload(0x041F751, songTooLongTwo);

	//track type check
	Payload trackTypeCheck("EB3E"); //JMP SHORT 00438B22
	PaySegment trackTypeSegment(0x0438AE2, trackTypeCheck);
	
	//release date checks
	Payload releaseDateOne("EB45"); //JMP SHORT 0042D965
	PaySegment releaseCheckSegment(0x042D91E, releaseDateOne);
	Payload releaseDateTwo("EBDA"); //JMP SHORT 0042D946
	releaseCheckSegment.AddPayload(0x042D96A, releaseDateTwo);

	payManager.AddSegment(releaseCheckSegment);
	payManager.AddSegment(trackTypeSegment);
	payManager.AddSegment(songLengthSegment);
	payManager.AddSegment(speedCheckSegment);
	payManager.Inject();

}