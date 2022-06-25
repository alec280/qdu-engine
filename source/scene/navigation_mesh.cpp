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

    void NavigationMesh::addConnection(int from, int to, bool bilateral)
    {
        m_cells[from].connections.push_back(to);
        if (bilateral) {
            m_cells[to].connections.push_back(from);
        }
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

    Vector2D NavigationMesh::getCellPosition(int cell)
    {
        if (cell >= m_cells.size() || cell == -1) {
            return {0, 0};
        }
        return m_cells[cell].begin;
    }

    std::vector<int> NavigationMesh::getPath(int from, int to)
    {
        return {};
    }

    std::vector<int> NavigationMesh::getPath(Vector2D& from, Vector2D& to)
    {
        return getPath(getCell(from), getCell(to));
    }
}