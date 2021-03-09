#include "imports.h"
#include "CUserCmd.h"
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

Ent* g_localplayer = nullptr;
DWORD g_clientModule = 0;

HRESULT __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice) // our hooked endscene
{
    static bool init = true;
    if (init)
    {   
        init = false;
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

typedef bool(__stdcall* CreateMoveFn)(float, CUserCmd*);
static CreateMoveFn originalFn;

bool __stdcall hkCreateMove(float sampleTime, CUserCmd* cmd)
{
    bhop();
    originalFn(sampleTime, cmd);
    if (menu::breakAngles) {
        cmd->viewangles.y = 60.f;
        return false;
    }
    else {
        return true;
    }
}

DWORD WINAPI HackThread(HMODULE hModule) {
    std::cout << "Entity interface: " << Interfaces->ClientEntityList << "\n";
    std::cout << "Engine interface: " << Interfaces->EngineClient << "\n";
    std::cout << "Input interface: " << Interfaces->InputSystem << "\n";

    uintptr_t cmad = (uintptr_t)((*(void***)Interfaces->ClientMode)[24]);

    originalFn = (CreateMoveFn)DetourFunction((PBYTE)cmad, (PBYTE)hkCreateMove);

    while (true) {
        Interfaces->InputSystem->DisableAllInput(menuOpened);

        if (!g_clientModule) {
            g_clientModule = (DWORD)GetModuleHandle("client.dll");
        }
        
        g_localplayer = (Ent*)Interfaces->ClientEntityList->GetClientEntity(Interfaces->EngineClient->GetLocalPlayer());
        
        if (g_localplayer != nullptr) {
            g_localplayer->m_iDefaultFOV = menu::fov;
        }
    }
    return 0;
}

DWORD WINAPI MainThread(LPVOID param) {
    while (!(GetModuleHandleA("serverbrowser.dll")))Sleep(200);
    Interfaces = new InterfaceCollection();
    HWND  window = FindWindowA(NULL, csgo);
    oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
    IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

    if (!pD3D)
        return false;

    D3DPRESENT_PARAMETERS d3dpp{ 0 };
    d3dpp.hDeviceWindow = window, d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD, d3dpp.Windowed = TRUE;

    //IDirect3DDevice9* Device = nullptr;
    static DWORD Device = NULL;
    while (!Device) {
        Device = **(DWORD**)(FindPattern("shaderapidx9.dll", "A1 ?? ?? ?? ?? 50 8B 08 FF 51 0C") + 0x1);
    }

    void** pVTable = *reinterpret_cast<void***>(Device);

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
