// dllmain.cpp : Defines the entry point for the DLL application.

#include "imports.h"
#include <string>
const char* csgo = "Counter-Strike: Global Offensive";
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef HRESULT(__stdcall* f_EndScene)(IDirect3DDevice9* pDevice); // our function prototype 
f_EndScene oEndScene; // original endscene
       
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

WNDPROC oWndProc;

bool menuOpened = false;
int previousKey = 0;
int currentKey = 0;
bool menuHold = false;

uintptr_t gameModule; //will contain offset for client.dll

IClientEntityList* ClientEntityList = (IClientEntityList*)GetInterface("client.dll", "VClientEntityList003");
IEngineClient* EngineClient = (IEngineClient*)GetInterface("engine.dll", "VEngineClient014");
IInputSystem* InputSystem = (IInputSystem*)GetInterface("inputsystem.dll", "InputSystemVersion001");

HRESULT __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice) // our hooked endscene
{
    static bool init = true;
    if (init)
    {   
        init = false;
        gameModule = (DWORD)GetModuleHandle("client.dll");
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

        ImGui_ImplWin32_Init(FindWindowA(NULL, csgo));
        ImGui_ImplDX9_Init(pDevice);
    }

    
    currentKey = GetAsyncKeyState(VK_INSERT);
    
    if (!menuHold) {
        if (currentKey & 0x0001) {
            menuOpened = !menuOpened;
        }
    }
    else {
        if (currentKey != previousKey) {
            menuOpened = !menuOpened;
        }
    }

    previousKey = currentKey;
    ImGui::GetIO().MouseDrawCursor = menuOpened;
    if (menuOpened) {
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        //application
        menu::main();

        //render
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    }
    return oEndScene(pDevice);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (menuOpened) {
        ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam); // handle keyboard input
        return true; // block input for csgo if menu is shown
    }

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

int SetClanTag(const char* tag)
{
    static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>((DWORD)(FindPattern("engine.dll", "53 56 57 8B DA 8B F9 FF 15")));
    pSetClanTag(tag, tag);
    return 0;
}

DWORD Findpattern(std::string moduleName, std::string pattern)
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

DWORD WINAPI HackThread(HMODULE hModule) {
    std::cout << "Entity interface: " << ClientEntityList << "\n";
    std::cout << "Engine interface: " << EngineClient << "\n";
    std::cout << "Input interface: " << InputSystem << "\n";

    while (true) {
        if (menu::changedTag) {
            SetClanTag("test");
        }

        InputSystem->DisableAllInput(menuOpened);
        Ent* ent2 = (Ent*)ClientEntityList->GetClientEntity(1);
        Ent* localplayer = (Ent*)ClientEntityList->GetClientEntity(EngineClient->GetLocalPlayer());
        if (localplayer != nullptr) {
            localplayer->m_iDefaultFOV = menu::fov;
            std::cout << localplayer->m_iHealth << "\n";
            Sleep(20);
        }
    }

    return 0;
}

DWORD WINAPI MainThread(LPVOID param) {
    std::cout << "Running." <<  "\n";

    HWND  window = FindWindowA(NULL, csgo);

    oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);

    IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

    if (!pD3D)
        return false;

    D3DPRESENT_PARAMETERS d3dpp{ 0 };
    d3dpp.hDeviceWindow = window, d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD, d3dpp.Windowed = TRUE;

    IDirect3DDevice9* Device = nullptr;
    if (FAILED(pD3D->CreateDevice(0, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &Device)))
    {
        pD3D->Release();
        return false;
    }


    void** pVTable = *reinterpret_cast<void***>(Device);

    if (Device)
        Device->Release(), Device = nullptr;

    oEndScene = (f_EndScene)DetourFunction((PBYTE)pVTable[42], (PBYTE)Hooked_EndScene);
    return false;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        AllocConsole(); // enables the console
        freopen("CONIN$", "r", stdin); // makes it possible to output to console with cout.
        freopen("CONOUT$", "w", stdout);
        CreateThread(0, 0, MainThread, hModule, 0, 0);
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0);
        break; 

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
