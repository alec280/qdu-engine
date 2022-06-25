#pragma once
#include "../game_object/game_object.hpp"

namespace QDUEngine
{
    class NavigationMesh : public GameObject {
        friend class Scene;
    public:
        explicit NavigationMesh(std::shared_ptr<VisualComponent> visualComponent);
        int getCell(Vector2D& pos);
    private:
        std::vector<int> m_cell{};
    };
}