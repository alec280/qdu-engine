#pragma once
#include <AL/alc.h>
#include <chrono>
#include "glm/vec3.hpp"
#include "../game_object/audio_component.hpp"
#include "../scene/scene.hpp"

namespace QDUEngine
{
    class Audio {
        friend class Application;
    public:
        float getMasterVolume() const;
        void play2D(const char* file);
        void setMasterVolume(float volume);
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
        void assignToChannel(std::shared_ptr<AudioComponent>& component);
        void clear();
        void end() noexcept;
        void freeChannel(int channelIdx);
        AudioSource getNextFreeChannel();
        bool loadWavFile(const char* filename, ALuint bufferId) const;
        void removeSource(int index);
        void start();
        void update(Scene* scene, float timeStep);
        static void updateAudioComponents(float timeStep, std::vector<std::shared_ptr<AudioComponent>>& components);
        static void updateListener(const Vector3D& position, const Vector3D& frontVector, const Vector3D& upVector);
    };
}