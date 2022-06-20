#include "audio.hpp"

#define OPENALCALL(function)\
	function;\
	{\
		ALenum error = alGetError(); \
		if (error != AL_NO_ERROR) {  \
            std::cout << "OpenAL Error" << std::endl; \
        }}

bool load_wav_file(const char* audiofile, ALuint bufferId)
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
            audiofile,
            &audioData.channels,
            &audioData.sampleRate,
            &audioData.totalPCMFrameCount,
            nullptr);

    if (!sampleData)
    {
        std::cerr << "Audio Clip Error: Failed to load file " << audiofile << std::endl;
        drwav_free(sampleData, nullptr);
        return false;
    }
    else if (audioData.GetTotalSamples() > drwav_uint64(std::numeric_limits<size_t>::max()))
    {
        std::cerr << "Audio Clip Error: File " << audiofile << " is to big to be loaded." << std::endl;
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

namespace QDUEngine
{
    void Audio::end() noexcept
    {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(m_audioContext);
        alcCloseDevice(m_audioDevice);
    }

    void Audio::play2D(const char* file) {
        /* Setting up a source */
        ALuint source;
        OPENALCALL(alGenSources((ALuint)1, &source));
        OPENALCALL(alSourcef(source, AL_PITCH, 1));
        OPENALCALL(alSourcef(source, AL_GAIN, 1));
        OPENALCALL(alSource3f(source, AL_POSITION, 0, 0, 0));
        OPENALCALL(alSource3f(source, AL_VELOCITY, 0, 0, 0));
        OPENALCALL(alSourcei(source, AL_LOOPING, AL_FALSE));

        /* Generating a buffer */
        ALuint buffer;
        OPENALCALL(alGenBuffers((ALuint)1, &buffer));

        bool success = load_wav_file(file, buffer);
        if (!success) {
            std::cout << "wav file error" << std::endl;
            return;
        }

        std::cout << "wav file loaded correctly" << std::endl;

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

        std::cout << "The wav file lasted " << duration << " seconds." << std::endl;
    }

    void Audio::start()
    {
        const int channels = 32;
        m_audioDevice = alcOpenDevice(nullptr);

        if (!m_audioDevice) {
            std::cout << "Audio Error: Failed to open audio device." << std::endl;
            return;
        }
        m_audioContext = alcCreateContext(m_audioDevice, nullptr);
        if (!alcMakeContextCurrent(m_audioContext)) {
            std::cout << "Audio Error: Failed to make audio context current." << std::endl;
            return;
        }

        OPENALCALL(alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED));
        m_masterVolume = 1.0f;
        OPENALCALL(alListenerf(AL_GAIN, m_masterVolume));

        m_channels = channels;
        m_openALSources.reserve(channels);
        for (unsigned int i = 0; i < channels; i++) {
            ALuint source = 0;
            OPENALCALL(alGenSources(1, &source));
            m_openALSources.emplace_back(source, i + 1);
        }
        m_firstFreeOpenALSourceIndex = 0;
    }

    void Audio::update(Scene* scene)
    {
        for (auto& object : scene->getObjects()) {
            auto component = object->getAudioComponent();
            if (component == nullptr) {
                continue;
            }
            if (component->m_to_play) {
                component->m_to_play = false;
                component->m_playing = true;
                play2D(component->m_source.c_str());
                component->m_playing = false;
            }
        }
    }
}