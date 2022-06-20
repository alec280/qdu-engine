#include "audio_source.hpp"

namespace QDUEngine
{
    AudioSource::AudioSource(ALuint source, int channelIdx) : m_sourceID(source), m_channelIdx(channelIdx) {}
}