#include "./drawer.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

Drawer::Drawer(sf::Image target) {
  this->targetTexture.loadFromImage(target);
  auto targetImageSize = target.getSize();
  this->colorShader.loadFromFile("./colorShader.glsl", sf::Shader::Fragment);
  this->colorShader.setUniform("target", this->targetTexture);
  this->renderTexture.create(targetImageSize.x, targetImageSize.y);
}

void Drawer::addNewShape(ShapeSpec newShape) {
  this->shapes.push_back(newShape);
}

void Drawer::drawShape(ShapeSpec shape) {
    this->shape.setSize(shape.size);
    this->shape.setPosition(shape.position);
    this->colorShader.setUniform("position", shape.position);
    this->colorShader.setUniform("size", sf::Vector2f(this->targetTexture.getSize()));
    this->colorShader.setUniform("opacity", shape.opacity);
    this->shape.setRotation(shape.rotation);
    this->renderTexture.draw(this->shape, &this->colorShader);
}

sf::Texture Drawer::currentTexture() {
  this->renderTexture.clear(sf::Color::Transparent);
  for (auto shape : this->shapes) {
    this->drawShape(shape);
  }
  this->renderTexture.display();
  return this->renderTexture.getTexture();
}

sf::Texture Drawer::textureWithNewShape(ShapeSpec newShape) {
  this->renderTexture.clear(sf::Color::Transparent);
  for (auto shape : this->shapes) {
    this->drawShape(shape);
  }
  this->drawShape(newShape);
  this->renderTexture.display();
  return this->renderTexture.getTexture();
}
