#include "./optimizer.hpp"
#include "drawer.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <ostream>
#include <random>
#include <vector>

Optimizer::Optimizer(Scorer& scorer, unsigned int populationSize, sf::Vector2u size) : scorer(scorer) {
  this->populationSize = populationSize;
  this->size = size;
  this->initPopulation();
}

void Optimizer::initPopulation() {
  this->population.clear();
  this->population.reserve(populationSize);
  for (int i = 0; i < this->populationSize; i++ ) {
      ShapeSpec element = {
        sf::Vector2f(rand() % size.x, rand() % size.y),
        sf::Vector2f(rand() % size.x, rand() % size.y),
        (float) (rand() % 360),
        (rand() % 100) / 100.0f,
      };
      element.score = this->scorer.score(element);
      this->population.push_back(element);
  }
}

struct ABC {
  int a;
  int b;
  int c;
};

const int diemensionality = 6;
struct ShapeSpecArray {
  float values[diemensionality];
};

ShapeSpecArray getRandoms() {
  ShapeSpecArray r;
  for (int i = 0; i < diemensionality; i++ ) {
    r.values[i] = rand() % 100 / 100.f;
  }
  return r;
} 

ShapeSpecArray toArray(ShapeSpec original) {
  ShapeSpecArray r = {
    original.size.x,
    original.size.y,
    original.position.x,
    original.position.y,
    original.rotation,
    original.opacity,
  };
  return r;
}

ShapeSpec fromArray(ShapeSpecArray original) {
  ShapeSpec result;
  result.size.x = original.values[0];
  result.size.y = original.values[1];
  result.position.x = original.values[2];
  result.position.y = original.values[3];
  result.rotation = original.values[4];
  result.opacity = original.values[5];
  return result;
}


ABC getABC(int avoid, int maxPlusOne) {
  int result[3] = {avoid, avoid, avoid};
  int i = 0;
  while (i < 3) {
    int random = rand() % maxPlusOne;
    if (
        random == result[0] ||
        random == result[1] ||
        random == result[2]
      ) {
      continue;
    }
    result[i] = random;
    i++;
  }
  return {
    result[0],
    result[1],
    result[2],
  };
}

void Optimizer::doIteration() {
  const float CR = 0.8;
  const float F = 0.8;

  for (int agentIndex = 0; agentIndex < this->populationSize; agentIndex++ ) {
    auto original = this->population[agentIndex];
    auto x = toArray(original);

    auto abcIndexes = getABC(agentIndex, this->populationSize);
    auto a = toArray(this->population[abcIndexes.a]);
    auto b = toArray(this->population[abcIndexes.b]);
    auto c = toArray(this->population[abcIndexes.c]);

    auto R = rand() % diemensionality;
    auto p = getRandoms();

    for (int i = 0; i < diemensionality; i++) {
      if (i != R && CR < p.values[i]) {
        continue;
      }
      x.values[i] = a.values[i] + F * (b.values[i] - c.values[i]);
    }

    auto candidate = fromArray(x);
    candidate.score = this->scorer.score(candidate);

    if (candidate.score < original.score) {
      this->population[agentIndex] = candidate;
    }
  }
}

ShapeSpec Optimizer::getBest() {
  ShapeSpec best = this->population[0];
  for (auto item : this->population ) {
    if (item.score < best.score) {
      best = item;
    }
  }
  return best;
}
