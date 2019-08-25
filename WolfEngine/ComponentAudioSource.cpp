#include "ComponentAudioSource.h"

ComponentAudioSource::ComponentAudioSource(GameObject * parent) : Component(Component::Type::AUDIO_SOURCE, parent)
{
}

ComponentAudioSource::~ComponentAudioSource()
{
}

bool ComponentAudioSource::OnEditor()
{
	return false;
}

void ComponentAudioSource::SaveComponent()
{
}

void ComponentAudioSource::RestoreComponent()
{
}

void ComponentAudioSource::LoadSound(const char * sound)
{
}

void ComponentAudioSource::PlaySound()
{
}

void ComponentAudioSource::StopSound()
{
}
