#include <win32.h>

#include <cstdio>
#include <strsafe.h>

std::wstring GetWindowTitle(HWND handle)
{
	std::wstring windowTitle = L"";

	// Get the length of the window title (wchar)
	int windowTitleLength = GetWindowTextLengthW(handle);
	if (windowTitleLength)
	{
		// Allocate memory for the string and copy 
		// the string into the memory. 
		LPWSTR lpWindowTitle = (LPWSTR)VirtualAlloc((LPVOID)NULL,
			(DWORD)(windowTitleLength + 1), MEM_COMMIT,
			PAGE_READWRITE);

		if (lpWindowTitle)
		{
			GetWindowTextW(handle, lpWindowTitle, windowTitleLength + 1);
			windowTitle = lpWindowTitle;

			// Free the memory and return. 
			VirtualFree(lpWindowTitle, 0, MEM_RELEASE);
		}
	}

	return windowTitle;
}

std::wstring GetWindowClassName(HWND handle)
{
	std::wstring className = L"";

	// Get the length of the window title (wchar)
	int windowTitleLength = GetWindowTextLengthW(handle);
	if (windowTitleLength)
	{
		// Allocate memory for the string and copy 
		// the string into the memory. 
		LPWSTR lpWindowClassName = (LPWSTR)VirtualAlloc((LPVOID)NULL,
			(DWORD)(1024 + 1), MEM_COMMIT,
			PAGE_READWRITE);

		if (lpWindowClassName)
		{
			GetClassNameW(handle, lpWindowClassName, 1024 + 1);
			className = lpWindowClassName;

			// Free the memory and return. 
			VirtualFree(lpWindowClassName, 0, MEM_RELEASE);
		}
	}

	return className;
}

void PrintWindowInfo(WindowInfo const& winfo)
{

}

void PrintWindowInfo(HWND hWnd)
{
	std::wstring windowTitle = GetWindowTitle(hWnd);

	if (windowTitle.size() > 0)
	{
		DWORD threadID = 0,
			processID = 0;

		threadID = GetWindowThreadProcessId(hWnd, &processID);

		wprintf(L"--------------------------\n");
		wprintf(L"Window: %s\n\n", windowTitle.c_str());

		size_t pos = windowTitle.find_last_of('\\');
		std::wstring leafName = windowTitle.substr(pos + 1);

		std::wstring className = GetWindowClassName(hWnd);
		wprintf(L"Class Name: %s\n\n", className.c_str());

		wprintf(L"\tThreadID:   %d\n", threadID);
		wprintf(L"\tProcessID:  %d\n", processID);
		wprintf(L"\tHandle:     %d\n", hWnd);

		std::wstring isVisible = IsWindowVisible(hWnd) ? L"YES" : L"NO";

		wprintf(L"\n\tIsVisible: %s\n\n", isVisible.c_str());
	}
}

void PrintThreadDescription(HANDLE hThread)
{
	PWSTR data;
	HRESULT hr = GetThreadDescription(hThread, &data);
	if (SUCCEEDED(hr))
	{
		wprintf(L"Thread Description: %ls\n\n", data);
		LocalFree(data);
	}
}

#define BUFFER 128

BOOL CALLBACK callback_WinPropProc(
	HWND hwndSubclass,  // handle of window with property 
	LPCWSTR lpszString,  // property string or atom
	HANDLE hData)       // data handle 
{
	static int nProp = 1;    // property counter 
	TCHAR tchBuffer[BUFFER]; // expanded-string buffer 
	size_t* nSize = nullptr; // size of string in buffer 
	HDC hdc;                 // device-context handle
	HRESULT hResult = 0;

	hdc = GetDC(hwndSubclass);

	//wprintf(L"%lc", lpszString[0]);

	// Display window property string in client area.
	//hResult = StringCchPrintfW(tchBuffer, BUFFER, L"WinProp %s", lpszString);
	//hResult = StringCchPrintf(tchBuffer, BUFFER, L"WinProp %d:  %s", nProp++, lpszString);
	if (FAILED(hResult))
	{
		// TO DO: write error handler if function fails.
	}
	//hResult = StringCchLength(tchBuffer, BUFFER, nSize);
	if (FAILED(hResult))
	{
		// TO DO: write error handler if function fails.
	}
	//TextOut(hdc, 10, nProp * 20, tchBuffer, *nSize);

	ReleaseDC(hwndSubclass, hdc);

	return TRUE;
}

BOOL CALLBACK callback_EnumChildProc(HWND hwnd, LPARAM lParam)
{
	int threadID = 0,
		processID = 0;

	//threadID = GetWindowThreadProcessId()
	return TRUE;
}

BOOL CALLBACK callback_FindWindowByTitle(HWND handle, LPARAM lParam)
{
	WindowInfo* winfo = (WindowInfo*)lParam;

	std::wstring currentTitle = GetWindowTitle(handle);
	if (!winfo || currentTitle != winfo->Title)
		return TRUE;

	winfo->Handle = handle;
	GetWindowThreadProcessId(winfo->Handle, &(winfo->ProcessID));

	return FALSE;
}

BOOL CALLBACK callback_PrintWindowTitle(HWND handle, LPARAM lParam)
{
	std::wstring windowTitle = GetWindowTitle(handle);
	if (windowTitle.size() > 0)
		wprintf(L"Window Title : %s\n", windowTitle.c_str());

	return TRUE;
}

BOOL CALLBACK callback_PrintWindowAndChildren(HWND handle, LPARAM lParam)
{
	std::wstring windowTitle = GetWindowTitle(handle);
	if (windowTitle.size() > 0)
		wprintf(L"Window Title : %s\n", windowTitle.c_str());

	return TRUE;
}

BOOL CALLBACK callback_PrintWindowsInfo(HWND handle, LPARAM lParam)
{
	bool skipBlankTitles = lParam ? (bool)lParam : true;
	std::wstring windowTitle = GetWindowTitle(handle);

	if (skipBlankTitles && windowTitle.size() > 0)
		PrintWindowInfo(handle);	

	return TRUE;
}

BOOL CALLBACK callback_PrintWindowsInfoByProcessID(HWND handle, LPARAM lParam)
{
	int processID = (int)lParam;
	DWORD winProcID = 0,
		winThreadID = 0;

	winThreadID = GetWindowThreadProcessId(handle, &winProcID);
	if (winThreadID && processID == winProcID)
	{
		if (IsWindowVisible(handle))
		{
			PrintWindowInfo(handle);

			PostMessageW(handle, WM_CLOSE, NULL, NULL);
			//EnumPropsW(handle, callback_WinPropProc);
		}
		
	}		

	return TRUE;
}

BOOL CALLBACK callback_FindWindowByProcID(HWND handle, LPARAM lParam)
{
	WindowInfo* winfo = (WindowInfo*)lParam;
	unsigned long process_id = 0;

	if (!winfo)
		return FALSE;

	GetWindowThreadProcessId(handle, &process_id);
	if (winfo->ProcessID != process_id)
		return TRUE;

	winfo->Handle = handle;
	return FALSE;
}

void PrintWindowsInfo(bool printChildren)
{
	if (printChildren)
	{
		EnumWindows(callback_PrintWindowAndChildren, NULL);
	}
	else
	{
		EnumWindows(callback_PrintWindowsInfo, NULL);
	}
}

void PrintWindowsInfoByProcessID(int processID)
{
	EnumWindows(callback_PrintWindowsInfoByProcessID, processID);
}

WindowInfo FindWindowByProcessID(unsigned long process_id)
{
	WindowInfo winfo;

	winfo.ProcessID = process_id;
	winfo.Handle = (HWND)0;

	EnumWindows(callback_FindWindowByProcID, (LPARAM)&winfo);
	return winfo;
}

WindowInfo FindWindowByTitle(std::wstring const& title)
{
	WindowInfo winfo;
	winfo.Title = title;
	winfo.Handle = (HWND)0;

	EnumWindows(callback_FindWindowByTitle, (LPARAM)&winfo);
	return winfo;
}

void ShutdownProcess(unsigned long process_id)
{
	// Attempt to get process's main window handle
	auto winfo = FindWindowByProcessID(process_id);
	if (winfo.Handle)
	{
		PostMessage(winfo.Handle, WM_CLOSE, 0, 0);
	}
}