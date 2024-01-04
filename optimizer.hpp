#pragma once

#include "./drawer.hpp"
#include "./scorer.hpp"
#include <SFML/Graphics/Image.hpp>
#include <vector>

class Optimizer {
  public:
    Optimizer(Scorer& scorer, unsigned int populationSize, sf::Vector2u size);

    ShapeSpec getBest();
    void initPopulation();
    void doIteration();

  private:
    sf::Vector2u size;
    unsigned int populationSize;
    Scorer& scorer;
    std::vector<ShapeSpec> population;
};
