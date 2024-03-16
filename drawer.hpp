#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <vector>

struct ShapeSpec {
  sf::Vector2f position;
  sf::Vector2f size;
  float rotation;
  float opacity;
  long score = 0;
};

class Drawer {
  public:
    Drawer(sf::Image target);

    sf::Texture textureWithNewShape(ShapeSpec newShape);
    void addNewShape(ShapeSpec newShape);
    sf::Texture currentTexture();
    unsigned int size();
  private:
    bool dirty = true;
    std::vector<ShapeSpec> shapes;
    sf::RenderTexture renderTexture;
    sf::Texture targetTexture;
    sf::Texture cachedTexture;
    sf::RectangleShape shape;
    sf::RectangleShape background;
    sf::Shader colorShader;
    void drawShape(ShapeSpec shape);
    void drawCache();
};
