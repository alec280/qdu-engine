#include "audio_component.hpp"

namespace QDUEngine
{
    AudioComponent::AudioComponent(const char* filename)
    {
        setSource(filename);
    }

    AudioStream AudioComponent::getStream()
    {
        return m_audioStream;
    }

    Vector3D AudioComponent::getPosition()
    {
        return m_position;
    }

    bool AudioComponent::hasSource()
    {
        return !m_audioStream.getSource().empty();
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
        if (!hasSource()) {
            return;
        }
        m_playing = true;
    }

    void AudioComponent::setAsListener(bool value)
    {
        m_listener = value;
    }

    void AudioComponent::setSource(const char* filename)
    {
        m_audioStream = AudioStream(filename);
        m_timeLeft = m_audioStream.getTotalTime();
    }
}