#pragma once
#include <cstdint>

VOID InitializeConsole(const char* title)
{
	FILE* pNewStdout = NULL;
	FILE* pNewStderr = NULL;
	FILE* pNewStdin = NULL;
	AllocConsole();
	SetConsoleTitleA(title);
	//// Assign `stdin`, `stout`, `stderr`
	::freopen_s(&pNewStdin, XorStr("CONIN$"), "r", stdin);
	::freopen_s(&pNewStdout, XorStr("CONOUT$"), "w", stdout);
	::freopen_s(&pNewStderr, XorStr("CONOUT$"), "w", stderr);
}

#define _DEBUG_MODE
#ifdef _DEBUG_MODE
#define DEBUG( text, ... ) printf(XorStr(text), __VA_ARGS__ )
#define DEBUG_INIT( title ) InitializeConsole(title)
#else
#define DEBUG(...) {}
#define DEBUG_INIT(...) {}
#endif // _DEBUG_MODE


// Classes
#define STR_MERGE_IMPL(X, Y)					X##Y
#define STR_MERGE(X, Y)							STR_MERGE_IMPL(X, Y)
#define MAKE_PAD(Size)							BYTE STR_MERGE(pad_, __COUNTER__) [ Size ]
//#define DEFINE_MEMBER_CLASS
//#define DEFINE_MEMBER_CLASS_INHERITANCE
#define DEFINE_MEMBER(Type, Name)				Type Name
#define DEFINE_MEMBER_0(Offset, X)				X
#define DEFINE_MEMBER_N(Offset, X)				struct { MAKE_PAD(Offset); X; }
#define DEFINE_MEMBER_B(Type, Name)				DEFINE_MEMBER_N(DEFINE_MEMBER_CLASS::Name, Type Name)
#define DEFINE_MEMBER_H(Type, Name)				DEFINE_MEMBER_N(DEFINE_MEMBER_CLASS::Name - sizeof(DEFINE_MEMBER_CLASS_INHERITANCE), Type Name)
#define DEFINE_MEMBER_H_ARRAY(Type, Name, Count)DEFINE_MEMBER_N(DEFINE_MEMBER_CLASS::Name - sizeof(DEFINE_MEMBER_CLASS_INHERITANCE), Type Name[Count])