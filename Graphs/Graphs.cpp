//Works without the destructors. TODO fix it for god's sake.

#include <vector>
#include <iostream>

class Node {
private:
    int id;
    std::vector<Node*> neighbors;

public:
    static int nextID;
    Node() : id(nextID++) {
    }

    Node(int id) : id(id) {
    }

    int getID() const {
        return id;
    }

    void addNeighbor(Node* neighbor) {
        neighbors.emplace_back(neighbor);
    }

    const std::vector<Node*>& getNeighbors() const {
        return neighbors;
    }
};

int Node::nextID = 0;

class Graph {
private:
    int id;
    std::vector<Node*> nodes;

public:
    static int nextID;
    Graph() : id(nextID++) {
    }

    Graph(int id) : id(id) {
    }
    Node* addNode() {
        Node* n = new Node();
        nodes.emplace_back(n);
        return nodes.back();
    }

    Node* addNode(int nodeId) {
        Node* n = new Node(nodeId);
        nodes.emplace_back(n);
        return nodes.back();
    }

    void addEdge(Node* node1, Node* node2) {
        node1->addNeighbor(node2);
        node2->addNeighbor(node1);
    }

    const std::vector<Node*> getNodes() const {
        return nodes;
    }

    void print() const {
        for (const Node* node : nodes) {
            std::cout << "Node ID: " << node->getID() << std::endl;
            const std::vector<Node*>& neighbors = node->getNeighbors();
            std::cout << "Neighbors: ";
            for (const Node* neighbor : neighbors) {
                std::cout << neighbor->getID() << " ";
            }
            std::cout << std::endl;
        }
    }
};

int Graph::nextID = 0;

int main() {

    Graph g;
    auto node1 = g.addNode();
    auto node2 = g.addNode();
    auto node3 = g.addNode();

    g.addEdge(node1, node2);
    g.addEdge(node1, node3);
    g.addEdge(node2, node3);

    g.print();

    delete node1;
    delete node2;
    delete node3;

    std::cout << Node::nextID << std::endl;

    return 0;
}