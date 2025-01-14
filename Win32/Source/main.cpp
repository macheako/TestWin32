#include <win32.h>

#include <string>
#include <cstdio>
#include <iostream>
#include <strsafe.h>
#include <fstream>
#include <vector>

// https://stackoverflow.com/questions/2573834/c-convert-string-or-char-to-wstring-or-wchar-t
#include <locale>
#include <codecvt>

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

#define FILE_IN "C:\\build\\myfile.txt"

//void PrintWindowInfo(WindowInfo const&);
void ErrorExit(LPCTSTR lpszFunction);

int main(int argc, char** argv)
{
	// 27916
	PrintWindowsInfoByProcessID(33352);
	return 0;

	std::ifstream fin(FILE_IN);
	if (!fin.is_open())
	{
		std::cerr << "Error : Unable to open input file [" << FILE_IN << "]\n";
		return EXIT_FAILURE;
	}

	std::vector<std::string> windowTitles;
	std::string line;
	while (std::getline(fin, line))
	{
		if (line.size())
			windowTitles.push_back(line);
	}

	for (auto&& title : windowTitles)
	{
		std::wstring wTitle = converter.from_bytes(title);
		auto winfo = FindWindowByTitle(wTitle);

		if (winfo.Handle)
		{
			//PrintWindowInfo(winfo);

			//if (!PostMessage(winfo.Handle, WM_CLOSE, 0, 0))
			//{
			//	ErrorExit(L"PostMessage");
			//}
		}
	}

	system("pause");
	return EXIT_SUCCESS;
}

//void PrintWindowInfo(WindowInfo const& winfo)
//{
//	std::cout << "Window Info - \n"
//		<< "\tTitle:      " << converter.to_bytes(winfo.Title) << "\n"
//		<< "\tProcess ID: " << winfo.ProcessID << "\n"
//		<< "\tHandle :    " << winfo.Handle << "\n"
//		<< std::endl;
//}

void ErrorExit(LPCTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}