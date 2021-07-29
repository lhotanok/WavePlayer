#include "FileHandler.h"

PWSTR FileHandler::GetOpenedFilePath()
{
    PWSTR filePath = NULL;
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        CComPtr<IFileOpenDialog> openDialog; // smart COM ptr

        // Create the FileOpenDialog object.
        hr = openDialog.CoCreateInstance(__uuidof(FileOpenDialog));

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = openDialog->Show(NULL); // status code, non-negative number means success

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                CComPtr<IShellItem> selectedFile;
                hr = openDialog->GetResult(&selectedFile);
                if (SUCCEEDED(hr))
                {
                    PWSTR filePath;
                    hr = selectedFile->GetDisplayName(SIGDN_FILESYSPATH, &filePath);

                    if (SUCCEEDED(hr))
                    {
                        return filePath;
                    }
                }
            }
        }
        CoUninitialize();
    }
    return NULL;
}
std::wstring FileHandler::ConvertToWstring(PWSTR ptr)
{
    std::wstringstream wSs;
    wSs << ptr;
    return wSs.str();
}
