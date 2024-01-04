#include "./scorer.hpp"
#include "drawer.hpp"
#include <SFML/Graphics/Image.hpp>

Scorer::Scorer(sf::Image target) : drawer(target) {
  this->target.loadFromImage(target);
  this->diffshader.loadFromFile("./diffshader.glsl", sf::Shader::Fragment);
  this->diffshader.setUniform("goal", this->target);
  auto targetImageSize = target.getSize() / 2u;
  this->renderer.create(targetImageSize.x, targetImageSize.y);
  this->shape.setPosition(0, 0);
  this->shape.setSize(sf::Vector2f(targetImageSize.x, targetImageSize.y));
}

float Scorer::score(ShapeSpec shape) {
  auto rendered = this->drawer.textureWithNewShape(shape);
  this->diffshader.setUniform("current", rendered);
  this->shape.setTexture(&rendered);
  this->renderer.draw(this->shape, &this->diffshader);
  this->renderer.display();

  auto texture = this->renderer.getTexture();
  auto image = texture.copyToImage();
  auto ptr = image.getPixelsPtr();
  auto size = image.getSize();
  long sum = 0;
  for (long i = 0; i < size.x * size.y * 4; i += 4) {
    auto value = ptr[i];
    sum += value;
  }
  return sum;
}
