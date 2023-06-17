#include <vector>
#include <iostream>
#include <memory>

class Node {
private:
    int id;
    std::vector<std::shared_ptr<Node>> neighbors;

public:
    static int nextID;
    Node() : id(nextID++) {
    }

    Node(int id) : id(id) {
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
};

int Node::nextID = 0;

class Graph {
private:
    int id;
    std::vector<std::shared_ptr<Node>> nodes;

public:
    static int nextID;
    Graph() : id(nextID++) {
    }

    Graph(int id) : id(id) {
    }
    std::shared_ptr<Node> addNode() {
        auto newNode = std::make_shared<Node>();
        nodes.push_back(newNode);
        return newNode;
    }

    std::shared_ptr<Node> addNode(int nodeId) {
        auto newNode = std::make_shared<Node>(nodeId);
        nodes.push_back(newNode);
        return newNode;
    }

    void addEdge(const std::shared_ptr<Node>& node1, const std::shared_ptr<Node>& node2) {
        node1->addNeighbor(node2);
        node2->addNeighbor(node1);
    }

    const std::vector<std::shared_ptr<Node>>& getNodes() const {
        return nodes;
    }

    void print() const {
        for (const auto& node : nodes) {
            std::cout << "Node ID: " << node->getID() << std::endl;
            const auto& neighbors = node->getNeighbors();
            std::cout << "Neighbors: ";
            if (neighbors.empty()) {
                std::cout << "Empty";
            }
            else {
                for (const auto& neighbor : neighbors) {
                    std::cout << neighbor->getID() << " ";
                }
                std::cout << std::endl;
            }
        }
    }

};

class DirectedGraph : public Graph {
public:
    void addEdge(std::shared_ptr<Node>& source, std::shared_ptr<Node>& target) {
        source->addNeighbor(target);
    }
};

int Graph::nextID = 0;

int main() {

    DirectedGraph g;
    auto node1 = g.addNode();
    auto node2 = g.addNode();
    auto node3 = g.addNode();

    g.addEdge(node1, node2);
    g.addEdge(node1, node3);
    g.addEdge(node2, node3);

    g.print();

    std::cout << std::endl;
    std::cout << Node::nextID << std::endl;

    return 0;
}