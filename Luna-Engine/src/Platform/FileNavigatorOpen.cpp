#include "FileNavigatorOpen.h"
#include <windows.h>
#include <shobjidl.h> // IFileDialog
#include <combaseapi.h> // CoCreateInstance
#include <string>

std::string FileNavigatorOpen::OpenFileDialog()
{
    std::string result;

    // Initialize COM
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog* pFileOpen = nullptr;
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_PPV_ARGS(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            hr = pFileOpen->Show(NULL);
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath = nullptr;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    if (SUCCEEDED(hr) && pszFilePath)
                    {
                        // convert wide string to std::string
                        char buffer[MAX_PATH];
                        WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1,
                            buffer, MAX_PATH, NULL, NULL);
                        result = buffer;
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }

    return result;
}
