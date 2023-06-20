#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "Graphs.h"
#include "Scenes.h"


void SceneManager::setCurrentScene(Scene& scene) {
    currentScene = &scene;
}

void SceneManager::handleEvents(sf::Event event, sf::RenderWindow& window) {
    currentScene->handleEvents(event, window);
    
}

void SceneManager::draw(sf::RenderWindow& window) {
    currentScene->draw(window);
}


MainScene::MainScene(SceneManager& sceneManager, Graph& g) {
    setSceneManager(sceneManager);
    setGraph(g);
}

void MainScene::setGraph(Graph& g) {
    this->g = g;
}

void MainScene::setSceneManager(SceneManager& sceneManager) {
    this->sceneManager = sceneManager;
}

void MainScene::handleEvents(sf::Event event, sf::RenderWindow& window) {
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::C) {
                    g.clear();
                }
                else if (event.key.code == sf::Keyboard::I) {
                    g.info();
                }
                else if (event.key.code == sf::Keyboard::Z && event.key.control) {
                    g.undo();
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

        this->draw(window);
    }
}

void MainScene::draw(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);

    g.draw(window);

    window.display();
}

MenuScene::MenuScene(SceneManager& sceneManager, MainScene* mainScene) {
    setSceneManager(sceneManager);
    try {
        font.loadFromFile("Fonts/BebasNeue-Regular.ttf");
    }
    catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << std::endl;
    }
    titleText.setFont(font);
    titleText.setString("Graphs");
    titleText.setCharacterSize(40);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(300, 80);

    button.setPosition(300, 250);
    button.setSize(sf::Vector2f(200, 50));
    button.setFillColor(sf::Color::Green);

    buttonText.setString("Click me");
    buttonText.setFont(font); 
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setPosition(button.getPosition() + sf::Vector2f(10, 10)); 

    this->mainScene = mainScene;
}


void MenuScene::setSceneManager(SceneManager& sceneManager) {
    this->sceneManager = &sceneManager;
}

void MenuScene::handleEvents(sf::Event event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        if (button.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
            sceneManager->setCurrentScene(*mainScene);  // Transition to the main scene
        }
    }
}

void MenuScene::draw(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);

    window.draw(titleText);
    window.draw(button);
    window.draw(buttonText);

    window.display();
}
