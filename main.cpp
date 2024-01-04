#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <vector>

const int size = 1024;
const int popSize = 512;

sf::Shader shader;

sf::Image diffTextures(sf::Texture& a, sf::Texture& goal, sf::RenderTexture* target) {
  sf::RectangleShape shape;
  shape.setPosition(0, 0);
  shape.setSize(sf::Vector2f(size, size));
  shader.setUniform("a", a);
  shader.setUniform("b", goal);
  shape.setTexture(&a);
  target->draw(shape, &shader);
  target->display();

  auto texture = target->getTexture();
  auto image = texture.copyToImage();
  return image;
}

long scoreImage(sf::Image img) {
  auto ptr = img.getPixelsPtr();
  auto size = img.getSize();
  long sum = 0;
  for (long i = 0; i < size.x * size.y * 4; i += 4) {
    auto value = ptr[i];
    sum += value;
  }
  return sum;
}


struct Candidate {
  sf::Vector2f position;
  sf::Vector2f size;
  float rotation;
  long score = 0;
};

void DrawCandidate(Candidate member, sf::RenderTexture* target) {
    auto shape = sf::RectangleShape();
    shape.setFillColor(sf::Color().Black);
    shape.setSize(member.size);
    shape.setPosition(member.position);
    shape.setRotation(member.rotation);
    target->draw(shape);
}

int main() {
  srand(time(NULL));
  shader.loadFromFile("./diffshader.glsl", sf::Shader::Fragment);
  sf::RenderTexture result;
  if (!result.create(size, size)) {
    return -1;
  }
  sf::Texture badApple;
  badApple.setSmooth(false);
  if (!badApple.loadFromFile("./frames/000996.bmp")) {
    return -1;
  }

  result.clear(sf::Color().White);
  result.setSmooth(false);

  sf::RenderTexture target;
  if (!target.create(size, size)) {
    return -1;
  }
  target.setSmooth(false);

  for (int iteration = 0; iteration < 10; iteration++) {
    std::cout << iteration << std::endl;
    // init population
    Candidate population[popSize];
    for (int i = 0; i < popSize; i++) {
      population[i] = {
        sf::Vector2f(rand() % size, rand() % size),
        sf::Vector2f(rand() % size / 20.0, rand() % size / 20.0),
        rand() % 1024 / 2.0f,
      };
    }

    for (int i = 0; i < popSize; i++) {
      target.clear(sf::Color::White);
      auto member = population[i];
      DrawCandidate(member, &target);
      target.display();
      auto texture = target.getTexture();
      target.clear(sf::Color::Black);
      auto score = scoreImage(diffTextures(texture, badApple, &target));
      population[i].score = score;
    }

    Candidate* best = population;
    for (int i = 1; i < popSize; i++) {
      auto member = population[i];
      if (best->score > member.score) {
        best = &population[i];
      }
    }
    DrawCandidate(best[0], &result);
    result.display();
    auto resultTexture = result.getTexture();
    auto img = resultTexture.copyToImage();
    img.saveToFile("./res.bmp");
  }
}
