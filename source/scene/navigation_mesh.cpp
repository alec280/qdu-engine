#include "navigation_mesh.hpp"

namespace QDUEngine
{
    NavigationMesh::NavigationMesh(std::shared_ptr<VisualComponent> visualComponent)
        : GameObject(nullptr, visualComponent)
    {}
}