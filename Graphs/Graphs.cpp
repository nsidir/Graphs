#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>

class Node {
private:
    int id;
    sf::CircleShape circle;
    std::vector<std::shared_ptr<Node>> neighbors;

public:
    static int nextID;
    Node(sf::Vector2f position) : id(nextID++) {
        circle.setRadius(20.f);
        circle.setFillColor(sf::Color::White);
        circle.setOutlineThickness(2.f);
        circle.setOutlineColor(sf::Color::Black);
        circle.setPosition(position);
    }

    const sf::CircleShape& getCircle() const {
        return circle;
    }

    void setCircle(const sf::CircleShape& circle) {
        this->circle = circle;
    }

    int getID() const {
        return id;
    }

    void addNeighbor(const std::shared_ptr<Node>& neighbor) {
        neighbors.emplace_back(neighbor);
    }

    const std::vector<std::shared_ptr<Node>>& getNeighbors() const {
        return neighbors;
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(circle);
    }
};

int Node::nextID = 0;

class Graph {
protected:
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<sf::Vector2f> nodes_coordinates;
    std::shared_ptr<Node> startingNode;

public:
    std::shared_ptr<Node> addNode(sf::Vector2f position) {
        auto newNode = std::make_shared<Node>(position);
        nodes.push_back(newNode);
        nodes_coordinates.push_back(position);
        return newNode;
    }

    virtual void addEdge(const std::shared_ptr<Node>& node1, const std::shared_ptr<Node>& node2) {
        node1->addNeighbor(node2);
        node2->addNeighbor(node1);
    }

    const std::vector<std::shared_ptr<Node>>& getNodes() const {
        return nodes;
    }

    const std::vector<sf::Vector2f>& getNodesCoordinates() const {
        return nodes_coordinates;
    }

    void setStartingNode(const std::shared_ptr<Node>& node) {
        startingNode = node;
    }

    void addEdgeIfValid(const std::shared_ptr<Node>& node) {
        if (startingNode && startingNode != node) {
            addEdge(startingNode, node);
        }
        startingNode.reset();
    }

    void info() const {
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
        std::cout << "-------------------" << std::endl;
    }

    void draw(sf::RenderWindow& window) const {
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

    void clear() {
        nodes.clear();
        nodes_coordinates.clear();
        startingNode.reset();
        Node::nextID = 0;

    }
};

class DirectedGraph : public Graph {
public:
    void addEdge(const std::shared_ptr<Node>& source, const std::shared_ptr<Node>& target) {
        source->addNeighbor(target);
    }

    void draw(sf::RenderWindow& window) const {
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

private:
    sf::Vector2f rotate(const sf::Vector2f& vector, float angle) const {
        float radAngle = angle * 3.14159265359f / 180.0f;
        float cosine = std::cos(radAngle);
        float sine = std::sin(radAngle);
        return sf::Vector2f(vector.x * cosine - vector.y * sine, vector.x * sine + vector.y * cosine);
    }
};



int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Graph Visualization");

    DirectedGraph g;
    //auto node1 = g.addNode(sf::Vector2f(100.f, 100.f));
    //auto node2 = g.addNode(sf::Vector2f(300.f, 200.f));
    //auto node3 = g.addNode(sf::Vector2f(500.f, 100.f));

    //g.addEdge(node1, node2);
    //g.addEdge(node1, node3);
    //g.addEdge(node2, node3);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::C) {
                    g.clear();
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Right) {
                    sf::Vector2f mousePosition = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                    g.addNode(mousePosition);
                }
            }
            
        }

        sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

        const std::vector<std::shared_ptr<Node>> nodes = g.getNodes();

        for (auto& node : nodes) {
            sf::CircleShape circle = node->getCircle();
            sf::Vector2f position = circle.getPosition();
            float radius = circle.getRadius();
            float distance = std::sqrt(std::pow(mousePosition.x - position.x - radius, 2) + std::pow(mousePosition.y - position.y - radius, 2));
            if (distance <= radius) {
                circle.setFillColor(sf::Color::Red);
                node->setCircle(circle);
            }
            else {
                circle.setFillColor(sf::Color::White);
                node->setCircle(circle);
            }
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Middle) {
                g.info();
            }
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePosition = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);

                for (auto& node : nodes) {
                    sf::CircleShape circle = node->getCircle();
                    sf::Vector2f position = circle.getPosition();
                    float radius = circle.getRadius();
                    float distance = std::sqrt(std::pow(mousePosition.x - position.x - radius, 2) + std::pow(mousePosition.y - position.y - radius, 2));
                    if (distance <= radius) {
                        circle.setFillColor(sf::Color::Red);
                        node->setCircle(circle);

                        g.setStartingNode(node); // Set the starting node for the edge
                    }
                    else {
                        circle.setFillColor(sf::Color::White);
                        node->setCircle(circle);
                    }
                }
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePosition = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);

                for (auto& node : nodes) {
                    sf::CircleShape circle = node->getCircle();
                    sf::Vector2f position = circle.getPosition();
                    float radius = circle.getRadius();
                    float distance = std::sqrt(std::pow(mousePosition.x - position.x - radius, 2) + std::pow(mousePosition.y - position.y - radius, 2));
                    if (distance <= radius) {
                        circle.setFillColor(sf::Color::Red);
                        node->setCircle(circle);

                        g.addEdgeIfValid(node); // Add the edge if a valid node is under the mouse position
                    }
                    else {
                        circle.setFillColor(sf::Color::White);
                        node->setCircle(circle);
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

        g.draw(window);

        window.display();
    }

    return 0;
}
