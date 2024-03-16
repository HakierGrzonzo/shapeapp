#include "./drawer.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

Drawer::Drawer(sf::Image target) {
  this->targetTexture.loadFromImage(target);
  auto targetImageSize = target.getSize();
  this->colorShader.loadFromFile("./colorShader.glsl", sf::Shader::Fragment);
  this->colorShader.setUniform("target", this->targetTexture);
  this->renderTexture.create(targetImageSize.x, targetImageSize.y);
  this->cachedTexture.create(targetImageSize.x, targetImageSize.y);
  this->background.setPosition(0, 0);
  this->background.setSize(sf::Vector2f(targetImageSize));
  this->background.setTexture(&this->cachedTexture);
}

void Drawer::addNewShape(ShapeSpec newShape) {
  this->shapes.push_back(newShape);
  this->dirty = true;
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
void Drawer::drawCache() {
    this->renderTexture.clear(sf::Color::Transparent);
    if (this->dirty) {
      for (auto shape : this->shapes) {
        this->drawShape(shape);
      }
    } else {
      this->renderTexture.draw(this->background);
    }
    if (this->dirty) {
      this->renderTexture.display();
      auto newCache = this->renderTexture.getTexture();
      this->cachedTexture.update(newCache);
      this->background.setTexture(&this->cachedTexture);
      this->dirty = false;
    }
}

sf::Texture Drawer::currentTexture() {
  this->drawCache();
  this->renderTexture.display();
  return this->renderTexture.getTexture();
}

sf::Texture Drawer::textureWithNewShape(ShapeSpec newShape) {
  this->drawCache();
  this->drawShape(newShape);
  this->renderTexture.display();
  return this->renderTexture.getTexture();
}

unsigned int Drawer::size() {
  return this->shapes.size();
}
