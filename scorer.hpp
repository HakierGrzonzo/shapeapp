#pragma once

#include "./drawer.hpp"
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class Scorer {
  public:
    Scorer(sf::Image target);
    float score(ShapeSpec shape);
    Drawer drawer;
  private:
    sf::Shader diffshader;
    sf::RenderTexture renderer;
    sf::Texture target;
    sf::RectangleShape shape;
};
