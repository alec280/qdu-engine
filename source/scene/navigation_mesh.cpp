#include "navigation_mesh.hpp"

namespace QDUEngine
{
    NavigationMesh::NavigationMesh(std::shared_ptr<VisualComponent> visualComponent)
        : GameObject(nullptr, visualComponent)
    {}

    void NavigationMesh::addCell(Vector2D& begin, Vector2D& end)
    {
        m_cells.push_back(CellData{begin, end});
    }

    int NavigationMesh::getCell(Vector2D& pos)
    {
        int i = 0;
        for (auto& cell : m_cells) {
            if (pos.x >= cell.begin.x && pos.x < cell.end.x && pos.y >= cell.begin.y && pos.y < cell.end.y) {
                return i;
            }
            i++;
        }
        return -1;
    }
}