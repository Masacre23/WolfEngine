#include "ComponentAudioListener.h"
#include "Interface.h"

ComponentAudioListener::ComponentAudioListener(GameObject * parent) : Component(Component::Type::AUDIO_LISTENER, parent)
{
}

ComponentAudioListener::~ComponentAudioListener()
{
}

bool ComponentAudioListener::OnEditor()
{
	if (ImGui::CollapsingHeader("Audio Listener"))
	{
		ImGui::InputFloat("Distance", &distance);
		ImGui::SliderFloat("Roll Out", &roll_out, 1.0f, 10.0f);
		ImGui::SliderFloat("Doppler", &doppler, 1.0f, 10.0f);
	}

	return ImGui::IsItemClicked();
}

void ComponentAudioListener::SaveComponent()
{
}

void ComponentAudioListener::RestoreComponent()
{
}
