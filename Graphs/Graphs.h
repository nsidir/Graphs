#ifndef GRAPHS_H
#define GRAPHS_H


class Node {
private:
    int id;
    sf::CircleShape circle;
    std::vector<std::shared_ptr<Node>> neighbors;

public:
    static int nextID;
    Node(sf::Vector2f position);

    const sf::CircleShape& getCircle() const;

    void setCircle(const sf::CircleShape& circle);

    int getID() const;

    void addNeighbor(const std::shared_ptr<Node>& neighbor);

    void removeNeighbor(const std::shared_ptr<Node>& neighbor);

    const std::vector<std::shared_ptr<Node>>& getNeighbors() const;

    void draw(sf::RenderWindow& window) const;
};

class Graph {
protected:
    std::vector<std::shared_ptr<Node>> nodes;
    std::shared_ptr<Node> startingNode;
    std::shared_ptr<Node> searchStartNode = nullptr;
    std::shared_ptr<Node> searchEndNode = nullptr;

    struct Edge {
        std::shared_ptr<Node> node1;
        std::shared_ptr<Node> node2;
    };

    std::vector<Edge> edges;


    enum Type { NodePl, EdgePl };


    std::vector<Type> placementHistory;  // History of node and edge placements



public:
    std::shared_ptr<Node> addNode(sf::Vector2f position);

    void pop_nodes();

    bool hasEdge(const std::shared_ptr<Node>& node1, const std::shared_ptr<Node>& node2) const;

    virtual void addEdge(const std::shared_ptr<Node>& node1, const std::shared_ptr<Node>& node2);

    void removeEdge(const std::shared_ptr<Node>& node1, const std::shared_ptr<Node>& node2);

    const std::vector<std::shared_ptr<Node>>& getNodes() const;

    void setSearchStartNode(const std::shared_ptr<Node>& node);

    void setSearchEndNode(const std::shared_ptr<Node>& node);

    const std::shared_ptr<Node>& getSearchStartNode();

    const std::shared_ptr<Node>& getSearchEndNode();

    void setStartingNode(const std::shared_ptr<Node>& node);

    bool startingNodeExists();

    void addEdgeIfValid(const std::shared_ptr<Node>& node);

    void info() const;

    virtual void draw(sf::RenderWindow& window) const;

    void clear();

    void undo();

};

class DirectedGraph : public Graph {
public:
    void addEdge(const std::shared_ptr<Node>& source, const std::shared_ptr<Node>& target);

    void draw(sf::RenderWindow& window) const override;

private:
    sf::Vector2f rotate(const sf::Vector2f& vector, float angle) const;
};


#endif