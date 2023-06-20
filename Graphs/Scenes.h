#ifndef SCENES_H
#define SCENES_H

#include <SFML/Graphics.hpp>

class Scene {
public:
    virtual void handleEvents(sf::Event event, sf::RenderWindow& window) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};

class MainScene; // Forward declaration

class MenuScene; // Forward declaration

class SceneManager {
private:
    Scene* currentScene;

public:
    void setCurrentScene(Scene& scene);

    void handleEvents(sf::Event event, sf::RenderWindow& window);

    void draw(sf::RenderWindow& window);
};

class MainScene : public Scene {
public:
    MainScene(SceneManager& sceneManager, Graph& g);

    void setGraph(Graph& g);

    void setSceneManager(SceneManager& sceneManager);

    void handleEvents(sf::Event event, sf::RenderWindow& window) override;

    void draw(sf::RenderWindow& window) override;

private:
    Graph g;
    SceneManager sceneManager;
};

class MenuScene : public Scene {
public:
    MenuScene(SceneManager& sceneManager, MainScene* mainScene);

    void setSceneManager(SceneManager& sceneManager);

    void handleEvents(sf::Event event, sf::RenderWindow& window) override;

    void draw(sf::RenderWindow& window) override;

private:
    SceneManager* sceneManager;
    MainScene* mainScene;
    sf::Text titleText;
    sf::Font font;
    sf::RectangleShape button;
    sf::Text buttonText;
};


#endif