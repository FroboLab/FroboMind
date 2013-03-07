/* 
 * File:   windows.h
 * Author: sek
 *
 * Created on 2007/10/26, 13:18
 */

#ifndef _WINDOWS_H
#define	_WINDOWS_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XWDFile.h>
#include <wchar.h>

#define CONST const

typedef void * HANDLE;
//typedef unsigned char bool8_t;
typedef const char * pcchar8_t;
//typedef pthread_mutex_t	*EV_HANDLE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef DWORD * LPDWORD;
typedef void * LPVOID;
typedef void * PVOID;
typedef unsigned char BYTE;
typedef unsigned short USHORT;
typedef unsigned long UINT;
typedef long LONG;
//typedef bool bool;
typedef BYTE BOOLEAN;
typedef unsigned char UCHAR;
typedef unsigned long UINT32;
typedef unsigned long ULONG32;
typedef ULONG32 * PULONG;
typedef unsigned long long int UINT64;
typedef UINT64 ULONG64;
typedef unsigned long ULONG;
typedef unsigned long long int ULONGLONG;
typedef bool bool8_t;
typedef void * HMODULE;
typedef void * HINSTANCE;

#define VOID void
typedef char CHAR;
typedef short SHORT;
typedef long LONG;

typedef wchar_t WCHAR;    // wc,   16-bit UNICODE character

typedef WCHAR *PWCHAR;
typedef WCHAR *LPWCH, *PWCH;
typedef CONST WCHAR *LPCWCH, *PCWCH;
typedef WCHAR *NWPSTR;
typedef WCHAR *LPWSTR, *PWSTR;

typedef CONST WCHAR *LPCWSTR, *PCWSTR;

//
// ANSI (Multi-byte Character) types
//
typedef CHAR *PCHAR;
typedef CHAR *LPCH, *PCH;

typedef CONST CHAR *LPCCH, *PCCH;
typedef CHAR *NPSTR;
typedef CHAR *LPSTR, *PSTR;
typedef CONST CHAR *LPCSTR, *PCSTR;

typedef char TCHAR, *PTCHAR;
typedef unsigned char TBYTE , *PTBYTE ;

typedef LPSTR LPTCH, PTCH;
typedef LPSTR PTSTR, LPTSTR;
typedef LPCSTR PCTSTR, LPCTSTR;


typedef union _ULARGE_INTEGER {
    struct {
	DWORD	LowPart;
	DWORD	HighPart;
    };
    struct {
	DWORD	LowPart;
	DWORD	HighPart;
    } u;
    ULONGLONG	QuadPart;
} ULARGE_INTEGER;

typedef struct _LIST_ENTRY {
    struct _LIST_ENTRY * Flink;
    struct _LIST_ENTRY * Blink;
} LIST_ENTRY;

	typedef struct tagRECT {
		long	left;
		long	top;
		long	right;
		long	bottom;
	} RECT;

	typedef struct tagSIZE {
		long	cx;
		long	cy;
	} SIZE, *PSIZE;

	typedef struct tagPOINT {
		long	x;
		long	y;
	} POINT;

	typedef Window HWND;

#define LPSTR char*
#define LPCSTR const char*
#define	LPTSTR	LPSTR
#define true true
#define false false
#define	NO_ERROR    0
#define	ERROR_ENVVAR_NOT_FOUND	203
#define	ERROR_INSUFFICIENT_BUFFER   122
#define	INVALID_HANDLE_VALUE	(-1)

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* _WINDOWS_H */

