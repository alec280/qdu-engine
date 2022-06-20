#pragma once
#include <iostream>
#include <cassert>
#include <vector>
#include <AL/al.h>
#include <AL/alc.h>
#include <dr_wav.h>
#include <chrono>
#include "../game_object/audio_component.hpp"
#include "../scene/scene.hpp"

#define OPENALCALL(function)\
	function;\
	{\
		ALenum error = alGetError(); \
		if (error != AL_NO_ERROR) {  \
            std::cout << "OpenAL Error" << std::endl; \
        }}


namespace QDUEngine
{
    class Audio {
        friend class Application;
    public:
        void play2D(const char* file);
    private:
        struct OpenALSourceArrayEntry {
            ALuint m_sourceID;
            uint32_t m_nextFreeIndex;
            OpenALSourceArrayEntry(ALuint source, uint32_t nextFreeIndex) :
                    m_sourceID(source), m_nextFreeIndex(nextFreeIndex) {}
        };
        ALCcontext* m_audioContext;
        ALCdevice* m_audioDevice;
        std::vector<OpenALSourceArrayEntry> m_openALSources;
        uint32_t m_firstFreeOpenALSourceIndex;
        uint32_t m_channels;
        //std::vector<FreeAudioSource> m_freeAudioSources;
        float m_masterVolume;
        void end() noexcept;
        bool load_wav_file(const char* filename, ALuint bufferId) const;
        void start();
        void update(Scene* scene);
    };
}