#include "ModuleEditor.h"
#include "Application.h"

#include <stdio.h>
#include "Glew/include/GL/glew.h"   
#include "SDL\include\SDL.h"
#include "ModuleWindow.h"
#include "JsonHandler.h"
#include "ModuleLevel.h"
#include "PanelMenuBar.h"
#include "GameObject.h"

ModuleEditor::ModuleEditor() : Module("ModuleEditor", true)
{
	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
	style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
	style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

	/*io.Fonts->AddFontFromFileTTF(XorStrA("C:\\Windows\\Fonts\\Ruda-Bold.ttf"), 12);
	io.Fonts->AddFontFromFileTTF(XorStrA("C:\\Windows\\Fonts\\Ruda-Bold.ttf"), 10);
	io.Fonts->AddFontFromFileTTF(XorStrA("C:\\Windows\\Fonts\\Ruda-Bold.ttf"), 14);
	io.Fonts->AddFontFromFileTTF(XorStrA("C:\\Windows\\Fonts\\Ruda-Bold.ttf"), 18);*/
}


ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
	ImGui_ImplSdlGL3_Init(App->window->GetWindow());
	for (int i = 0; i < size_vector; ++i)
	{
		fps_log.push_back(0);
		ms_log.push_back(0);
	}
	show_configuration = new bool(false);
	show_about = new bool(false);
	ref = new ImGuiStyle();
	
	game_objects_labels.push_back("Main Camera");
	menu = new PanelMenuBar();
	return true;
}

update_status ModuleEditor::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->GetWindow());

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float dt)
{
	//bool show_another_window = false;
	menu->Draw(ref);

	//Console();

	//MenuBar();
	
	Interface();
	
	/*if(*show_configuration)
		Configuration();

	if (*show_about)
		About();
		

	//if(show_test_window)
	//	ImGui::ShowTestWindow();*/

	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

void ModuleEditor::HandleInput(SDL_Event* event)
{
	ImGui_ImplSdlGL3_ProcessEvent(event);
}

void ModuleEditor::DrawEditor()
{
	ImGui::Render();
}

void ModuleEditor::Draw(const char* title, bool* p_opened)
{
	ImGui::Begin(title, p_opened);
	ImGui::TextUnformatted(Buf.begin());
	if (ScrollToBottom)
		ImGui::SetScrollHere(1.0f);
	ScrollToBottom = false;
	ImGui::End();
}

void ModuleEditor::AddLog(const char* fmt, ...) IM_PRINTFARGS(2)
{
	va_list args;
	va_start(args, fmt);
	Buf.appendv(fmt, args);
	va_end(args);
	ScrollToBottom = true;
}

void ModuleEditor::Console()
{
	Draw("Console");
}

/*void ModuleEditor::MenuBar()
{
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
	if (ImGui::BeginMenu("Configuration"))
	{
		if (ImGui::MenuItem("Options"))
			*show_configuration = true;
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
			*show_about = true;
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}*/

/*void ModuleEditor::Configuration()
{
	ImGui::Begin("Configuration", show_configuration);
	ImGui::Text("Options");

	if (ImGui::CollapsingHeader("Application"))
	{
		static char str0[128] = "Wolf Engine";
		static char str1[128] = "Sleepy Creepy";
		//str0 = App->window->TITLE;
		
		ImGui::InputText("App Name", str0, sizeof(str0));
		ImGui::InputText("Organization", str1, sizeof(str1));
		static int i1 = 0;
		ImGui::SliderInt("Max FPS", &i1, 0, 120);

		std::vector<float> aux_fps = fps_log;
		std::vector<float> aux_ms = ms_log;
		fps_log.clear();
		ms_log.clear();
		for (std::vector<float>::iterator it = aux_fps.begin() + 1; it != aux_fps.end(); ++it)
		{
			fps_log.push_back(*it);
		}

		for (std::vector<float>::iterator it = aux_ms.begin() + 1; it != aux_ms.end(); ++it)
		{
			ms_log.push_back(*it);
		}

		fps_log.push_back(App->frames_last_sec);

		ms_log.push_back(App->last_frame_ms);

		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fps_log.back());
		ImGui::PlotHistogram("##framerate", &fps_log.front(), fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

		sprintf_s(title, 25, "Milliseconds %0.1f", ms_log.back());
		ImGui::PlotHistogram("##milliseconds", &ms_log.front(), ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));

	}
	if (ImGui::CollapsingHeader("Window"))
	{
		ImGui::Text("Icon: *default*");
		static float brightness = 0;
		ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f);
		static int width = 0;
		ImGui::SliderInt("Width", &width, 0, 1920);
		static int height = 0;
		ImGui::SliderInt("Height", &height, 0, 1080);
		ImGui::Text("Refresh rate: %d", App->frames_last_sec);

		if (ImGui::Checkbox("Fullscreen", &fullscreen))
			App->window->FULLSCREEN = fullscreen;
		ImGui::SameLine();
		if (ImGui::Checkbox("Resizable", &resizable))
			App->window->RESIZABLE = resizable;
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Restart to apply");
		if (ImGui::Checkbox("Borderless", &borderless))
			App->window->BORDERLESS = borderless;
		ImGui::SameLine();
		if (ImGui::Checkbox("Full Desktop", &full_desktop))
			App->window->FULLSCREEN_DESKTOP = full_desktop;

		ImGui::ShowStyleEditor(ref);
	}
	if (ImGui::CollapsingHeader("Hardware"))
	{
		SDL_version version;
		SDL_GetVersion(&version);
		ImGui::Text("SDL Version: ");
		ImGui::SameLine();
		ImGui::TextColored(yellow, "%d.%d.%d", version.major, version.minor, version.patch);

		ImGui::Separator();
		ImGui::Text("CPUs: ");
		ImGui::SameLine();
		ImGui::TextColored(yellow, "%d", SDL_GetCPUCount());
		ImGui::Text("System RAM: ");
		ImGui::SameLine();
		ImGui::TextColored(yellow, "%dGB", SDL_GetSystemRAM() / 1000);
		ImGui::Text("Caps: ");
		if (SDL_HasRDTSC())
		{
			ImGui::SameLine();
			ImGui::TextColored(yellow, "RDTSC,");
		}
		if (SDL_HasMMX())
		{
			ImGui::SameLine();
			ImGui::TextColored(yellow, "MMX,");
		}
		if (SDL_HasSSE())
		{
			ImGui::SameLine();
			ImGui::TextColored(yellow, "SSE,");
		}
		if (SDL_HasSSE2())
		{
			ImGui::SameLine();
			ImGui::TextColored(yellow, "SSE2,");
		}
		if (SDL_HasSSE3())
		{
			ImGui::SameLine();
			ImGui::TextColored(yellow, "SSE3,");
		}
		if (SDL_HasSSE41())
		{
			ImGui::SameLine();
			ImGui::TextColored(yellow, "SSE41,");
		}
		if (SDL_HasSSE42())
		{
			ImGui::SameLine();
			ImGui::TextColored(yellow, "SSE42,");
		}
		if (SDL_HasAVX())
		{
			ImGui::SameLine();
			ImGui::TextColored(yellow, "AVX,");
		}

		ImGui::Separator();
		ImGui::Text("GPU: ");
		ImGui::SameLine();
	}
	ImGui::End();
}

/*void ModuleEditor::About()
{
	ImGui::Begin("About", show_about);
	ImGui::Text("Engine name: ");
	ImGui::SameLine();
	ImGui::TextColored(yellow, App->window->TITLE);
	ImGui::Text("Game engine made with C++.");
	ImGui::Spacing();
	ImGui::Text("Authors:");
	ImGui::TextColored(yellow, "Esteban Arrua, Guillem Ferre and Adrian Guevara.");

	ImGui::End();
}*/

void ModuleEditor::Interface()
{
	bool* b = new bool(true);

	ImGui::SetNextWindowPos(ImVec2(0, 20));
	ImGui::SetNextWindowSize(ImVec2(App->window->GetScreenWidth() / 5, App->window->GetScreenHeight() - App->window->GetScreenHeight() / 3 - 20));
	
	ImGui::Begin("Hierachy", b, ImVec2(App->window->GetScreenWidth()/5, App->window->GetScreenHeight()/1.58f),-1.0f,ImGuiWindowFlags_ChildWindowAutoFitX|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_ChildWindowAutoFitY|ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	
	//ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
	ImGui::Unindent(15.0f);

	static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
	static int selection_mask_child = (1 << 2);
	int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
	int child_clicked = -1;
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.
	for (int i = 0; i < game_objects_labels.size(); i++)
	{
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0);
		if (game_objects_labels[i] == "Root")
		{
			// Node
			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, App->level->GetRoot()->GetChild(0)->GetName().c_str());
			if (ImGui::IsItemClicked())
				node_clicked = i;
			if (node_open)
			{
				for (int j = 0; j < App->level->GetRoot()->GetChild(0)->GetNumChilds(); ++j)
				{
					node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask_child & (1 << j)) ? ImGuiTreeNodeFlags_Selected : 0);
					ImGui::TreeNodeEx((void*)(intptr_t)(j+i), node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, App->level->GetRoot()->GetChild(0)->GetChild(j)->GetName().c_str());
					if (ImGui::IsItemClicked())
					{
						child_clicked = j;
					}
				}
				ImGui::TreePop();
			}
		}
		else
		{
			// Leaf: The only reason we have a TreeNode at all is to allow selection of the leaf. Otherwise we can use BulletText() or TreeAdvanceToLabelPos()+Text().
			ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, game_objects_labels[i].c_str());
			if (ImGui::IsItemClicked())
				node_clicked = i;
		}
	}

	if (node_clicked != -1)
	{
		// Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
		if (ImGui::GetIO().KeyCtrl)
			selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
		else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
			selection_mask = (1 << node_clicked);           // Click to single-select
	}
	if (child_clicked != -1)
	{
		// Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
		if (ImGui::GetIO().KeyCtrl)
			selection_mask_child ^= (1 << child_clicked);          // CTRL+click to toggle
		else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
			selection_mask_child = (1 << child_clicked);           // Click to single-select
	}

	ImGui::PopStyleVar();
	//ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
	ImGui::Indent(15.0f);
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(App->window->GetScreenWidth()- App->window->GetScreenWidth()/5, 20));
	ImGui::SetNextWindowSize(ImVec2(App->window->GetScreenWidth() / 5, App->window->GetScreenHeight() - App->window->GetScreenHeight() / 3 - 20));
	ImGui::Begin("Inspector", b, ImVec2(App->window->GetScreenWidth() / 5, App->window->GetScreenHeight() / 1.58f), -1.0f, ImGuiWindowFlags_ChildWindowAutoFitX | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_ChildWindowAutoFitY | ImGuiWindowFlags_NoResize| ImGuiWindowFlags_NoCollapse);
	node_clicked = -1;
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.
	for (int i = 0; i < 1; i++)
	{
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0);
		bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Transform");
		if (ImGui::IsItemClicked())
			node_clicked = i;
		
		if (node_open)
		{
			ImGui::DragInt3("Position", new int[23, 23, 23]);
			ImGui::SameLine();
			ImGui::TreePop();
		}
	}
	if (node_clicked != -1)
	{
		// Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
		if (ImGui::GetIO().KeyCtrl)
			selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
		else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
			selection_mask = (1 << node_clicked);           // Click to single-select
	}

	ImGui::PopStyleVar();
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(0, App->window->GetScreenHeight()- App->window->GetScreenHeight() / 3));
	ImGui::SetNextWindowSize(ImVec2(App->window->GetScreenWidth(), App->window->GetScreenHeight() / 3));
	ImGui::Begin("Project", b, ImVec2(App->window->GetScreenWidth(), App->window->GetScreenHeight()/3), -1.0f, ImGuiWindowFlags_ChildWindowAutoFitX | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_ChildWindowAutoFitY | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::End();

}