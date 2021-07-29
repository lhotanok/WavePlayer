#pragma once

#include <windows.h>
#include <shobjidl.h> 
#include <atlbase.h>

#include <string>
#include <sstream>

class FileHandler {
public:
    static PWSTR GetOpenedFilePath();
    static std::wstring ConvertToWstring(PWSTR ptr);
};