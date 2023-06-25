#include "Board.h"
#include <random>
#include <fstream>

//--Code Provided to generate random numbers from Lab 6--
mt19937 random_mt;

int Random(int min, int max){
    uniform_int_distribution<int> dist(min, max);
    return dist(random_mt);}
//-------------------------------------------------------


Board::Board() {//Default constructor

    //first reading board_config.cfg file
    fstream boardConfig("files/board_config.cfg");

    string columnInfoString;
    getline(boardConfig,columnInfoString);//first line in file is column info
    this->columns = stoi(columnInfoString);

    string rowInfoString;
    getline(boardConfig,rowInfoString);//second line in file is row info
    this->rows= stoi(rowInfoString);

    this->tiles=(this->rows)*(this->columns);//total tiles is rows*columns

    string mineCountInfoString;
    getline(boardConfig,mineCountInfoString);//third line in file is mine info
    this->mineCount= stoi(mineCountInfoString);

    this->flagsToPlace=mineCount;//for now...(until the user places flags)

    //initializing rest of boolean values
    this->is_debugMode = false;
    this->is_paused = false;
    this->leaderBoard = false;
    this->loser = false;
    this->winner = false;

    //creating the tiles and adding them to a 2D Vector
    for (unsigned i = 0; i < rows; i++) {
        vector<Tile*> *currRow = new vector<Tile *>;//do pointers so that program runs quicker/uses less memory
        for (unsigned j = 0; j < columns; j++) {
            Tile *tempPointer = new Tile(j, i);//new pointer (don't worry, we'll call clear() later)
            currRow->push_back(tempPointer);}
        twoDimensionalVectorOfBoardPointer.push_back(currRow);//putting each row back into 2D vector
    }

    //Then going through the tiles and randomly assigning 50 of them to be mines
    for (int i = 0; i < this->mineCount; i++) {

        //I do the "-1" because if we have 25 rows or columns, the vector will hold info from 0,24
        int randomRow = Random(0, rows - 1);
        int randomColumn = Random(0, columns - 1);

        if (twoDimensionalVectorOfBoardPointer.at(randomRow)->at(randomColumn)->is_mine){i --;} //if it's already a mine, we go back to beginning of the for-loop and go back one iteration (to make sure we have 50 distinct mines, not just 50 times assign random value mine bc could assign same one mine more than once)
        else { twoDimensionalVectorOfBoardPointer.at(randomRow)->at(randomColumn)->is_mine = true;}//otherwise change the empty Tile into a mine Tile
    }

    //now we set each tile a number for how many surrounding mines there are
    for (int i = 0; i < twoDimensionalVectorOfBoardPointer.size(); i++) {//i is row
        for (int j = 0; j < twoDimensionalVectorOfBoardPointer.at(i)->size(); j++) {//j is column

            //here I assign each tile a vector of neighboring tiles
            if (j - 1 >= 0)//if there is a tile to the left
                twoDimensionalVectorOfBoardPointer.at(i)->at(j)->vectorOfNeighborTilePointers.push_back(twoDimensionalVectorOfBoardPointer.at(i)->at(j - 1));

            if ((i - 1 >= 0) && (j - 1 >= 0))//if there is a tile to the top-left
                twoDimensionalVectorOfBoardPointer.at(i)->at(j)->vectorOfNeighborTilePointers.push_back(twoDimensionalVectorOfBoardPointer.at(i - 1)->at(j - 1));

            if (i - 1 >= 0)//if there is a tile to the top
                twoDimensionalVectorOfBoardPointer.at(i)->at(j)->vectorOfNeighborTilePointers.push_back(twoDimensionalVectorOfBoardPointer.at(i - 1)->at(j));

            if ((i - 1 >= 0) && (j + 1 <= columns - 1))//if there is a tile to the top-right
                twoDimensionalVectorOfBoardPointer.at(i)->at(j)->vectorOfNeighborTilePointers.push_back(twoDimensionalVectorOfBoardPointer.at(i - 1)->at(j + 1));

            if (j + 1 <= columns - 1)//if there is a tile to the right
                twoDimensionalVectorOfBoardPointer.at(i)->at(j)->vectorOfNeighborTilePointers.push_back(twoDimensionalVectorOfBoardPointer.at(i)->at(j + 1));

            if ((i + 1 <= rows - 1) && (j + 1 <= columns - 1))//if there is a tile to the bottom-right
                twoDimensionalVectorOfBoardPointer.at(i)->at(j)->vectorOfNeighborTilePointers.push_back(twoDimensionalVectorOfBoardPointer.at(i + 1)->at(j + 1));

            if (i + 1 <= rows - 1)//if there is a tile to the bottom
                twoDimensionalVectorOfBoardPointer.at(i)->at(j)->vectorOfNeighborTilePointers.push_back(twoDimensionalVectorOfBoardPointer.at(i + 1)->at(j));

            if ((i + 1 <= rows - 1) && (j - 1 >= 0))//if there is a tile to the  bottom-left
                twoDimensionalVectorOfBoardPointer.at(i)->at(j)->vectorOfNeighborTilePointers.push_back(twoDimensionalVectorOfBoardPointer.at(i + 1)->at(j - 1));

            //here I assign every tile's number of surrounding mines
            if (!twoDimensionalVectorOfBoardPointer.at(i)->at(j)->is_mine) {//first make sure current tile is not a mine itself
                for (unsigned int k = 0; k < twoDimensionalVectorOfBoardPointer.at(i)->at(j)->vectorOfNeighborTilePointers.size(); k++) {
                    //for every number of neighboring mines in vector, increase the variable of surrounding mines by 1
                    if (twoDimensionalVectorOfBoardPointer.at(i)->at(j)->vectorOfNeighborTilePointers.at(k)->is_mine)
                        twoDimensionalVectorOfBoardPointer.at(i)->at(j)->surroundingMines ++;}
            }
        }
    }
}

void Board::draw(RenderWindow& window) {
    for (unsigned i=0; i < twoDimensionalVectorOfBoardPointer.size(); i++){
        for (unsigned j=0; j < twoDimensionalVectorOfBoardPointer.at(i)->size(); j++){
            twoDimensionalVectorOfBoardPointer.at(i)->at(j)->draw(window, this->is_debugMode, this-> is_paused, this->leaderBoard, this->loser, this->winner); //calling Tile's draw function
        }
    }
}

void Board::toggleDebugMode() {
    if (is_debugMode)
        is_debugMode= false;
    else if (!is_debugMode)
        is_debugMode= true;
}

void Board::togglePauseMode() {
    if (is_paused)
        is_paused= false;
    else if (!is_paused)
        is_paused= true;
}

void Board::toggleLeaderBoard() {
    if(leaderBoard)
        leaderBoard= false;
    else if(!leaderBoard)
        leaderBoard= true;
}

void Board::disableAllTiles() {
    for (unsigned i=0; i < twoDimensionalVectorOfBoardPointer.size(); i++){
        for (unsigned j=0; j < twoDimensionalVectorOfBoardPointer.at(i)->size(); j++){
            twoDimensionalVectorOfBoardPointer.at(i)->at(j)->is_enabled= false;}
    }
}

void Board::enableAllTiles() {
    for (unsigned i=0; i < twoDimensionalVectorOfBoardPointer.size(); i++){
        for (unsigned j=0; j < twoDimensionalVectorOfBoardPointer.at(i)->size(); j++){
            twoDimensionalVectorOfBoardPointer.at(i)->at(j)->is_enabled= true;}
    }
}

void Board::reveal(RenderWindow& window,Tile* tile) {
    if(tile->is_flagged)
        return;//according to instructions pdf for the project, we are not supposed to reveal any mines that have been flagged (even recursively)

    tile->is_revealed= true;

    if (tile->surroundingMines == 0&&tile->is_enabled) {//recursive case
        for (unsigned int i=0; i<tile->vectorOfNeighborTilePointers.size();i++) {//cycle through neighboring tiles
            if (!tile->vectorOfNeighborTilePointers.at(i)->is_revealed&&tile->vectorOfNeighborTilePointers.at(i)->is_enabled)
                reveal(window, tile->vectorOfNeighborTilePointers.at(i));
        }
    }
}

bool Board::checkIfWinner() {
    short count=0;
    for (unsigned i=0; i < twoDimensionalVectorOfBoardPointer.size(); i++){
        for (unsigned j=0; j < twoDimensionalVectorOfBoardPointer.at(i)->size(); j++) {
            if (!twoDimensionalVectorOfBoardPointer.at(i)->at(j)->is_mine && twoDimensionalVectorOfBoardPointer.at(i)->at(j)->is_revealed) {//checking how many tiles you revealed that aren't mines
                count++;
                if(count==(tiles-mineCount))//you win if that number is the number of total tiles minus the number of mines
                    return true;
            }
        }
    }
    return false;
}

void Board::clear() {
// Kind of like a destructor (not even sure if this was needed or if I am doing it right)
    for (unsigned i = 0; i < twoDimensionalVectorOfBoardPointer.size(); i++) {
        vector<Tile*>* currRow = twoDimensionalVectorOfBoardPointer.at(i);
        for (unsigned j = 0; j < currRow->size(); j++){
            delete currRow->at(j);}
        delete currRow;
    }
}