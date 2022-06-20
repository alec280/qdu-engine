#pragma once
#include <iostream>
#include <cassert>
#include <vector>
#include <AL/alc.h>
#include <dr_wav.h>
#include <chrono>
#include "glm/vec3.hpp"
#include "../game_object/audio_component.hpp"
#include "../scene/scene.hpp"
#include "audio_source.hpp"

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
        struct SourceEntry {
            ALuint m_sourceID;
            int m_nextFreeIdx;
            SourceEntry(ALuint source, int nextFreeIndex) : m_sourceID(source), m_nextFreeIdx(nextFreeIndex) {}
        };
        ALCcontext* m_audioContext;
        ALCdevice* m_audioDevice;
        std::vector<SourceEntry> m_channels;
        int m_firstFreeChannelIdx;
        float m_masterVolume;
        void end() noexcept;
        AudioSource getNextFreeSource();
        bool load_wav_file(const char* filename, ALuint bufferId) const;
        void start();
        void update(Scene* scene);
        static void updateListener(const Vector3D& position, const Vector3D& frontVector, const Vector3D& upVector);
    };
}