#include <Windows.h>
#include <string>
#include <iostream>

#ifdef _DEBUG
#define MODULE "BWAPId.dll"
#else
#define MODULE "BWAPI.dll"
#endif

DWORD GetSingleRegString(HKEY hBaseKey, const char *pszSubKey, const char *pszValueName, char *pszOutput, DWORD *dwOutSize)
{
  HKEY hKey = NULL;
  pszOutput[0] = '\0';

  // Open the key
  DWORD dwErrCode = RegOpenKeyExA(hBaseKey, pszSubKey, 0, KEY_QUERY_VALUE, &hKey);
  if ( dwErrCode != ERROR_SUCCESS )
    return dwErrCode;

  // Query the value
  dwErrCode = RegQueryValueExA(hKey, pszValueName, NULL, NULL, (LPBYTE)pszOutput, dwOutSize);

  // Close key and return error code
  RegCloseKey(hKey);
  return dwErrCode;
}

std::string GetRegString(const char *pszSubKey, const char *pszValueName)
{
  // Declare temporary string and size
  char szTemp[MAX_PATH];
  DWORD dwSize = MAX_PATH;

  // Retrieve the key and value from HKCU
  DWORD dwErrCode = GetSingleRegString(HKEY_CURRENT_USER, pszSubKey, pszValueName, szTemp, &dwSize);
  // If it's not found, then search HKLM
  if ( dwErrCode != ERROR_SUCCESS )
    GetSingleRegString(HKEY_LOCAL_MACHINE, pszSubKey, pszValueName, szTemp, &dwSize);

  // Return the string
  return std::string(szTemp);
}

std::string GetLastErrorMessage()
{
  LPSTR pszErrMsg;
  FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, GetLastError(), 0, (LPSTR)&pszErrMsg, 0, nullptr);
  std::string result(pszErrMsg);
  LocalFree(pszErrMsg);
  return result;
}

#pragma warning(suppress: 28251)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  std::string sInstallPath = GetRegString("SOFTWARE\\Blizzard Entertainment\\Starcraft", "InstallPath");
  sInstallPath += "\\bwapi-data\\" MODULE;

  std::cout << lpCmdLine << " --> " << sInstallPath << std::endl;
  if ( !CopyFileA(lpCmdLine, sInstallPath.c_str(), FALSE) )
  {
    std::cerr << GetLastErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
