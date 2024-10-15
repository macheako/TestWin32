//#define WIN32_LEAN_AND_MEAN
//#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <WinUser.h>

#include <string>
#include <cstdio>
#include <iostream>
#include <strsafe.h>

#include <fstream>
#include <tinyxml/tinyxml.h>






#include <shlobj_core.h>
#include <direct.h>

#define DEFAULT_CONFIG_DIRNAME "UnrealBowling"

int QuitProgram(int exit)
{
	system("pause");
	return exit;
}

std::string DefaultConfigDir(HANDLE userToken = NULL)
{
	PWSTR appdata = NULL;

	// Make Win32 request for default path to Local Application Data
	if (SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_DEFAULT_PATH, userToken, &appdata) == S_OK)
	{
		char temp_cstr[MAX_PATH];
		size_t nbytes = 0;

		// Convert character(s) to non-wide format
		if (wcstombs_s(&nbytes, temp_cstr, appdata, MAX_PATH) == 0)
		{
			// Return path concatenated with default config directory name
			return std::string(temp_cstr, nbytes)
				.append("\\")
				.append(DEFAULT_CONFIG_DIRNAME);
		}
	}

	return {};
}

int main(int argc, char** argv)
{
	// "C:\Users\Crunchy\AppData\Local\UnrealBowling\KeepAlive.run.xml"
	// 
	// 
	//std::string xmlFilePath = DefaultConfigDir().append("\\KeepAlive.test.xml");
	//std::string xmlFilePath = "C:\\Users\\Crunchy\\Developer\\tmp\\testfile.xml";
	std::string xmlFilePath = "C:\\Users\\Crunchy\\AppData\\Local\\UnrealBowling\\KeepAlive.run.xml";

	//std::string xmlFilePath = DefaultConfigDir() + "\\KeepAlive.run.xml";
	std::cout << "Reading XML Document : " << xmlFilePath << std::endl;

	std::ifstream filein;
	filein.open(xmlFilePath.c_str());

	if (filein.is_open())
	{
		std::cout << "WE GOOD FAM" << std::endl;
		filein.close();
	}


	TiXmlDocument* document = new TiXmlDocument();
	if (!document->LoadFile(xmlFilePath.c_str()))
	{
		std::cerr << "UNABLE TO LOAD XML DOCUMENT" << std::endl;
		return QuitProgram(1);
	}

	std::cout << "OPENED XML DOCUMENT" << std::endl;

	return QuitProgram(0);
}