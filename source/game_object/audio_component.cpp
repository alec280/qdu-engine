#include "audio_component.hpp"

namespace QDUEngine
{
    AudioComponent::AudioComponent(const char* source)
    {
        setSource(source);
    }

    void AudioComponent::move(const Vector2D& by)
    {
        m_position += by;
    }

    void AudioComponent::move(const Vector3D& by)
    {
        m_position += by;
    }

    void AudioComponent::play()
    {
        if (m_playing || m_source.empty()) {
            return;
        }
        m_to_play = true;
    }

    void AudioComponent::setAsListener(bool value)
    {
        m_listener = value;
    }

    void AudioComponent::setSource(const char* audioPath)
    {
        m_source = audioPath;
    }
}