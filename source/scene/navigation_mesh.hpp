#pragma once
#include "../game_object/game_object.hpp"

namespace QDUEngine
{
    class NavigationMesh : public GameObject {
        friend class Scene;
    public:
        explicit NavigationMesh(std::shared_ptr<VisualComponent> visualComponent);
        void addCell(Vector2D& begin, Vector2D& end);
        void addConnection(int from, int to, bool bilateral);
        int getCell(Vector2D& pos);
        Vector2D getCellPosition(int cell);
        std::vector<int> getPath(int from, int to);
        std::vector<int> getPath(Vector2D& from, Vector2D& to);
    private:
        struct CellData {
            Vector2D begin;
            Vector2D end;
            std::vector<int> connections{};
        };
        std::vector<CellData> m_cells{};
    public:
        std::vector<CellData> getCells();
    };
}