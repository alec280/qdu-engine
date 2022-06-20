#pragma once
#include <cassert>
#include <AL/al.h>
#include <dr_wav.h>
#include <string>
#include <iostream>
#include <vector>
#include <filesystem>

#define OPENALCALL(function)\
	function;\
	{\
		ALenum error = alGetError(); \
		if (error != AL_NO_ERROR) {  \
            std::cout << "[Engine] OpenAL Error: " << alGetString(error) << std::endl; \
        }}

namespace QDUEngine
{
    class AudioStream {
    public:
        AudioStream() = default;
        explicit AudioStream(const char* filename);
        explicit AudioStream(const std::string& filename);
        explicit AudioStream(const std::filesystem::path& filename);
        ~AudioStream();
        void clear();
        ALuint getBufferId() const;
        std::string& getSource();
        float getTotalTime() const;
    private:
        struct WavData {
            unsigned int channels = 0;
            unsigned int sampleRate = 0;
            drwav_uint64 totalPCMFrameCount = 0;
            std::vector<uint16_t> pcmData;
            drwav_uint64 getTotalSamples() const;
        };
        ALuint m_alBufferId = 0;
        int m_channels = 0;
        int m_sampleRate = 0;
        std::string m_source;
        float m_totalTime = 0.f;
    };
}