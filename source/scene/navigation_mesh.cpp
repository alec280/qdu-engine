#include "navigation_mesh.hpp"

namespace QDUEngine
{
    NavigationMesh::NavigationMesh(std::shared_ptr<VisualComponent> visualComponent)
        : GameObject(nullptr, visualComponent)
    {}

    int NavigationMesh::getCell(Vector2D& pos)
    {
        return 0;
    }
}