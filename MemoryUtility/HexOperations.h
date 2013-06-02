#ifndef HEXOPERATIONS
#define HEXOPERATIONS

#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>
using namespace std;

namespace MemUtil
{
	class HexOperations
	{
		public:
			static string hexadd(const string & hex1, const string & hex2);
			static string hexdecadd(const string & hex1, const string & dec1);
			static string hexdecsubstract(const string & hex1, const string & dec1);
			static string hexsubstract(const string & hex1, const string & hex2);
			static string dec2hex(long i);
			static BYTE str2byte(string byte);
			static long hex2dec(const string & hexstr);
			static long str2dec(string dec);
			static DWORD str2dword(string address);
			static void endian_swap_long(unsigned long & nLongNumber);
			static void endian_swap(unsigned __int64& x);
	};
}
#endif