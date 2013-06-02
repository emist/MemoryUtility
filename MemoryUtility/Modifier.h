#ifndef MODIFIER
#define MODIFIER

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <map>
#include "Payload.h"

using namespace std;

namespace MemUtil
{
	class MemoryUtility
	{
		public:
			static HANDLE AttachToProcess(DWORD ProcessId, bool killonexit);
			static DWORD FindProcessId(std::string processName);
			static DWORD GetModuleBase(LPCSTR lpModuleName, DWORD dwProcessId);
			static DWORD GetOffset(std::string exename);
			static BOOL Patch(HANDLE proc, vector<Payload> payload, DWORD address);	
			static BOOL Patch(HANDLE proc, map<DWORD, vector<Payload>> payloads);	
			static LPVOID ReadMemory(HANDLE proc, DWORD address,SIZE_T size, SIZE_T * bytesRead);
			static BOOL SetPrivilege(HANDLE hToken,LPCTSTR lpszPrivilege,BOOL bEnablePrivilege);
			static BOOL WriteMemory(HANDLE proc, LPVOID address, LPCVOID buffer, SIZE_T size, SIZE_T * byteswritten);
			//BOOL BuildOpcode(

		private:
			static BOOL WritePaySegment(HANDLE proc, map<DWORD, vector<Payload>> paysegment);
			static BOOL WritePayload(HANDLE proc, Payload payload, DWORD address);
			static int MemoryUtility::iGetDebugPrivilege ( void );
	};
}
#endif



