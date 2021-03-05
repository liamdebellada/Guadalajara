#include "Utils.h"
DWORD utils::FindPattern(std::string moduleName, std::string pattern)
{
    const char* pat = pattern.c_str();
    DWORD firstMatch = 0;
    DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
    MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
    DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
    for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
    {
        if (!*pat)
            return firstMatch;

        if (*(PBYTE)pat == '?' || *(BYTE*)pCur == getByte(pat))
        {
            if (!firstMatch)
                firstMatch = pCur;

            if (!pat[2])
                return firstMatch;

            if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '?')
                pat += 3;

            else
                pat += 2;
        }
        else
        {
            pat = pattern.c_str();
            firstMatch = 0;
        }
    }

    std::cout << "failed to find pattern" << "\n";
    return NULL;
}

