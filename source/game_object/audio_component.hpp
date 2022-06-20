#pragma once
#include <string>
#include <vector>

namespace QDUEngine
{
    class AudioComponent {
        friend class Audio;
    public:
        explicit AudioComponent(const char* source);
        void play();
        void setSource(const char* audioPath);
    private:
        bool m_playing = false;
        std::vector<float> m_position = {0.f, 0.f, 0.f};
        bool m_to_play = false;
        std::string m_source;
    };
}