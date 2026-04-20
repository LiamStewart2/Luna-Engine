#include "FileNavigation.h"

#include <windows.h>
#include <shobjidl.h>
#include <combaseapi.h>
#include <string>
#include "ShlObj_core.h"

std::string FileNavigation::OpenFileDialog(const std::vector<FileTypeFilter>& filters, int defaultIndex)
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
            std::vector<COMDLG_FILTERSPEC> specs;
            specs.reserve(filters.size());

            for (const FileTypeFilter& f : filters)
                specs.push_back({f.name.c_str(), f.extensions.c_str()});

            if (!specs.empty())
            {
                pFileOpen->SetFileTypes(static_cast<UINT>(specs.size()), specs.data());

                if(defaultIndex >= 1 && defaultIndex <= (int)specs.size())
                    pFileOpen->SetFileTypeIndex(defaultIndex);

                const std::wstring& extensions = filters[defaultIndex - 1].extensions;
                size_t pos = extensions.find(L"*");
                if (pos != std::wstring::npos)
                {
                    size_t dot = extensions.find(L".");
                    if (dot != std::wstring::npos)
                    {
                        std::wstring defaultExtension = extensions.substr(dot + 1, extensions.find(L";", dot) - dot - 1);
                        pFileOpen->SetDefaultExtension(defaultExtension.c_str());
                    }
                }
            }

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

std::string FileNavigation::SaveAsFileDialog(const std::vector<FileTypeFilter>& filters, int defaultIndex)
{
    std::string result;


    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    if (SUCCEEDED(hr))
    {
        IFileSaveDialog* pFileSave = nullptr;
        hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pFileSave));

        if (SUCCEEDED(hr))
        {
            std::vector<COMDLG_FILTERSPEC> specs;
            specs.reserve(filters.size());

            for (const FileTypeFilter& f : filters)
                specs.push_back({ f.name.c_str(), f.extensions.c_str() });

            if (!specs.empty())
            {
                pFileSave->SetFileTypes(static_cast<UINT>(specs.size()), specs.data());

                if(defaultIndex >= 1 && defaultIndex <= (int)specs.size())
                    pFileSave->SetFileTypeIndex(defaultIndex);

                const std::wstring& extensions = filters[defaultIndex - 1].extensions;
                size_t pos = extensions.find(L"*");
                if (pos != std::wstring::npos)
                {
                    size_t dot = extensions.find(L".");
                    if (dot != std::wstring::npos)
                    {
                        std::wstring defaultExtension = extensions.substr(dot + 1, extensions.find(L";", dot) - dot - 1);
                        pFileSave->SetDefaultExtension(defaultExtension.c_str());
                    }
                }
            }

            hr = pFileSave->Show(NULL);
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileSave->GetResult(&pItem);
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
            pFileSave->Release();
        }
        CoUninitialize();
    }

    return result;
}

std::string FileNavigation::BrowseDialog()
{
    std::string result;

    // Buffer to receive the display name (unused for final path but required by API)
    CHAR displayName[MAX_PATH] = {0};

    // Prepare BROWSEINFOA on the stack and zero-initialize it
    BROWSEINFOA browseInfo = {};
    browseInfo.hwndOwner = NULL;
    browseInfo.pidlRoot = NULL;
    browseInfo.pszDisplayName = displayName;
    browseInfo.lpszTitle = "Open Folder";
    browseInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS;
    browseInfo.lpfn = NULL;
    browseInfo.lParam = NULL;
    browseInfo.iImage = 0;

    // Show the dialog and get the PIDL for the selected folder
    LPITEMIDLIST pidl = SHBrowseForFolderA(&browseInfo);
    if (pidl != NULL)
    {
        CHAR path[MAX_PATH] = {0};
        // Convert the returned PIDL to a filesystem path
        if (SHGetPathFromIDListA(pidl, path))
        {
            result = path;
        }
        // Free the PIDL allocated by SHBrowseForFolderA
        CoTaskMemFree(pidl);
    }

    return result;
}