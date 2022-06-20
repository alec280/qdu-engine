#pragma once
#include <string>
#include <vector>
#include "../core/vector_3d.hpp"

namespace QDUEngine
{
    class AudioComponent {
        friend class Audio;
    public:
        AudioComponent() = default;
        explicit AudioComponent(const char* source);
        void move(const Vector2D& by);
        void move(const Vector3D& by);
        void play();
        void setAsListener(bool value);
        void setSource(const char* audioPath);
    private:
        bool m_listener = false;
        bool m_playing = false;
        Vector3D m_position = {0.f, 0.f, 0.f};
        bool m_to_play = false;
        std::string m_source;
        bool m_wait = false;
    };
}