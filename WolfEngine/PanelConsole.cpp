#include "PanelConsole.h"
#include "Application.h"

PanelConsole::PanelConsole(bool active) : Panel("Console", active)
{
}


PanelConsole::~PanelConsole()
{
}

void PanelConsole::Draw()
{
	BROFILER_CATEGORY("PanelConsole-Draw", Profiler::Color::Azure);

	ImGui::Begin("Console", &active, ImGuiWindowFlags_ShowBorders);
	ImGui::TextUnformatted(Buf.begin());
	if (ScrollToBottom)
		ImGui::SetScrollHere(1.0f);
	ScrollToBottom = false;
	ImGui::End();
}

void PanelConsole::Clear()
{
	Buf.clear();
}

void PanelConsole::AddLog(const char* fmt)
{
	va_list args;
	va_start(args, fmt);
	Buf.appendv(fmt, args);
	va_end(args);
	ScrollToBottom = true;
}