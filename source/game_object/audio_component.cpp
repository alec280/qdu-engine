#include "audio_component.hpp"

namespace QDUEngine
{
    AudioComponent::AudioComponent(const char* filename)
    {
        setSource(filename);
    }

    void AudioComponent::clear()
    {
        m_autoPlay = false;
        m_isPlaying = false;
        m_loop = false;
        m_position = {};
        m_radius = 2.f;
        m_stream = nullptr;
        m_timeLeft = 0;
    }

    bool AudioComponent::hasSource()
    {
        if (m_stream == nullptr) {
            return false;
        }
        return !m_stream->getSource().empty();
    }

    nlohmann::json AudioComponent::getData()
    {
        return {
                {"autoPlay",   m_autoPlay},
                {"is3D",       m_is3D},
                {"isListener", m_isListener},
                {"loop",       m_loop},
                {"posX",       m_position.x},
                {"posY",       m_position.y},
                {"posZ",       m_position.z},
                {"pitch",      m_pitch},
                {"radius",     m_radius},
                {"source",     m_stream ? m_stream->getSource() : ""},
                {"timeLeft",   m_timeLeft}
        };
    }

    Vector3D AudioComponent::getPosition()
    {
        return m_position;
    }

    std::shared_ptr<AudioStream> AudioComponent::getStream()
    {
        return m_stream;
    }

    float AudioComponent::getTimeLeft()
    {
        return m_timeLeft;
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
        m_isPlaying = true;
        m_timeLeft = m_stream->getTotalTime();
    }

    void AudioComponent::setAs3D(bool value)
    {
        m_is3D = value;
    }

    void AudioComponent::setAsListener(bool value)
    {
        m_isListener = value;
    }

    void AudioComponent::setAsLooping(bool value)
    {
        m_loop = value;
    }

    void AudioComponent::setAutoPlay(bool value)
    {
        m_autoPlay = value;
    }

    void AudioComponent::setPitch(float value)
    {
        m_pitch = value;
    }

    void AudioComponent::setRadius(float value)
    {
        m_radius = value;
    }

    void AudioComponent::setSource(const char* filename)
    {
        m_stream = std::make_shared<AudioStream>(AudioStream(filename));
        m_timeLeft = m_stream->getTotalTime();
    }

    void AudioComponent::setTimeLeft(float value)
    {
        m_timeLeft = fmax(0.f, value);
    }
}