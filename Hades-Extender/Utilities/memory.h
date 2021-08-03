#pragma once
#include <Psapi.h>
#include <algorithm>

typedef uintptr_t ptr;
typedef unsigned int uint;
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned long ulong;

class Memory
{
public:
	static std::string HexToBytes(std::string hex)
	{
		std::string bytes;

		hex.erase(std::remove_if(hex.begin(), hex.end(), isspace), hex.end());

		for (uint i = 0; i < hex.length(); i += 2)
		{
			if ((uchar)hex[i] == '?')
			{
				bytes += '?';
				i -= 1;

				continue;
			}

			uchar byte = (uchar)std::strtol(hex.substr(i, 2).c_str(), nullptr, 16);
			bytes += byte;
		}

		return bytes;
	}
	static ptr SigScan(const char* pattern, const char* target = XorStr("UnityPlayer.dll"))
	{

		HMODULE mod = GetModuleHandleA(target); // Maybe change secondary pattern to module name
		MODULEINFO info;
		GetModuleInformation(GetCurrentProcess(), mod, &info, sizeof(info));

		uchar* base = (uchar*)mod;

		std::string signature = HexToBytes(pattern);

		uchar first = (uchar)signature.at(0);
		uchar* end = (base + info.SizeOfImage) - signature.length();

		for (; base < end; ++base)
		{
			if (*base != first)
				continue;

			uchar* bytes = base;
			uchar* sig = (uchar*)signature.c_str();

			for (; *sig; ++sig, ++bytes)
			{
				if (*sig == '?')
					continue;

				if (*bytes != *sig)
					goto end;
			}

			return (ptr)base;

		end:;
		}

		return NULL;
	}
	static ptr GetCallAddress(const char* pattern, const char* target = "EngineWin64s.dll")
	{
		auto address = Memory::SigScan(pattern, target);
		if (!address)
		{
			return 0;
		}
		uintptr_t call_addr = (address + *(signed long*)(address + 1) + 5);
		return call_addr;
	}

	static ptr GetInstanceAddress(const char* pattern, const char* target = "EngineWin64s.dll")
	{
		auto address = Memory::SigScan(pattern, target);
		if (!address)
		{
			return 0;
		}
		return (ptr)(address + *(signed long*)(address + 3) + 7);
	}

	static void MemCpy(void* dest, void* src, size_t n)
	{
		char* csrc = (char*)src;
		char* cdest = (char*)dest;

		for (int i = 0; i < n; i++)
			cdest[i] = csrc[i];
	}
};