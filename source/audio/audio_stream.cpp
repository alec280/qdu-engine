#include "audio_stream.hpp"

namespace QDUEngine
{
    drwav_uint64 AudioStream::WavData::getTotalSamples() const
    {
        return totalPCMFrameCount * channels;
    }

    AudioStream::AudioStream(const std::string& filename) : m_source(filename)
    {
        WavData audioData;
        auto sampleData = drwav_open_file_and_read_pcm_frames_s16(filename.c_str(),
                                                                          &audioData.channels,
                                                                          &audioData.sampleRate,
                                                                          &audioData.totalPCMFrameCount,
                                                                          nullptr);
        if (!sampleData) {
            std::cout << "[Engine] Failed to load " << filename << " as AudioStream." << std::endl;
            drwav_free(sampleData, nullptr);
            m_alBufferId = 0;
        } else if (audioData.getTotalSamples() > drwav_uint64(std::numeric_limits<size_t>::max())) {
            std::cout << "[Engine] File " << filename << " is too big." << std::endl;
            drwav_free(sampleData, nullptr);
            m_alBufferId = 0;
        } else {
            audioData.pcmData.resize(size_t(audioData.getTotalSamples()));
            m_totalTime = (float)audioData.totalPCMFrameCount / (float)audioData.sampleRate;
            m_sampleRate = (int)audioData.sampleRate;
            m_channels = (int)audioData.channels;
            std::memcpy(audioData.pcmData.data(), sampleData, audioData.pcmData.size() * 2);
            drwav_free(sampleData, nullptr);
            OPENALCALL(alGenBuffers(1, &m_alBufferId));
            if (m_alBufferId == 0) {
                std::cout << "[Engine] OpenAL wasn't able to load: " << filename << std::endl;
            }
            OPENALCALL(alBufferData(m_alBufferId, audioData.channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, audioData.pcmData.data(), audioData.pcmData.size() * 2, audioData.sampleRate));
        }
    }

    AudioStream::~AudioStream()
    {
        return;
    }

    void AudioStream::clear()
    {
        if (m_alBufferId) {
            OPENALCALL(alDeleteBuffers(1, &m_alBufferId));
            m_alBufferId = 0;
        }
    }

    ALuint AudioStream::getBufferId() const
    {
        return m_alBufferId;
    }

    std::string& AudioStream::getSource()
    {
        return m_source;
    }

    float AudioStream::getTotalTime() const
    {
        return m_totalTime;
    }
}