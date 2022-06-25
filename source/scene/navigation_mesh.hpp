#pragma once
#include "../game_object/game_object.hpp"

namespace QDUEngine
{
    class NavigationMesh : public GameObject {
        friend class Scene;
    public:
        explicit NavigationMesh(std::shared_ptr<VisualComponent> visualComponent);
        void addCell(Vector2D& begin, Vector2D& end);
        int getCell(Vector2D& pos);
    private:
        struct CellData {
            Vector2D begin;
            Vector2D end;
        };
        std::vector<CellData> m_cells{};
    };
}