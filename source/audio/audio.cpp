#include "audio.hpp"

namespace QDUEngine
{
    void Audio::end() noexcept
    {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(m_audioContext);
        alcCloseDevice(m_audioDevice);
    }

    AudioSource Audio::getNextFreeSource()
    {
        if (m_firstFreeChannelIdx == m_channels.capacity()) {
            std::cout << "[Engine] Not enough channels available." << std::endl;
        }
        auto& entry = m_channels[m_firstFreeChannelIdx];
        int channelIdx = m_firstFreeChannelIdx;
        m_firstFreeChannelIdx = entry.m_nextFreeIdx;
        return {entry.m_sourceID, channelIdx};
    }

    bool Audio::load_wav_file(const char* filename, ALuint bufferId) const
    {
        struct WavData {
            unsigned int channels = 0;
            unsigned int sampleRate = 0;
            drwav_uint64 totalPCMFrameCount = 0;
            std::vector<uint16_t> pcmData;
            drwav_uint64 GetTotalSamples() const { return totalPCMFrameCount * channels; }
        };

        WavData audioData;
        drwav_int16* sampleData = drwav_open_file_and_read_pcm_frames_s16(
                filename,
                &audioData.channels,
                &audioData.sampleRate,
                &audioData.totalPCMFrameCount,
                nullptr);

        if (!sampleData)
        {
            std::cerr << "[Engine] Failed to load file: " << filename << std::endl;
            drwav_free(sampleData, nullptr);
            return false;
        }
        else if (audioData.GetTotalSamples() > drwav_uint64(std::numeric_limits<size_t>::max()))
        {
            std::cerr << "[Engine] File " << filename << " is to big to be loaded." << std::endl;
            drwav_free(sampleData, nullptr);
            return false;
        }

        audioData.pcmData.resize(size_t(audioData.GetTotalSamples()));
        std::memcpy(audioData.pcmData.data(), sampleData, audioData.pcmData.size() * 2);
        drwav_free(sampleData, nullptr);

        assert(bufferId != 0);

        alBufferData(
                bufferId,
                audioData.channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
                audioData.pcmData.data(),
                (int)audioData.pcmData.size() * 2,
                (int)audioData.sampleRate);

        return true;
    }

    void Audio::play2D(const char* file) {
        ALuint source;
        OPENALCALL(alGenSources((ALuint)1, &source));
        OPENALCALL(alSourcef(source, AL_PITCH, 1));
        OPENALCALL(alSourcef(source, AL_GAIN, 1));
        OPENALCALL(alSource3f(source, AL_POSITION, 0, 0, 0));
        OPENALCALL(alSource3f(source, AL_VELOCITY, 0, 0, 0));
        OPENALCALL(alSourcei(source, AL_LOOPING, AL_FALSE));

        ALuint buffer;
        OPENALCALL(alGenBuffers((ALuint)1, &buffer));

        if (!load_wav_file(file, buffer)) {
            std::cout << "[Engine] WAV could not be loaded." << std::endl;
            return;
        }

        std::cout << "[Engine] WAV file loaded correctly." << std::endl;

        /* Binding the buffer with the data to source */
        OPENALCALL(alSourcei(source, AL_BUFFER, buffer));

        auto t0 = std::chrono::steady_clock::now();

        /* Playing the source */
        OPENALCALL(alSourcePlay(source));

        /* Wait while playing the song */
        ALint source_state;
        OPENALCALL(alGetSourcei(source, AL_SOURCE_STATE, &source_state));
        while (source_state == AL_PLAYING) {
            OPENALCALL(alGetSourcei(source, AL_SOURCE_STATE, &source_state));
        }

        auto dt = std::chrono::steady_clock::now() - t0;

        // cleanup context
        OPENALCALL(alDeleteSources(1, &source));
        OPENALCALL(alDeleteBuffers(1, &buffer));

        const auto duration = std::chrono::duration_cast<std::chrono::seconds>(dt).count();

        std::cout << "[Engine] The wav file lasted " << duration << " seconds." << std::endl;
    }

    void Audio::start()
    {
        const int channelAmount = 32;
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

    void Audio::update(Scene* scene)
    {
        auto listenerPosition = Vector3D{};
        auto componentsToPlay = std::vector<std::shared_ptr<AudioComponent>>{};
        for (auto& object : scene->getObjects()) {
            auto component = object->getAudioComponent();
            if (component == nullptr) {
                continue;
            }
            if (component->m_listener) {
                listenerPosition = component->m_position;
            }
            if (component->m_to_play) {
                componentsToPlay.push_back(component);
                component->m_to_play = false;
                component->m_playing = true;
                play2D(component->m_source.c_str());
                component->m_playing = false;
            }
        }
        OPENALCALL(alListenerf(AL_GAIN, m_masterVolume));
        updateListener(listenerPosition, Vector3D{}, Vector3D{});
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