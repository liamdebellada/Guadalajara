#include "Menu.h"
#include <windows.h>
#include "ClantagChanger.h"
int menu::fov = 90;
bool menu::changedTag = false;
bool menu::isBhop = false;
static char clanTagString[128] = "";

void menu::main()
{
	//application
	ImGui::Begin("Gudalajara horn");
	ImGui::Text("Saddle text");
	ImGui::SliderInt("Fov", &menu::fov, -180, 180);
	ImGui::InputText("Clantag text", clanTagString, IM_ARRAYSIZE(clanTagString));
	if (ImGui::Button("Update tag")) ClantagChanger::SetClanTag((const char *)clanTagString);
	ImGui::Checkbox("Test", &menu::isBhop);
	ImGui::End();
}
