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
        return 0;
    }
}