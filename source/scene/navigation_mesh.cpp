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
        auto fromCell = m_cells[from].connections;
        auto toCell = m_cells[to].connections;
        if (!std::count(fromCell.begin(), fromCell.end(), to)) {
            m_cells[from].connections.push_back(to);
        }
        if (bilateral && !std::count(toCell.begin(), toCell.end(), from)) {
            m_cells[to].connections.push_back(from);
        }
    }

    std::vector<int> NavigationMesh::auxGetPath(std::map<int, int>& cameFrom, int current)
    {
        auto result = std::vector<int>{current};
        int realCurrent = current;
        while (cameFrom.contains(realCurrent)) {
            realCurrent = cameFrom[realCurrent];
            result.insert(result.begin(), realCurrent);
        }
        return result;
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

    std::vector<NavigationMesh::CellData> NavigationMesh::getCells()
    {
        return m_cells;
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
        auto openList = std::vector<int>{from};
        auto cameFrom = std::map<int, int>{};
        auto scores = std::map<int, float>{{from ,0}};
        while(!openList.empty()) {
            int minCell = -1;
            float minValue = std::numeric_limits<float>::infinity();
            for (auto cell : openList) {
                if (!scores.contains(cell)) {
                    continue;
                }
                if (scores[cell] < minValue) {
                    minValue = scores[cell];
                    minCell = cell;
                }
            }
            if (minCell == to) {
                return auxGetPath(cameFrom, minCell);
            }
            openList.erase(std::remove(openList.begin(), openList.end(), minCell), openList.end());
            for (auto connection : m_cells[minCell].connections) {
                float score = std::numeric_limits<float>::infinity();
                float scoreConnection = std::numeric_limits<float>::infinity();
                if (scores.contains(minCell)) {
                    score = scores[minCell] + 1;
                }
                if (scores.contains(connection)) {
                    scoreConnection = scores[connection];
                }
                if (score < scoreConnection) {
                    cameFrom[connection] = minCell;
                    scores[connection] = score;
                    if (std::find(openList.begin(), openList.end(), connection) == openList.end()) {
                        openList.push_back(connection);
                    }
                }
            }
        }
        return {};
    }

    std::vector<int> NavigationMesh::getPath(Vector2D& from, Vector2D& to)
    {
        return getPath(getCell(from), getCell(to));
    }
}