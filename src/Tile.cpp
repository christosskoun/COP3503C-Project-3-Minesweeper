#include "Tile.h"

Tile::Tile( int xcoord, int ycoord) {//parameterized constructor

    //every tile should have access to the following textures:
    hiddenTileTexture.loadFromFile("files/images/tile_hidden.png");
    revealedTileTexture.loadFromFile("files/images/tile_revealed.png");
    mineTexture.loadFromFile("files/images/mine.png");
    flagTexture.loadFromFile("files/images/flag.png");
    numberOneTexture.loadFromFile("files/images/number_1.png");
    numberTwoTexture.loadFromFile("files/images/number_2.png");
    numberThreeTexture.loadFromFile("files/images/number_3.png");
    numberFourTexture.loadFromFile("files/images/number_4.png");
    numberFiveTexture.loadFromFile("files/images/number_5.png");
    numberSixTexture.loadFromFile("files/images/number_6.png");
    numberSevenTexture.loadFromFile("files/images/number_7.png");
    numberEightTexture.loadFromFile("files/images/number_8.png");

    this->surroundingMines=0;//initially before we change this later in the board constructor

    this->sprite.setPosition(xcoord * 32, ycoord * 32);//default for hidden tile

    is_flagged = false;
    is_mine = false;
    is_revealed = false;
    is_enabled= true;
}

void Tile::draw(RenderWindow& window, bool is_debugMode, bool is_paused, bool leaderBoard, bool loser, bool winner) {
    if (is_debugMode && is_mine) {
        this->sprite.setTexture(hiddenTileTexture);
        window.draw(this->sprite);

        if(is_flagged){
            this->sprite.setTexture(flagTexture);
            window.draw(this->sprite);}

        this->sprite.setTexture(mineTexture);
        window.draw(this->sprite);}

    else if (is_debugMode && !is_mine) {
        if(!is_revealed&&!is_flagged){
            this->sprite.setTexture(hiddenTileTexture);
            window.draw(this->sprite);}

        else if(!is_revealed&&is_flagged){
            this->sprite.setTexture(hiddenTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(flagTexture);
            window.draw(this->sprite);}

        else if (is_revealed&&surroundingMines==0) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);}

        else if (is_revealed&& surroundingMines==1) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberOneTexture);
            window.draw(this->sprite);}

        else if (is_revealed&& surroundingMines==2) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberTwoTexture);
            window.draw(this->sprite);}

        else if (is_revealed&& surroundingMines==3) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberThreeTexture);
            window.draw(this->sprite);}

        else if (is_revealed&& surroundingMines==4) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberFourTexture);
            window.draw(this->sprite);}

        else if (is_revealed&& surroundingMines==5) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberFiveTexture);
            window.draw(this->sprite);}

        else if (is_revealed&& surroundingMines==6) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberSixTexture);
            window.draw(this->sprite);}

        else if (is_revealed&& surroundingMines==7) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberSevenTexture);
            window.draw(this->sprite);}

        else if (is_revealed&& surroundingMines==8) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberEightTexture);
            window.draw(this->sprite);}
    }

    else if (loser && is_mine) {
        this->sprite.setTexture(revealedTileTexture);
        window.draw(this->sprite);

        if(is_flagged){
        this->sprite.setTexture(flagTexture);
        window.draw(this->sprite);}

        this->sprite.setTexture(mineTexture);
        window.draw(this->sprite);}

    else if (winner&&is_mine) {
        this->sprite.setTexture(hiddenTileTexture);
        window.draw(this->sprite);

        this->sprite.setTexture(flagTexture);
        window.draw(this->sprite);
    }

    else if (loser && !is_mine) {
        if(!is_revealed){
            this->sprite.setTexture(hiddenTileTexture);
            window.draw(this->sprite);}

        else if (is_revealed&&surroundingMines==0) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);}

        else if (is_revealed&& surroundingMines==1) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberOneTexture);
            window.draw(this->sprite);}

        else if (is_revealed&& surroundingMines==2) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberTwoTexture);
            window.draw(this->sprite);}

        else if (is_revealed&& surroundingMines==3) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberThreeTexture);
            window.draw(this->sprite);}

        else if (is_revealed&& surroundingMines==4) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberFourTexture);
            window.draw(this->sprite);}

        else if (is_revealed&& surroundingMines==5) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberFiveTexture);
            window.draw(this->sprite);}

        else if (is_revealed&& surroundingMines==6) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberSixTexture);
            window.draw(this->sprite);}

        else if (is_revealed&& surroundingMines==7) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberSevenTexture);
            window.draw(this->sprite);}

        else if (is_revealed&& surroundingMines==8) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberEightTexture);
            window.draw(this->sprite);}
    }

    else if (winner && !is_mine) {
        if(surroundingMines==0){
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);
        }
        else if (surroundingMines==1) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberOneTexture);
            window.draw(this->sprite);}

        else if (surroundingMines==2) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberTwoTexture);
            window.draw(this->sprite);}

        else if (surroundingMines==3) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberThreeTexture);
            window.draw(this->sprite);}

        else if (surroundingMines==4) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberFourTexture);
            window.draw(this->sprite);}

        else if (surroundingMines==5) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberFiveTexture);
            window.draw(this->sprite);}

        else if (surroundingMines==6) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberSixTexture);
            window.draw(this->sprite);}

        else if (surroundingMines==7) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberSevenTexture);
            window.draw(this->sprite);}

        else if (surroundingMines==8) {
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            this->sprite.setTexture(numberEightTexture);
            window.draw(this->sprite);}
    }

    else if (!is_revealed&&!is_flagged&&is_enabled) {
        this->sprite.setTexture(hiddenTileTexture);
        window.draw(this->sprite);}

    else if(!is_revealed&&is_flagged) {
        this->sprite.setTexture(hiddenTileTexture);
        window.draw(this->sprite);

        this->sprite.setTexture(flagTexture);
        window.draw(this->sprite);
    }

    else if (is_revealed&&surroundingMines==0&&!is_mine) {
        this->sprite.setTexture(revealedTileTexture);
        window.draw(this->sprite);}

    else if (is_revealed&& surroundingMines==1&&is_enabled&&!is_mine) {
        this->sprite.setTexture(revealedTileTexture);
        window.draw(this->sprite);

        this->sprite.setTexture(numberOneTexture);
        window.draw(this->sprite);}

    else if (is_revealed&& surroundingMines==2&&is_enabled&&!is_mine) {
        this->sprite.setTexture(revealedTileTexture);
        window.draw(this->sprite);

        this->sprite.setTexture(numberTwoTexture);
        window.draw(this->sprite);}

    else if (is_revealed&& surroundingMines==3&&is_enabled&&!is_mine) {
        this->sprite.setTexture(revealedTileTexture);
        window.draw(this->sprite);

        this->sprite.setTexture(numberThreeTexture);
        window.draw(this->sprite);}

    else if (is_revealed&& surroundingMines==4&&is_enabled&&!is_mine) {
        this->sprite.setTexture(revealedTileTexture);
        window.draw(this->sprite);

        this->sprite.setTexture(numberFourTexture);
        window.draw(this->sprite);}

    else if (is_revealed&& surroundingMines==5&&is_enabled&&!is_mine) {
        this->sprite.setTexture(revealedTileTexture);
        window.draw(this->sprite);

        this->sprite.setTexture(numberFiveTexture);
        window.draw(this->sprite);}

    else if (is_revealed&& surroundingMines==6&&is_enabled&&!is_mine) {
        this->sprite.setTexture(revealedTileTexture);
        window.draw(this->sprite);

        this->sprite.setTexture(numberSixTexture);
        window.draw(this->sprite);}

    else if (is_revealed&& surroundingMines==7&&is_enabled&&!is_mine) {
        this->sprite.setTexture(revealedTileTexture);
        window.draw(this->sprite);

        this->sprite.setTexture(numberSevenTexture);
        window.draw(this->sprite);}

    else if (is_revealed&& surroundingMines==8&&is_enabled&&!is_mine) {
        this->sprite.setTexture(revealedTileTexture);
        window.draw(this->sprite);

        this->sprite.setTexture(numberEightTexture);
        window.draw(this->sprite);}

    if (is_paused){
        this->sprite.setTexture(revealedTileTexture);
        window.draw(this->sprite);}

    if (leaderBoard&&!winner){//not winner bc if you win, the tiles should not be all revealed
        this->sprite.setTexture(revealedTileTexture);
        window.draw(this->sprite);}
}

void Tile::toggleFlag() {
    if (this->is_flagged)
        this->is_flagged=false;

    else if (!this->is_flagged)
        this->is_flagged=true;
}