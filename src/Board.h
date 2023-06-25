#pragma once
#include "Tile.h"

struct Board {
    //variables used:
    int rows;
    int columns;
    int tiles;
    int flagsToPlace;
    int mineCount;
    vector<vector<Tile*>*> twoDimensionalVectorOfBoardPointer; //holds all variable information for all the tiles on the board
    bool is_debugMode;
    bool is_paused;
    bool leaderBoard;
    bool loser;
    bool winner;

    //functions/methods used:
    Board();//Default constructor
    void draw(RenderWindow& window);//add function to draw/reset board
    void toggleDebugMode();
    void togglePauseMode();
    void toggleLeaderBoard();
    void disableAllTiles();
    void enableAllTiles();
    void reveal(RenderWindow& window,Tile* tile);
    bool checkIfWinner();
    void clear();//kinda like a destructor?
};