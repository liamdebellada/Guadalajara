#include "ClantagChanger.h"
int ClantagChanger::SetClanTag(const char* tag)
{
    static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>((DWORD)(FindPattern("engine.dll", "53 56 57 8B DA 8B F9 FF 15")));
    pSetClanTag(tag, tag);
    return 0;
}
