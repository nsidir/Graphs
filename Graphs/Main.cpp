#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <algorithm>

#include "Graphs.h"
#include "Scenes.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Graph Visualization App");
    //sf::RenderWindow window(sf::VideoMode(1920, 1080), "Graph Visualization App");

    Graph g;

    SceneManager sceneManager;
    MainScene* mainScene = new MainScene(sceneManager, g);
    MenuScene menuScene(sceneManager, mainScene);

    sceneManager.setCurrentScene(menuScene);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { 
                delete mainScene;
                window.close();
            }
              

            sceneManager.handleEvents(event, window);
            sceneManager.draw(window);
        }
    }




    return 0;
}