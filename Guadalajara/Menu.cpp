#include "Menu.h"
#include <windows.h>
int menu::fov = 90;
bool menu::changedTag = false;

void menu::main()
{
	//application
	ImGui::Begin("Gudalajara horn");
	ImGui::Text("Saddle text");
	ImGui::SliderInt("Fov", &menu::fov, -180, 180);
	if (ImGui::Button("Update Tag")) {
		menu::changedTag = true;
		Sleep(20);
		menu::changedTag = false;
	}
	ImGui::ShowDemoWindow();
	ImGui::End();
}
