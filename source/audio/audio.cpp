#include "audio.hpp"

namespace QDUEngine
{
    void Audio::assignChannel(std::shared_ptr<AudioComponent>& component, int requestCount)
    {
        auto stream = component->getStream();
        if (!stream) {
            return;
        }
        if (!component->m_isAssigned) {
            auto unusedSource = getNextFreeChannel();
            if (unusedSource.m_channelIdx == -1) {
                if (requestCount != m_lastRequestCount) {
                    std::cout << "[Engine] Not enough channels available (requesting: "
                    << requestCount << ", maximum: " << m_channels.capacity() << ")." << std::endl;
                    m_lastRequestCount = requestCount;
                }
                return;
            }
            component->m_audioSource = unusedSource;
            component->m_isAssigned = true;
            if (!component->m_is3D) {
                OPENALCALL(alSourcei(unusedSource.m_sourceID, AL_SOURCE_RELATIVE, AL_TRUE));
                OPENALCALL(alSource3f(unusedSource.m_sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f));
            } else {
                auto pos = component->m_position;
                OPENALCALL(alSourcei(unusedSource.m_sourceID, AL_SOURCE_RELATIVE, AL_FALSE));
                OPENALCALL(alSource3f(unusedSource.m_sourceID, AL_POSITION, pos.x, pos.y, pos.z));
            }
            OPENALCALL(alSourcei(unusedSource.m_sourceID, AL_LOOPING, component->m_loop));
            OPENALCALL(alSourcef(unusedSource.m_sourceID, AL_PITCH, component->m_pitch));
            OPENALCALL(alSourcef(unusedSource.m_sourceID, AL_GAIN, component->m_volume));
            OPENALCALL(alSourcef(unusedSource.m_sourceID, AL_MAX_DISTANCE, component->m_radius));
            OPENALCALL(alSourcef(unusedSource.m_sourceID, AL_REFERENCE_DISTANCE, 1.0));
            OPENALCALL(alSourcei(unusedSource.m_sourceID, AL_BUFFER, stream->getBufferId()));
            OPENALCALL(alSourcef(unusedSource.m_sourceID, AL_SEC_OFFSET, stream->getTotalTime() - component->m_timeLeft));
            if (component->m_isPlaying) {
                OPENALCALL(alSourcePlay( unusedSource.m_sourceID));
            }
        }
    }

    void Audio::clear() {
        for (auto& sourceEntry : m_channels) {
            OPENALCALL(alDeleteSources(1, &(sourceEntry.m_sourceID)));
        }
        for (auto& stream : m_loadedComponents) {
            stream.second->clear();
        }
    }

    void Audio::end() noexcept
    {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(m_audioContext);
        alcCloseDevice(m_audioDevice);
    }

    void Audio::freeChannel(int channelIdx) {
        auto& sourceEntry = m_channels[channelIdx];
        if (m_firstFreeChannelIdx == m_channels.capacity()) {
            m_firstFreeChannelIdx = channelIdx;
            sourceEntry.m_nextFreeIdx = (int)m_channels.capacity();
        }
        else {
            auto& firstFreeEntry = m_channels[m_firstFreeChannelIdx];
            sourceEntry.m_nextFreeIdx = m_firstFreeChannelIdx;
            m_firstFreeChannelIdx = channelIdx;
        }
    }

    std::shared_ptr<AudioComponent> Audio::getAudio(const char* audioPath)
    {
        std::string source = audioPath;
        if (source.empty()) {
            auto audioPtr = std::make_shared<AudioComponent>(AudioComponent());
            return audioPtr;
        }
        for (auto& element : m_loadedComponents) {
            if (element.first == audioPath) {
                auto streamPtr = element.second;
                return makeAudioPtr(streamPtr, source);
            }
        }
        auto streamPtr = std::make_shared<AudioStream>(AudioStream(audioPath));
        m_loadedComponents[audioPath] = streamPtr;
        return makeAudioPtr(streamPtr, source);
    }

    AudioSource Audio::getNextFreeChannel()
    {
        if (m_firstFreeChannelIdx == m_channels.capacity()) {
            return {0, -1};
        }
        auto& entry = m_channels[m_firstFreeChannelIdx];
        int channelIdx = m_firstFreeChannelIdx;
        m_firstFreeChannelIdx = entry.m_nextFreeIdx;
        return {entry.m_sourceID, channelIdx};
    }

    float Audio::getMasterVolume() const
    {
        return m_masterVolume;
    }

    std::shared_ptr<AudioComponent> Audio::makeAudioPtr(std::shared_ptr<AudioStream>& ptr, std::string& source)
    {
        auto audioComponent = AudioComponent();
        audioComponent.m_stream = ptr;
        auto audioPtr = std::make_shared<AudioComponent>(audioComponent);
        return audioPtr;
    }

    void Audio::playAudio(const char* path, bool is3D, Vector3D pos)
    {
        auto component = getAudio(path);
        component->setAs3D(is3D);
        component->move(pos);
        component->play();
        m_oneOffAudioComponents.push_back(component);
    }

    std::vector<std::shared_ptr<AudioComponent>> Audio::removeRedundantSources(const Vector3D& listenerPosition, std::vector<std::shared_ptr<AudioComponent>>& components)
    {
        auto usefulComponents = std::vector<std::shared_ptr<AudioComponent>>{};
        for (auto& component : components) {
            if (component->m_timeLeft > 0 && component->m_isPlaying) {
                if (!component->m_is3D) {
                    usefulComponents.push_back(component);
                    continue;
                }
                auto position = component->getPosition();
                float squareRadius = component->m_radius * component->m_radius;
                float squareDistance = position.squareDistanceTo(listenerPosition);
                if (squareDistance <= squareRadius) {
                    usefulComponents.push_back(component);
                    continue;
                }
            }
            if (component->m_isAssigned) {
                removeSource(component->m_audioSource.m_channelIdx);
                component->m_isAssigned = false;
            }
        }
        return usefulComponents;
    }

    void Audio::removeSource(int channelIdx)
    {
        auto& sourceEntry = m_channels[channelIdx];
        OPENALCALL(alSourcef(sourceEntry.m_sourceID, AL_GAIN, 0.0f));
        OPENALCALL(alSourceStop(sourceEntry.m_sourceID));
        OPENALCALL(alSourcei(sourceEntry.m_sourceID, AL_BUFFER, 0));
        freeChannel(channelIdx);
    }

    void Audio::start()
    {
        const int channelAmount = 2;
        m_audioDevice = alcOpenDevice(nullptr);

        if (!m_audioDevice) {
            std::cout << "[Engine] Failed to open audio device." << std::endl;
            return;
        }
        m_audioContext = alcCreateContext(m_audioDevice, nullptr);
        if (!alcMakeContextCurrent(m_audioContext)) {
            std::cout << "[Engine] Failed to make audio context current." << std::endl;
            return;
        }

        OPENALCALL(alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED));
        m_masterVolume = 1.0f;
        OPENALCALL(alListenerf(AL_GAIN, m_masterVolume));

        m_channels.reserve(channelAmount);
        for (unsigned int i = 0; i < channelAmount; i++) {
            ALuint source = 0;
            OPENALCALL(alGenSources(1, &source));
            m_channels.emplace_back(source, i + 1);
        }
        m_firstFreeChannelIdx = 0;
    }

    void Audio::stopAll(Scene* scene)
    {
        for (auto& object : scene->getObjects()) {
            auto component = object->getAudioComponent();
            if (!component) {
                continue;
            }
            if (component->m_isAssigned) {
                removeSource(component->m_audioSource.m_channelIdx);
                component->m_isAssigned = false;
            }
        }
    }

    void Audio::setMasterVolume(float volume)
    {
        m_masterVolume = std::clamp(volume, 0.0f, 1.0f);
        OPENALCALL(alListenerf(AL_GAIN, m_masterVolume));
    }

    void Audio::update(Scene* scene, float timeStep)
    {
        auto listenerPosition = Vector3(0, 0, 0);
        auto audioComponents = std::vector<std::shared_ptr<AudioComponent>>{};
        for (auto& object : scene->getObjects()) {
            auto component = object->getAudioComponent();
            if (component == nullptr) {
                continue;
            }
            if (component->m_isListener) {
                listenerPosition = component->getPosition();
            }
            audioComponents.push_back(component);
        }

        for (auto& component : m_oneOffAudioComponents) {
            audioComponents.push_back(component);
        }

        updateListener(listenerPosition, Vector3D{0, 0, -1}, Vector3D{0, 1, 0});
        updateAudioComponents(timeStep, audioComponents);
        auto usefulComponents = audioComponents;
        if (audioComponents.size() > m_channels.capacity()) {
            usefulComponents = removeRedundantSources(listenerPosition, audioComponents);
        }
        for (auto& componentIn : usefulComponents) {
            assignChannel(componentIn, (int)usefulComponents.size());
        }

        auto toKeep = std::vector<std::shared_ptr<AudioComponent>>{};
        for (auto& component : m_oneOffAudioComponents) {
            if (component->m_isAssigned) {
                toKeep.push_back(component);
            }
        }
        m_oneOffAudioComponents = toKeep;
    }

    void Audio::updateAudioComponents(float timeStep, std::vector<std::shared_ptr<AudioComponent>>& components)
    {
        for (auto& audioComponent : components) {
            if (!audioComponent->m_isPlaying) {
                continue;
            }
            if (audioComponent->m_timeLeft < 0) {
                audioComponent->m_isPlaying = false;
            }
            audioComponent->m_timeLeft -= timeStep * audioComponent->m_pitch;
            if (audioComponent->m_loop) {
                while (audioComponent->m_timeLeft < 0) {
                    audioComponent->m_timeLeft += audioComponent->m_stream->getTotalTime();
                }
            }
        }
    }

    void Audio::updateListener(const Vector3D& position, const Vector3D& frontVector, const Vector3D& upVector)
    {
        OPENALCALL(alListener3f(AL_POSITION, position.x, position.y, position.z));
        ALfloat forwardAndUpVectors[] = {
                frontVector.x, frontVector.y, frontVector.z,
                upVector.x, upVector.y, upVector.z
        };
        OPENALCALL(alListenerfv(AL_ORIENTATION, forwardAndUpVectors));
    }
}