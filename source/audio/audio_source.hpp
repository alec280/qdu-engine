#pragma once
#include <AL/al.h>

namespace QDUEngine
{
    class AudioSource {
        friend class Audio;
    public:
        AudioSource() = default;
        AudioSource(ALuint source, int channelIdx);
    private:
        ALuint m_sourceID;
        int m_channelIdx;
    };
}