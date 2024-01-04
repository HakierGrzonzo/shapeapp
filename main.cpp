#include "drawer.hpp"
#include "optimizer.hpp"
#include "scorer.hpp"
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
#include <string>
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


int main(int argc, char** argv) {
  if (argc != 2) {
    return -1;
  }
  std::string filepath(argv[1]);
  srand(time(NULL));
  shader.loadFromFile("./diffshader.glsl", sf::Shader::Fragment);
  sf::RenderTexture result;
  if (!result.create(size, size)) {
    return -1;
  }
  sf::Image badApple;
  if (!badApple.loadFromFile(filepath)) {
    return -1;
  }

  Scorer scorer(badApple);
  Optimizer optimizer(scorer, 50, badApple.getSize());
  long oldScore = optimizer.getBest().score;
  int maxIterations = 10;
  while (true) {
    std::cout << std::endl;
    for (int i = 0; i < maxIterations; i++) {
      optimizer.initPopulation();
      optimizer.doIteration();
      std::cout << i << '\r';
      std::cout.flush();
    }
    auto best = optimizer.getBest();
    if (oldScore > best.score) {
      scorer.drawer.addNewShape(best);
      scorer.drawer.currentTexture().copyToImage().saveToFile("./res.bmp");
      std::system("viu res.bmp");
      oldScore = best.score;
      if (maxIterations > 3) {
        maxIterations -= 1;
      }
    } else {
      maxIterations += 10;
    }
  }
}
