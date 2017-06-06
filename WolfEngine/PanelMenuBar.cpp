#include <Windows.h>
#include "Application.h"
#include "ModuleEditor.h"
#include "Panel.h"
#include "PanelConsole.h"
#include "PanelMenuBar.h"
#include "Interface.h"
#include "PanelAbout.h"
#include "PanelConfiguration.h"
#include "Globals.h"
#include "ModuleLevel.h"
#include "Primitive.h"
#include "GameObject.h"

PanelMenuBar::PanelMenuBar(bool active) : Panel("Menu", active)
{
	config = new PanelConfiguration();
	about = new PanelAbout();
}


PanelMenuBar::~PanelMenuBar()
{
	RELEASE(config);
	RELEASE(about);
}

void PanelMenuBar::Draw()
{
	BROFILER_CATEGORY("PanelMenuBar-Draw", Profiler::Color::Azure);

	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		//ShowExampleMenuFile();
		//ImGui::MenuItem("(dummy menu)", NULL, false, false);
		if (ImGui::MenuItem("New")) {}
		if (ImGui::MenuItem("Open", "Ctrl+O")) {}
		if (ImGui::BeginMenu("Open Recent"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Save", "Ctrl+S")) {}
		if (ImGui::MenuItem("Save As..")) {}
		ImGui::Separator();
		if (ImGui::BeginMenu("Options"))
		{
			static bool enabled = true;
			ImGui::MenuItem("Enabled", "", &enabled);
			ImGui::BeginChild("child", ImVec2(0, 60), true);
			for (int i = 0; i < 10; i++)
				ImGui::Text("Scrolling Text %d", i);
			ImGui::EndChild();
			static float f = 0.5f;
			static int n = 0;
			ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
			ImGui::InputFloat("Input", &f, 0.1f);
			ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Colors"))
		{
			for (int i = 0; i < ImGuiCol_COUNT; i++)
				ImGui::MenuItem(ImGui::GetStyleColName((ImGuiCol)i));
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Disabled", false)) // Disabled
		{
			IM_ASSERT(0);
		}
		if (ImGui::MenuItem("Checked", NULL, true)) {}
		if (ImGui::MenuItem("Quit", "Alt+F4")) {}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::MenuItem("Options", "1", &(config->active)))
			!config->active;
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("GameObject"))
	{
		if (ImGui::MenuItem("Create Empty"))
			App->level->CreateGameObject("GameObject");
		if (ImGui::BeginMenu("3D Object"))
		{
			if (ImGui::MenuItem("Cube"))
				App->level->CreateGameObject("Resources/Default.png", PrimitiveCube(float3::one, float3(0.0f, 0.0f, 0.0f)), "Cube");
			if (ImGui::MenuItem("Sphere"))
				App->level->CreateGameObject("Resources/Default.png", PrimitiveSphere(1.0f, float3(0.0f, 0.0f, 0.0f)), "Sphere");
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("UI"))
		{
			if (ImGui::MenuItem("Text"))
			{
				GameObject* canvas = nullptr;
				if (canvas_created)
				{
					canvas = App->level->GetRoot()->FindByName("Canvas");
				}
				else
				{
					canvas = App->level->CreateGameObject("Canvas");
					canvas->CreateComponent(Component::Type::CANVAS);
					canvas_created = true;
				}
				GameObject* go = App->level->CreateGameObject("Text", canvas, canvas);
				go->CreateComponent(Component::Type::RECT_TRANSFORM);
				go->CreateComponent(Component::Type::TEXT);
			}

			if (ImGui::MenuItem("Image"))
			{
				GameObject* canvas = nullptr;
				if (canvas_created)
				{
					canvas = App->level->GetRoot()->FindByName("Canvas");
				}
				else
				{
					canvas = App->level->CreateGameObject("Canvas");
					canvas->CreateComponent(Component::Type::CANVAS);
					canvas_created = true;
				}
				GameObject* go = App->level->CreateGameObject("Image", canvas, canvas);
				go->CreateComponent(Component::Type::RECT_TRANSFORM);
				go->CreateComponent(Component::Type::IMAGE);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("View"))
	{
		if (ImGui::MenuItem("Console", "1", &(App->editor->console->active)))
			!App->editor->console->active;

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Gui Demo"))
			show_test_window = !show_test_window;
		if (ImGui::MenuItem("Documentation"))
		{
			const char* urlD = "https://github.com/GuillemFP/WolfEngine/wiki";
			ShellExecuteA(NULL, "open", "chrome", urlD, NULL, SW_MAXIMIZE);
		}
		if (ImGui::MenuItem("Download release"))
		{
			const char* urlD = "https://github.com/GuillemFP/WolfEngine/releases";
			ShellExecuteA(NULL, "open", "chrome", urlD, NULL, SW_MAXIMIZE);
		}
		if (ImGui::MenuItem("Report a bug"))
		{
			const char* urlD = "https://github.com/GuillemFP/WolfEngine/issues";
			ShellExecuteA(NULL, "open", "chrome", urlD, NULL, SW_MAXIMIZE);
		}
		if (ImGui::MenuItem("About"))
			about->active = true;
		ImGui::EndMenu();
	}

	if (config->active)
		config->Draw();
	
	if (about->active)
		about->Draw();

	if (show_test_window)
		ImGui::ShowTestWindow();

	ImGui::EndMainMenuBar();
}