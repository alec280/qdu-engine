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
        int m_lastRequestCount = 0;
        std::map<std::string, std::shared_ptr<AudioStream>> m_loadedComponents;
        float m_masterVolume;
        void assignChannel(std::shared_ptr<AudioComponent>& component, int requestCount);
        void clear();
        void end() noexcept;
        void freeChannel(int channelIdx);
        std::shared_ptr<AudioComponent> getAudio(const char* audioPath);
        AudioSource getNextFreeChannel();
        static std::shared_ptr<AudioComponent> makeAudioPtr(std::shared_ptr<AudioStream>& ptr, std::string& source);
        void removeRedundantSources(const Vector3D& listenerPosition, std::vector<std::shared_ptr<AudioComponent>>& components);
        void removeSource(int index);
        void start();
        void stopAll(Scene* scene);
        void update(Scene* scene, float timeStep);
        static void updateAudioComponents(float timeStep, std::vector<std::shared_ptr<AudioComponent>>& components);
        static void updateListener(const Vector3D& position, const Vector3D& frontVector, const Vector3D& upVector);
    };
}