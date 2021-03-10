#include "Menu.h"
#include <windows.h>
#include "ClantagChanger.h"
int menu::fov = 90;
bool menu::changedTag = false;
bool menu::isBhop = false;
bool menu::breakAngles = false;
static char clanTagString[128] = "";

void menu::main()
{
	ImGui::Begin("Gudalajara horn");
	ImGui::Text("Saddle text");
	ImGui::SliderInt("Fov", &menu::fov, -180, 180);
	ImGui::InputText("Clantag text", clanTagString, IM_ARRAYSIZE(clanTagString));
	if (ImGui::Button("Update tag")) ClantagChanger::SetClanTag((const char *)clanTagString);
	ImGui::Checkbox("Bhop", &menu::isBhop);
	ImGui::Checkbox("Texas Rodeo", &menu::breakAngles);
	ImGui::End();
}

void menu::secondary() {
	ImGui::Begin("Gudalajara horn 2");
	ImGui::Text("Window2");
	ImGui::End();
}
