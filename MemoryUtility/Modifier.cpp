#include "Modifier.h"
#include "hexoperations.h"
#include "Logger.h"
#include "Payload.h"

using namespace std;
using namespace MemUtil;

BOOL MemoryUtility::SetPrivilege(HANDLE hToken,LPCTSTR lpszPrivilege,BOOL bEnablePrivilege) 
{
        TOKEN_PRIVILEGES tp;
        LUID luid;

        if ( !LookupPrivilegeValue(NULL,lpszPrivilege,&luid ) )// receives LUID of privilege
        {
			printf("LookupPrivilegeValue error: %u\n", GetLastError() ); 
            return FALSE; 
        }

        tp.PrivilegeCount = 1;
        tp.Privileges[0].Luid = luid;
        if (bEnablePrivilege)
                tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        else
                tp.Privileges[0].Attributes = 0;

        // Enable the privilege or disable all privileges.

        if (!AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(TOKEN_PRIVILEGES),(PTOKEN_PRIVILEGES) NULL,(PDWORD) NULL) )
        { 
			printf("AdjustTokenPrivileges error: %u\n", GetLastError() ); 
            return FALSE; 
        } 

        if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
        {
			printf("The token does not have the specified privilege. \n");
			return FALSE;
        } 

        return TRUE;
}

int MemoryUtility::iGetDebugPrivilege ( void ) 
{     
	HANDLE hToken;
	TOKEN_PRIVILEGES CurrentTPriv;
	LUID luidVal;
	
	if ( OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken) == FALSE )
		return 0;    

	if ( LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luidVal) == FALSE )     
	{    
		CloseHandle( hToken );
		return 0; 
	}     
	
	CurrentTPriv.PrivilegeCount = 1; 
	CurrentTPriv.Privileges[0].Attributes  = SE_PRIVILEGE_ENABLED;
	CurrentTPriv.Privileges[0].Luid = luidVal; 
	int iRet = AdjustTokenPrivileges(hToken, FALSE, &CurrentTPriv, sizeof( TOKEN_PRIVILEGES ), NULL, NULL);
	CloseHandle(hToken); 
	return iRet; 
}

HANDLE MemoryUtility::AttachToProcess(DWORD ProcessId, bool killonexit)
{
	int err = 0;

	if(!iGetDebugPrivilege())
	{
		Logger::Logit("MemoryUtility::AttachToProcess", "Failed to set debug privileges");
	}
	
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_CREATE_THREAD, FALSE, ProcessId);
	if(hProcess == NULL)
	{
		Logger::Logit("MemoryUtility::AttachToProcess", "Cannot open process");
	}
	
	return hProcess;
}

LPVOID MemoryUtility::ReadMemory(HANDLE proc, DWORD address,SIZE_T size, SIZE_T * bytesRead)
{
	BYTE * buf = new BYTE[size];

	for(unsigned int i = 0; i < size; i++)
	{
		BOOL couldread = ReadProcessMemory(proc, (void*) (address+i), &buf[i], 1, bytesRead);

		if(!couldread)
		{
			Logger::Logit("MemoryUtility::ReadMemory", "Error reading memory");
		}
	}

	return buf;
}

BOOL MemoryUtility::WritePaySegment(HANDLE proc, map<DWORD, vector<Payload>> paysegment)
{
	int ret = 0;
	for(map<DWORD, vector<Payload>>::iterator iter = paysegment.begin(); iter != paysegment.end(); iter++)
	{
		DWORD addr = iter->first;
		stringstream ss;
		string addrstr;
		vector<Payload> payload(iter->second);
		Logger::Logit("MemoryUtility::WritePaySegment", "Writting Segment");
		for(vector<Payload>::iterator pay_iter = payload.begin(); pay_iter != payload.end(); pay_iter++)
		{
			for(unsigned int i = 0; i < pay_iter->pay.size(); i++)
			{
				ret = MemoryUtility::WriteMemory(proc, (LPVOID)addr, &pay_iter->pay[i], 1, NULL);
				ss.clear();
				ss << hex << iter->first;
				ss >> hex >> addrstr;
				if(ret != 1)
				{
					Logger::Logit("MemoryUtility::WritePaySegment", "Error Writting Byte");
					return ret;
				}
				ss.clear();	

				addr = HexOperations::str2dword(HexOperations::hexdecadd(addrstr, "1"));
			}
		}
	}
	return ret;
}

BOOL MemoryUtility::WritePayload(HANDLE proc, Payload payload, DWORD address)
{
	stringstream ss;
	string addrstr;
	string offsetstr;
	bool ret;
	unsigned long addr = 0;
	addr = (unsigned long) address;
	
	Logger::Logit("MemoryUtility::WriteOpcode", "Address=" + std::to_string(static_cast<long long>(addr)));
	Logger::Logit("MemoryUtility::WriteOpcode", "payload size=" + std::to_string(static_cast<long long>(payload.pay.size())));
	for(unsigned int i = 0; i < payload.pay.size(); i++)
	{
		ret = MemoryUtility::WriteMemory(proc, (LPVOID)addr, &payload.pay[i], 1, NULL);
		ss.clear();
		ss << hex << addr;
		ss >> hex >> addrstr;
		if(ret != 1)
		{
			Logger::Logit("MemoryUtility::WriteOpcode", "Error Writting Byte");
			return ret;
		}
		ss.clear();	

		addr = HexOperations::str2dword(HexOperations::hexdecadd(addrstr, "1"));
	}

	return ret;
}

BOOL MemoryUtility::Patch(HANDLE proc, map<DWORD, vector<Payload>> paysegment)
{
	Logger::Logit("MemoryUtility::Patch", "Paysegment size=" + std::to_string(static_cast<long long>(paysegment.size())));
	return WritePaySegment(proc, paysegment);
}

BOOL MemoryUtility::Patch(HANDLE proc, vector<Payload> payloads, DWORD address)
{
	unsigned long value = 0;
	unsigned long addr = 0;
	int offset = 0;
	stringstream ss;
	string hextest;
	string dectest;
	addr = (unsigned long)address;
	for(unsigned int i = 0; i < payloads.size() ; i++)
	{

		Logger::Logit("MemoryUtility::Patch", "Payloads size=" + std::to_string(static_cast<long long>(payloads.size())));
		Logger::Logit("MemoryUtility::Patch", "pay size=" + std::to_string(static_cast<long long>(payloads[i].pay.size())));
		Logger::Logit("MemoryUtility::Patch", "Address=" + std::to_string(static_cast<long long>(address)));
		WritePayload(proc,payloads[i], addr);
		offset+=payloads[i].pay.size();

		ss.clear();
		ss << address;
		ss >> hextest;
		ss.clear();
		ss << offset;
		ss >> dectest;

		addr = HexOperations::str2dword(HexOperations::hexdecadd(hextest, dectest));
	}
	return true;
}

BOOL MemoryUtility::WriteMemory(HANDLE proc, LPVOID address, LPCVOID buffer, SIZE_T size, SIZE_T * byteswritten)
{
	BOOL wrote = false;
	unsigned long hold = NULL;
	
    VirtualProtectEx(proc,(void*)address,size, PAGE_EXECUTE_READWRITE, &hold);
	if(!(wrote = WriteProcessMemory(proc, address, buffer, size, byteswritten)))
	{
		Logger::Logit("MemoryUtility::WriteMemory", "Memory Write Failed");
		cout << GetLastError() << endl;
	}

	VirtualProtectEx(proc,(void*)address, size, hold, NULL);

	return wrote;
}
/*
unsigned long ReadMemory(HANDLE proc, DWORD address,SIZE_T size, SIZE_T * bytesRead)
{
	unsigned long buffer = 0;
	//BOOL couldread = ReadProcessMemory(proc, (void *)0x4C55DA , &buffer, size, bytesRead);
	BOOL couldread = ReadProcessMemory(proc, (void*) address, &buffer, size, bytesRead);
	if(!couldread)
	{
		cout << "Error Reading Memory" << endl;
	}

	return buffer;
}
*/

DWORD MemoryUtility::GetModuleBase(LPCSTR lpModuleName, DWORD dwProcessId)
{
	MODULEENTRY32 lpModuleEntry = {0}; 

	HANDLE hSnapShot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwProcessId ); 

	if(!hSnapShot)
		return NULL; 

	lpModuleEntry.dwSize = sizeof(lpModuleEntry);

	BOOL bModule = Module32First( hSnapShot, &lpModuleEntry ); 

	while(bModule)
	{
		if(!strcmp( lpModuleEntry.szModule, lpModuleName ) )
		{
			CloseHandle( hSnapShot ); 
			
			return (DWORD)lpModuleEntry.modBaseAddr; 
		}

		bModule = Module32Next( hSnapShot, &lpModuleEntry );
	}
		
	CloseHandle( hSnapShot ); 

	return NULL; 
}

DWORD MemoryUtility::FindProcessId(std::string processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if ( processesSnapshot == INVALID_HANDLE_VALUE )
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if ( !processName.compare(processInfo.szExeFile) )
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while ( Process32Next(processesSnapshot, &processInfo) )
	{
		if ( !processName.compare(processInfo.szExeFile) )
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}
	
	CloseHandle(processesSnapshot);
	return 0;
}