#include "audio_component.hpp"

namespace QDUEngine
{
    void AudioComponent::play()
    {
        if (m_playing || m_source.empty()) {
            return;
        }
        m_to_play = true;
    }

    void AudioComponent::setSource(const char* audioPath)
    {
        m_source = audioPath;
    }
}