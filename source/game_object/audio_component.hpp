#pragma once
#include <memory>
#include <nlohmann/json.hpp>
#include "../core/vector_3d.hpp"
#include "../audio/audio_source.hpp"
#include "../audio/audio_stream.hpp"

namespace QDUEngine
{
    class AudioComponent {
        friend class Audio;
    public:
        AudioComponent() = default;
        explicit AudioComponent(const char* filename);
        Vector3D getPosition();
        std::shared_ptr<AudioStream> getStream();
        void clear();
        nlohmann::json getData();
        bool hasSource();
        void move(const Vector2D& by);
        void move(const Vector3D& by);
        void play();
        void setAs3D(bool value);
        void setAsListener(bool value);
        void setAsLooping(bool value);
        void setAutoPlay(bool value);
        void setSource(const char* filename);
    private:
        AudioSource m_audioSource{};
        bool m_autoPlay = false;
        bool m_is3D = false;
        bool m_isAssigned = false;
        bool m_isListener = false;
        bool m_isPlaying = false;
        bool m_loop = false;
        float m_pitch = 1.f;
        Vector3D m_position = Vector3(0, 0, 0);
        float m_radius = 2.f;
        std::shared_ptr<AudioStream> m_stream = nullptr;
        float m_timeLeft = 0.f;
        float m_volume = 1.f;
    };
}