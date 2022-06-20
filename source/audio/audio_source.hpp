#pragma once
#include <AL/al.h>

namespace QDUEngine
{
    class AudioSource {
    public:
        AudioSource(ALuint source, int channelIdx);
    private:
        ALuint m_sourceID;
        int m_channelIdx;
    };
}