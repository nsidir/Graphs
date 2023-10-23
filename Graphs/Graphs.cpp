#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <functional>
#include <limits>

#include "Graphs.h"
#include "Font.h"
#include <queue>

    Node::Node(sf::Vector2f position) : id(nextID++) {
        circle.setRadius(20.f);
        circle.setFillColor(sf::Color::White);
        circle.setOutlineThickness(2.f);
        circle.setOutlineColor(sf::Color::Black);
        circle.setPosition(position);
    }

    const sf::CircleShape& Node::getCircle() const {
        return circle;
    }

    void Node::setCircle(const sf::CircleShape& circle) {
        this->circle = circle;
    }

    int Node::getID() const {
        return id;
    }

    void Node::addNeighbor(const std::shared_ptr<Node>& neighbor) {
        if (std::find(neighbors.begin(), neighbors.end(), neighbor) == neighbors.end()) {
            neighbors.emplace_back(neighbor);
        }
    }

    void Node::removeNeighbor(const std::shared_ptr<Node>& neighbor) {
        auto it = std::find(neighbors.begin(), neighbors.end(), neighbor);
        if (it != neighbors.end()) {
            neighbors.erase(it);
        }
    }

    const std::vector<std::shared_ptr<Node>>& Node::getNeighbors() const {
        return neighbors;
    }

    void Node::draw(sf::RenderWindow& window) const {
        window.draw(circle);

        sf::Font font;
        
        try {
            font.loadFromFile(FONT);
        }
        catch (const std::exception& e) {
            std::cout << "ERROR: " << e.what() << std::endl;
        }

        sf::Text idText;
        idText.setFont(font);
        idText.setCharacterSize(20);
        idText.setString(std::to_string(getID()));
        idText.setFillColor(sf::Color::Black);

        sf::FloatRect textBounds = idText.getLocalBounds();
        idText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        idText.setPosition(circle.getPosition() + sf::Vector2f(circle.getRadius(), circle.getRadius()));

        window.draw(idText);
    }

int Node::nextID = 0;



std::shared_ptr<Node> Graph::addNode(sf::Vector2f position) {
    auto newNode = std::make_shared<Node>(position);
    nodes.push_back(newNode);
    placementHistory.push_back(NodePl);
    return newNode;
}

void Graph::pop_nodes() {
    nodes.pop_back();
}

bool Graph::hasEdge(const std::shared_ptr<Node>& node1, const std::shared_ptr<Node>& node2) const {
    auto it = std::find_if(edges.begin(), edges.end(), [&](const Edge& edge) {
        return (edge.node1 == node1 && edge.node2 == node2) || (edge.node1 == node2 && edge.node2 == node1);
        });

    return it != edges.end();
}

void Graph::addEdge(const std::shared_ptr<Node>& node1, const std::shared_ptr<Node>& node2) {
    if (node1 != node2 && !hasEdge(node1, node2)) {
        edges.push_back({ node1, node2 });
        node1->addNeighbor(node2);
        node2->addNeighbor(node1);
        placementHistory.push_back(EdgePl);
    }
}

void Graph::removeEdge(const std::shared_ptr<Node>& node1, const std::shared_ptr<Node>& node2) {
    node1->removeNeighbor(node2);
    node2->removeNeighbor(node1);

    auto it = std::find_if(edges.begin(), edges.end(), [&](const Edge& edge) {
        return (edge.node1 == node1 && edge.node2 == node2) || (edge.node1 == node2 && edge.node2 == node1);
        });

    if (it != edges.end()) {
        edges.erase(it);
    }
}

const std::vector<std::shared_ptr<Node>>& Graph::getNodes() const {
    return nodes;
}

void Graph::setSearchStartNode(const std::shared_ptr<Node>& node) {
    searchStartNode = node;
}

void Graph::setSearchEndNode(const std::shared_ptr<Node>& node) {
    searchEndNode = node;
}

const std::shared_ptr<Node>& Graph::getSearchStartNode() {
    return searchStartNode;
}

const std::shared_ptr<Node>& Graph::getSearchEndNode() {
    return searchEndNode;
}

void Graph::setStartingNode(const std::shared_ptr<Node>& node) {
    startingNode = node;
}

bool Graph::startingNodeExists() {
    if (startingNode.get() != nullptr) {
        return true;
    }
    return false;
}

void Graph::addEdgeIfValid(const std::shared_ptr<Node>& node) {
    if (startingNode && startingNode != node) {
        addEdge(startingNode, node);
    }
    startingNode.reset();
}

int Graph::shortestPath(const Graph* graph, std::shared_ptr<Node> start, std::shared_ptr<Node> end) const {
    constexpr int INF = std::numeric_limits<int>::max();
    int numNodes = graph->getNodes().size();

    std::vector<int> memo(numNodes, INF);

    std::queue<std::shared_ptr<Node>> bfsQueue;
    bfsQueue.push(start);
    memo[start->getID()] = 0;

    while (!bfsQueue.empty()) {
        auto currentNode = bfsQueue.front();
        bfsQueue.pop();

        const auto& neighbors = currentNode->getNeighbors();
        for (const auto& neighbor : neighbors) {
            if (memo[neighbor->getID()] == INF) {
                memo[neighbor->getID()] = memo[currentNode->getID()] + 1;
                bfsQueue.push(neighbor);

                if (neighbor == end) {
                    return memo[end->getID()];
                }
            }
        }
    }

    return 0;
}




void Graph::info() const {
    for (const auto& node : nodes) {
        std::cout << "Node ID: " << node->getID() << std::endl;
        const auto& neighbors = node->getNeighbors();
        std::cout << "Neighbors: ";
        if (neighbors.empty()) {
            std::cout << "Empty " << std::endl;
        }
        else {
            for (const auto& neighbor : neighbors) {
                std::cout << neighbor->getID() << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "-------------------" << std::endl;
    }
    if (searchStartNode && searchEndNode) {
        std::cout << "Shortest Path length is: " << shortestPath(this, searchStartNode, searchEndNode) << std::endl;
    }

    std::cout << "-------------------" << std::endl;
}

void Graph::draw(sf::RenderWindow& window) const {
    for (const auto& node : nodes) {
        node->draw(window);
        const auto& neighbors = node->getNeighbors();
        for (const auto& neighbor : neighbors) {
            sf::Vertex line[] = {
                sf::Vertex(node->getCircle().getPosition() + sf::Vector2f(node->getCircle().getRadius(), node->getCircle().getRadius())),
                sf::Vertex(neighbor->getCircle().getPosition() + sf::Vector2f(neighbor->getCircle().getRadius(), neighbor->getCircle().getRadius()))
            };
            window.draw(line, 2, sf::Lines);
        }
    }
}

void Graph::clear() {
    nodes.clear();
    startingNode.reset();
    Node::nextID = 0;
    std::system("cls");
}

void Graph::undo() {
    if (!placementHistory.empty()) {
        if (placementHistory.back() == NodePl) {
            if (nodes.size() >= 1) {
                auto lastNode = nodes.back();
                for (const auto& neighbor : lastNode->getNeighbors()) {
                    neighbor->removeNeighbor(lastNode);
                }
                nodes.pop_back();
                Node::nextID--;
            }
        }
        else if (placementHistory.back() == EdgePl) {
            if (!nodes.empty()) {
                if (!edges.empty()) {
                    const Edge& lastEdge = edges.back();
                    edges.pop_back();
                    removeEdge(lastEdge.node1, lastEdge.node2);
                }
            }
        }

        placementHistory.pop_back();
    }
}


void DirectedGraph::addEdge(const std::shared_ptr<Node>& source, const std::shared_ptr<Node>& target) {
    source->addNeighbor(target);
}

void DirectedGraph::draw(sf::RenderWindow& window) const {
    for (const auto& node : nodes) {
        node->draw(window);
        const auto& neighbors = node->getNeighbors();
        for (const auto& neighbor : neighbors) {
            sf::Vector2f startPos = node->getCircle().getPosition() + sf::Vector2f(node->getCircle().getRadius(), node->getCircle().getRadius());
            sf::Vector2f endPos = neighbor->getCircle().getPosition() + sf::Vector2f(neighbor->getCircle().getRadius(), neighbor->getCircle().getRadius());

            sf::Vector2f direction = endPos - startPos;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            sf::Vector2f unitDirection = direction / length;

            const float nodeRadius = neighbor->getCircle().getRadius();
            sf::Vector2f arrowOffset = unitDirection / 4.0f * nodeRadius;

            const float arrowSize = 16.0f;
            sf::Vertex line[] = {
                sf::Vertex(startPos),
                sf::Vertex(endPos - arrowOffset)
            };
            window.draw(line, 2, sf::Lines);

            sf::Vertex arrowHead[] = {
                sf::Vector2f(endPos - arrowOffset - unitDirection * arrowSize),
                sf::Vector2f(endPos - arrowOffset - unitDirection * arrowSize - rotate(unitDirection, 45.0f) * arrowSize),
                sf::Vector2f(endPos - arrowOffset - unitDirection * arrowSize - rotate(unitDirection, -45.0f) * arrowSize)
            };
            window.draw(arrowHead, 3, sf::Triangles);
        }
    }
}

sf::Vector2f DirectedGraph::rotate(const sf::Vector2f& vector, float angle) const {
    float radAngle = angle * 3.14159265359f / 180.0f;
    float cosine = std::cos(radAngle);
    float sine = std::sin(radAngle);
    return sf::Vector2f(vector.x * cosine - vector.y * sine, vector.x * sine + vector.y * cosine);
}


