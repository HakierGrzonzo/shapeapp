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

#define MAX_ITER 60 
#define MIN_ITER 20
#define TARGET_SCORE_DIFF 500

const int size = 1024;
const int popSize = 80;

sf::Shader shader;

int main(int argc, char** argv) {
  if (argc != 3) {
    return 1;
  }
  std::string filepath(argv[1]);
  std::string resultpath(argv[2]);
  srand(time(NULL));
  shader.loadFromFile("./diffshader.glsl", sf::Shader::Fragment);
  sf::RenderTexture result;
  if (!result.create(size, size)) {
    return 1;
  }
  sf::Image badApple;
  if (!badApple.loadFromFile(filepath)) {
    return 1;
  }

  Scorer scorer(badApple);
  Optimizer optimizer(scorer, popSize, badApple.getSize());
  long oldScore = optimizer.getBest().score;
  int maxIterations = 10 + MIN_ITER;
  while (maxIterations < MAX_ITER) {
    for (int i = 0; i < maxIterations; i++) {
      optimizer.initPopulation();
      optimizer.doIteration();
      std::cout << i << '\r';
      std::cout.flush();
    }
    auto best = optimizer.getBest();
    std::cout << oldScore - best.score << std::endl;
    auto scoreDiff = oldScore - best.score;
    if (scoreDiff > 0) {
      scorer.drawer.addNewShape(best);
      oldScore = best.score;
      if (maxIterations >= MIN_ITER && scoreDiff > TARGET_SCORE_DIFF) {
        maxIterations -= 1;
      } else {
        maxIterations += 1;
      }
    } else {
      maxIterations += 10;
    }
  }
  std::cout << "Used " << scorer.drawer.size() << " shapes" << std::endl;
  scorer.drawer.currentTexture().copyToImage().saveToFile(resultpath);
}
