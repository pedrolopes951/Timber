#pragma once
#include "SFML/Graphics.hpp"
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <typeinfo>
#include <array>
constexpr uint32_t NUM_OF_BRANCHES = 6;
constexpr uint32_t NUM_OF_CLOUD = 3;
constexpr uint32_t NUM_OF_TREES = 1;
constexpr uint32_t NUM_OF_BEES = 1;
// Line the axe up with the tree
const float AXE_POSITION_LEFT = 700;
const float AXE_POSITION_RIGHT = 1075;

// Function declaration
void updateBranches(int seed);
//sf::Sprite branches[NUM_OF_BRANCHES];
std::array<sf::Sprite, NUM_OF_BRANCHES> branches;
// Where is the player/branch?
// Left or Right
enum class side { LEFT, RIGHT, NONE };
//side branchPositions[NUM_OF_BRANCHES];
std::array<side, NUM_OF_BRANCHES>  branchPositions;
