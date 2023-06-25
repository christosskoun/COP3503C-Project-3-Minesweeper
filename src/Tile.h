#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

class Tile{
    Texture hiddenTileTexture;
    Texture revealedTileTexture;
    Texture mineTexture;
    Texture flagTexture;
    Texture numberOneTexture;
    Texture numberTwoTexture;
    Texture numberThreeTexture;
    Texture numberFourTexture;
    Texture numberFiveTexture;
    Texture numberSixTexture;
    Texture numberSevenTexture;
    Texture numberEightTexture;

public:
    Sprite sprite;
    vector<Tile*> vectorOfNeighborTilePointers;//stores Tile information for a maximum of 8 neighboring tiles for each tile

    bool is_mine;
    bool is_flagged;
    bool is_enabled;
    bool is_revealed;
    int surroundingMines; //public so can allocate in Board constructor

    Tile(int xcoord, int ycoord); //parameterized constructor
    void draw(RenderWindow& window, bool is_debugMode, bool is_paused,bool leaderBoard, bool loser, bool winner);//draw function that takes in window by reference and draws sprite into window (called in Board's default constructor)
    void toggleFlag();
};