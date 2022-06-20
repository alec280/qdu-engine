#pragma once
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
        AudioStream getStream();
        bool hasSource();
        void move(const Vector2D& by);
        void move(const Vector3D& by);
        void play();
        void setAsListener(bool value);
        void setSource(const char* filename);
    private:
        AudioSource m_audioSource{};
        AudioStream m_stream{};
        bool m_isAssigned = false;
        bool m_is3D = false;
        bool m_listener = false;
        bool m_loop = false;
        float m_pitch = 1.f;
        bool m_playing = false;
        Vector3D m_position = Vector3(0, 0, 0);
        float m_radius = 0.f;
        float m_timeLeft = 0.f;
        float m_volume = 1.f;
    };
}