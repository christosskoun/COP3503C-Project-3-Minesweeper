#include <iostream>
#include <fstream> //for file input and output
#include <string>
#include <cctype> //Used in welcome window to make sure letters are in right case
#include <sstream> //used to split up string stream when reading in text file for leaderboard information
#include <SFML/Graphics.hpp> //Allows Graphics Interface to be utilized
#include "Board.h" //comes with Tile.h
#include "Stopwatch.h" //Allows you to keep time as well as start/stop

using namespace std; //don't have to type "std" everytime use standard library
using namespace sf; //don't have to type "sf" everytime use sfml library

//Function used in setText() below provided by professor that draws an imaginary text box around text to center it properly
void getTextRect(Text &text, float xcoord, float ycoord){
    FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top + textRect.height/2.0f);
    text.setPosition(Vector2f(xcoord, ycoord));}

//Stores all information needed to display text including color, font, and size
Text setText(const string& textString, Font& font, short size, Color color, float xcoord, float ycoord){
    Text text(textString, font,size);
    text.setFillColor(color);
    getTextRect(text,xcoord,ycoord);
    return text;}

//Used to store leaderboard information
struct Player {
    short timeInSec;
    string name;};

int main(){
    Board gameBoard;//this line takes a long while to load so there may be a delay until the window pops up

    int mainWindowWidth=gameBoard.columns*32;//for Welcome and Game window
    int mainWindowHeight=gameBoard.rows*32+100;//for Welcome and Game window

    int leaderboardWindowWidth=gameBoard.columns*16;//for Welcome and Game window
    int leaderboardWindowHeight=gameBoard.rows*16+50;//for Welcome and Game window

    //reading in font information from given file
    Font font;
    fstream fontFile("files/font.ttf");
    if (!font.loadFromFile("files/font.ttf"))
        cout << "Failed to load font file!" << endl;

    String userInput;
    Text userName;

    RenderWindow welcomeWindow(VideoMode(mainWindowWidth, mainWindowHeight), "WELCOME");
    while (welcomeWindow.isOpen()) {//runs until red "X" on welcome window is pressed
        Event event;
        Text welcomeText=setText("WELCOME TO MINESWEEPER!",font, 24,Color::White,mainWindowWidth/2.0f,(mainWindowHeight/2.0f)-150);
        welcomeText.setStyle(Text::Bold| Text::Underlined);

        Text inputPromptText=setText("Enter your name:",font, 20,Color::White,mainWindowWidth/2.0f,(mainWindowHeight/2.0f)-75);
        inputPromptText.setStyle(Text::Bold);

        while (welcomeWindow.pollEvent(event)) {//continuously checks for if a defined event is clicked in the welcome window
            if (event.type == Event::Closed) {
                welcomeWindow.close();
                return 0;}//so that the game window will not automatically open if the user presses the "x" button
            else if (event.type == Event::TextEntered) {
                if (isalpha(event.text.unicode) &&userInput.getSize() < 10) {//makes sure you cannot type more than 10 letters and not any numbers/special characters

                    if (userInput.getSize()==0){//make first letter capitalized
                        char temp=static_cast<char>(event.text.unicode);//needed because unicode and char are different and only way to cast it using sfml
                        temp = toupper(temp);//forcing character to be uppercase
                        userInput+=temp;}//adding the character to the string name

                    else {//for every letter that's not the first letter
                        char temp=static_cast<char>(event.text.unicode);//needed because unicode and char are different and only way to cast it using sfml
                        temp = tolower(temp);//forcing character to be lowercase
                        userInput+=temp;}//adding the character to the string name

                    userName = setText(userInput+"|", font, 18, Color::Yellow, mainWindowWidth / 2.0f,(mainWindowHeight / 2.0f) - 45);//added "|" for the cursor
                    userName.setStyle(Text::Bold);}
            }
            else if (event.type == Event::KeyPressed &&event.key.code == Keyboard::Enter&&userInput.getSize() != 0) {//checking if user pressed "enter" key and if so, closes window
                welcomeWindow.close();
            }
            else if (event.type == Event::KeyPressed && event.key.code == Keyboard::BackSpace &&userInput.getSize() > 0) {//checking if user pressed "backspace" key and if so, remove letters from their name (need at least one letter on screen to remove)
                userInput.erase((userInput.getSize() - 1));
                userName = setText(userInput+"|", font, 18, Color::Yellow, mainWindowWidth / 2.0f, (mainWindowHeight / 2.0f) - 45);
                userName.setStyle(Text::Bold);}//erase() removes the character at the assigned index of a string
        }
        //The following are updated ever frame:
        welcomeWindow.clear(Color::Blue);//sets window background to blue
        welcomeWindow.draw(welcomeText);
        welcomeWindow.draw(inputPromptText);
        welcomeWindow.draw(userName);//This is the name that's being typed as the user types in their name
        welcomeWindow.display();//needed to show updated changes
    }

//Loading and setting all button images and positions:
    Sprite faceSprite;
    faceSprite.setPosition((((gameBoard.columns)/2)*32)-32,32*(gameBoard.rows+0.5));
    Texture faceTexture;
    faceTexture.loadFromFile("files/images/face_happy.png");//initially starts off as a happy face
    faceSprite.setTexture(faceTexture);
    //no enabled bool as will always be enabled

    Sprite debugSprite;
    debugSprite.setPosition(((gameBoard.columns)*32)-304,32*(gameBoard.rows+0.5));
    Texture debugTexture;
    debugTexture.loadFromFile("files/images/debug.png");
    debugSprite.setTexture(debugTexture);
    bool debugButtonEnabled=true;//will be false during game over screen

    Sprite pauseSprite;
    pauseSprite.setPosition(((gameBoard.columns)*32)-240,32*(gameBoard.rows+0.5));
    Texture pauseTexture;
    pauseTexture.loadFromFile("files/images/pause.png");
    pauseSprite.setTexture(pauseTexture);
    bool pauseButtonEnabled=true;//will be false during game over screen

    Sprite leaderboardSprite;
    leaderboardSprite.setPosition(((gameBoard.columns)*32)-176,32*(gameBoard.rows+0.5));
    Texture leaderboardTexture;
    leaderboardTexture.loadFromFile("files/images/leaderboard.png");
    leaderboardSprite.setTexture(leaderboardTexture);
    //no enabled bool as will always be enabled

//Loading number image file and splitting the image up by number/symbol (negative sign) (11 total subimages)
    Sprite digitsSprite[11];
    Texture digitsTexture;
    digitsTexture.loadFromFile("files/images/digits.png");
    for (unsigned int i=0;i<11;i++){
        digitsSprite[i].setTexture(digitsTexture);
        digitsSprite[i].setTextureRect(IntRect (i*21,0,21,32));}

    StopWatch gameClock; //clock starts at this line

    vector<Player> vectorOfActiveHighscores; //Includes the top five highscores shown on screen
    vector<Player> vectorOfALLHighscoreFileLines; //All highscores even after 5th place

    int newWinnerPosition=-1;//indicates the name with a star* next to it is nonexistent (ie no recent high score)

    // Loading the "already set" highscores
    string tempLine;//because we are going to read in one row at a time
    fstream highscoreFile("files/leaderboard.txt", ios_base::in); //declaring the file

//read in All lines from leaderboard.txt
    while(getline(highscoreFile, tempLine)) {//looks over each row one at a time
        Player tempPlayer;
        stringstream ss(tempLine); //putting the line into a string stream allows us to separate by delimiters later

        string highscoreMinString;//first info given in row
        getline(ss, highscoreMinString, ':');//find the value by reading up until the colon appears
        int highScoreMin = stoi(highscoreMinString);

        string highscoreSecString;//second info given in row
        getline(ss, highscoreSecString, ',');//find the value by reading up until the comma appears
        int highScoreSec = stoi(highscoreSecString);

        highScoreSec += (highScoreMin *60); //put users time into all seconds so can compare who had the faster time later on
        tempPlayer.timeInSec = highScoreSec; //storing time in seconds into class object

        getline(ss, tempPlayer.name);//find the value by reading up until the end of the ss

        vectorOfALLHighscoreFileLines.push_back(tempPlayer);//stores each highscore file lines into a vector
    }
    highscoreFile.close(); //good practice :)

//read in highscores from leaderboard.txt (first 5 lines)
    highscoreFile.open("files/leaderboard.txt", ios_base::in);
    for(unsigned int i=0; i<5&&getline(highscoreFile, tempLine); i++) {//looks over each row one at a time but no more than the first 5 lines of the file
        Player tempPlayer;
        stringstream ss(tempLine); //putting the line into a string stream allows us to separate by delimiters later

        string highscoreMinString;//first info given in row
        getline(ss, highscoreMinString, ':');//find the value by reading up until the colon appears
        int highScoreMin = stoi(highscoreMinString);

        string highscoreSecString;//second info given in row
        getline(ss, highscoreSecString, ',');//find the value by reading up until the comma appears
        int highScoreSec = stoi(highscoreSecString);

        highScoreSec += (highScoreMin *60); //put users time into all seconds so map automatically orders by fastest to slowest
        tempPlayer.timeInSec = highScoreSec;//storing time in seconds into class object

        getline(ss, tempPlayer.name);//find the value by reading up until the end of the ss

        vectorOfActiveHighscores.push_back(tempPlayer);//stores each highscores' values into a vector
    }
    highscoreFile.close(); //good practice :)

    RenderWindow gameWindow(VideoMode(mainWindowWidth, mainWindowHeight), "Minesweeper");
    while (gameWindow.isOpen()){//runs until red "X" on window is pressed
        gameWindow.clear(Color::White);
        gameBoard.draw(gameWindow);

        int currentUserMins=gameClock.getElapsedTime().asSeconds()/60;
        int currentUserSecs=((int)gameClock.getElapsedTime().asSeconds())%60;

        if (gameBoard.flagsToPlace<0){
            digitsSprite[10].setPosition(12, (32*(gameBoard.rows+0.5))+16);
            gameWindow.draw(digitsSprite[10]);
        }

        if(gameBoard.flagsToPlace>=0){//if flags left are positive, continue as normal
        int mineCountHundredsDigit = gameBoard.flagsToPlace / 100;
        digitsSprite[mineCountHundredsDigit].setPosition(33, (32*(gameBoard.rows+0.5))+16);
        gameWindow.draw(digitsSprite[mineCountHundredsDigit]);

        int mineCountTensDigit = (gameBoard.flagsToPlace - (mineCountHundredsDigit * 100)) / 10;
        digitsSprite[mineCountTensDigit].setPosition(54, (32*(gameBoard.rows+0.5))+16);
        gameWindow.draw(digitsSprite[mineCountTensDigit]);

        int mineCountOnesDigit =gameBoard.flagsToPlace - (mineCountHundredsDigit * 100) - (mineCountTensDigit * 10);
        digitsSprite[mineCountOnesDigit].setPosition(75, (32*(gameBoard.rows+0.5))+16);
        gameWindow.draw(digitsSprite[mineCountOnesDigit]);}

        else if(gameBoard.flagsToPlace<0){// if mine count is negative
            gameBoard.flagsToPlace=abs(gameBoard.flagsToPlace);//make it temporarily positive to do calculations
            int mineCountHundredsDigit = gameBoard.flagsToPlace / 100;
            digitsSprite[mineCountHundredsDigit].setPosition(33, (32*(gameBoard.rows+0.5))+16);
            gameWindow.draw(digitsSprite[mineCountHundredsDigit]);

            int mineCountTensDigit = (gameBoard.flagsToPlace - (mineCountHundredsDigit * 100)) / 10;
            digitsSprite[mineCountTensDigit].setPosition(54, (32*(gameBoard.rows+0.5))+16);
            gameWindow.draw(digitsSprite[mineCountTensDigit]);

            int mineCountOnesDigit =gameBoard.flagsToPlace - (mineCountHundredsDigit * 100) - (mineCountTensDigit * 10);
            digitsSprite[mineCountOnesDigit].setPosition(75, (32*(gameBoard.rows+0.5))+16);
            gameWindow.draw(digitsSprite[mineCountOnesDigit]);

            gameBoard.flagsToPlace*=-1;//then revert it back to negative for rest of code logic
        }

        //Setting location of time digits on bottom-right of screen
        digitsSprite[currentUserMins/10].setPosition((gameBoard.columns*32)-97,(32*(gameBoard.rows+0.5))+16); //left minute's digit
        gameWindow.draw(digitsSprite[currentUserMins/10]); //have to draw right away so can have multiple digits of same number on screen at once

        digitsSprite[currentUserMins%10].setPosition((gameBoard.columns*32)-76,(32*(gameBoard.rows+0.5))+16); //right minute's digit
        gameWindow.draw(digitsSprite[currentUserMins%10]); //have to draw right away so can have multiple digits of same number on screen at once


        digitsSprite[currentUserSecs/10].setPosition((gameBoard.columns*32)-54,(32*(gameBoard.rows+0.5))+16); //left second's digit
        gameWindow.draw(digitsSprite[currentUserSecs/10]); //have to draw right away so can have multiple digits of same number on screen at once

        digitsSprite[currentUserSecs%10].setPosition((gameBoard.columns*32)-33,(32*(gameBoard.rows+0.5))+16); //right second's digit
        gameWindow.draw(digitsSprite[currentUserSecs%10]); //have to draw right away so can have multiple digits of same number on screen at once

        Event gameEvent;

        //To store location of where a mouse button click occurred
        auto gameWindowClickPosition = Mouse::getPosition(gameWindow);
        auto gameWindowClick = gameWindow.mapPixelToCoords(Mouse::getPosition(gameWindow));

        //The following are updated ever frame:
        gameWindow.draw(faceSprite);
        gameWindow.draw(debugSprite);
        gameWindow.draw(pauseSprite);
        gameWindow.draw(leaderboardSprite);
        gameWindow.display();

        while (gameWindow.pollEvent(gameEvent)) {//continuously checks for if a defined event is clicked in the game window

            if (gameBoard.leaderBoard) {//bool will be true if user won game or if the leaderboard button was left-clicked on
                RenderWindow leaderboardWindow(VideoMode(leaderboardWindowWidth, leaderboardWindowHeight),"Leaderboard");
                while (leaderboardWindow.isOpen()) {//runs until red "X" on window is pressed

                    leaderboardWindow.clear(Color::Blue);//makes background of the leaderboard window blue
                    Text leaderBoardText = setText("LEADERBOARD", font, 20, Color::White, leaderboardWindowWidth / 2.0f,(leaderboardWindowHeight / 2.0f) - 120);
                    leaderBoardText.setStyle(Text::Bold | Text::Underlined);

                    string combiningHighscoreText="";//empty string gets added to after all highscores are loaded
                    for (unsigned int i=0; i<vectorOfActiveHighscores.size();i++) {//iterates through all top 5 highscores in .txt file
                        short tempUserTimeInSeconds = vectorOfActiveHighscores.at(i).timeInSec;
                        int tempUserMinInt=tempUserTimeInSeconds/60;
                        string tempUserMinStr;
                        if(tempUserMinInt<10)//if minutes don't take two digits
                            tempUserMinStr=((string)"0")+to_string(tempUserMinInt);//make them have two digits
                        else//minutes are two digits
                            tempUserMinStr=to_string(tempUserMinInt);
                        int tempUserSecInt=tempUserTimeInSeconds%60;
                        string tempUserSecStr;
                        if(tempUserSecInt<10)//if seconds don't take two digits
                            tempUserSecStr=((string)"0")+to_string(tempUserSecInt);//make them have two digits
                        else//seconds take two digits
                            tempUserSecStr=to_string(tempUserSecInt);
                        string tempUserTime=tempUserMinStr+":"+tempUserSecStr;
                        string tempUserName =vectorOfActiveHighscores.at(i).name;

                        if(newWinnerPosition>=0){//initially set to -1 and changes when user wins and beats a top 5 high score
                            if(i==newWinnerPosition)
                                tempUserName+="*";}//add the start to the end of the name that is new

                        //Split the following up because the first line doesn't need two end-lines before it, only the ones after for formatting reasons
                        if(i==0)
                            combiningHighscoreText+= to_string(i+1)+"\t"+tempUserTime+"\t"+tempUserName;
                        else
                            combiningHighscoreText += (string) "\n\n" + to_string(i + 1) + "\t" + tempUserTime + "\t" + tempUserName;
                    }

                    Text highscoreText = setText(combiningHighscoreText, font, 18, Color::White, leaderboardWindowWidth / 2.0f,(leaderboardWindowHeight / 2.0f) + 20);
                    highscoreText.setStyle(Text::Bold);

                    Event leaderBoardEvent;
                    while (leaderboardWindow.pollEvent(leaderBoardEvent)) {//continuously checks for if a defined event is clicked in the leaderboard window
                        if (leaderBoardEvent.type == Event::Closed) {//closes leaderboard window if user presses the red X
                            //The following is code for what I thought would let me turn off the leaderboard by pressing the leaderboard button again, but it didn't work: ||(leaderboardSprite.getGlobalBounds().contains(gameWindowClick)&&gameEvent.type == Event::MouseButtonPressed)
                            gameBoard.leaderBoard= false;
                            if(!gameBoard.is_paused&&!gameBoard.loser&&!gameBoard.winner){//because if the user paused the game before the leaderboard button was pressed, the game should remain paused when the window closes as well as if the user has already won, closing the leaderboard should not reenable the time or the buttons
                                gameBoard.enableAllTiles();//Able to click on tiles again
                                gameClock.start();}//clock is paused initially during leaderboard window but starts once more when you close the window

                            leaderboardWindow.close();}
                    }
                    //The following are updated ever frame:
                    leaderboardWindow.draw(leaderBoardText);
                    leaderboardWindow.draw(highscoreText);
                    leaderboardWindow.display();
                }
            }

            if (gameEvent.type == Event::Closed) {//closes game window if user presses the red "X"
                gameBoard.clear();
                gameWindow.close();
                return 0;}

            if (gameEvent.type == Event::MouseButtonPressed &&!gameBoard.leaderBoard) { // Get the position of each click to help visualize things(doesn't do anything if leaderBoard window is open)
                cout << "Mouse clicked at position (" << (gameWindowClickPosition.x / 32) << ", "<< (gameWindowClickPosition.y / 32) << ")"<< endl; //divide by 32 to see which column and row exactly and print info to console

                if (gameEvent.mouseButton.button == sf::Mouse::Left) {//if user left-clicks the mouse:

                    //Go through every Tile object in the board
                    for (unsigned i = 0; i < gameBoard.twoDimensionalVectorOfBoardPointer.size(); i++) {
                        for (unsigned j = 0; j < gameBoard.twoDimensionalVectorOfBoardPointer.at(i)->size(); j++) {

                            //if you clicked on a tile, and it was enabled, check if you clicked on a mine or not
                            if (gameBoard.twoDimensionalVectorOfBoardPointer.at(i)->at(j)->sprite.getGlobalBounds().contains(gameWindowClick) && gameBoard.twoDimensionalVectorOfBoardPointer.at(i)->at(j)->is_enabled) {

                                //if you did not click on a mine:
                                if (!gameBoard.is_paused && !gameBoard.is_debugMode && !gameBoard.leaderBoard && !gameBoard.twoDimensionalVectorOfBoardPointer.at(i)->at(j)->is_mine && !gameBoard.twoDimensionalVectorOfBoardPointer.at(i)->at(j)->is_flagged) // we can only reveal tiles when we are not in debug or pause mode
                                    gameBoard.reveal(gameWindow, gameBoard.twoDimensionalVectorOfBoardPointer.at(i)->at(j));//calls recursive function to remove nearby tiles

                                //if you clicked on a mine (you lost)
                                else if (gameBoard.twoDimensionalVectorOfBoardPointer.at(i)->at(j)->is_mine && gameBoard.twoDimensionalVectorOfBoardPointer.at(i)->at(j)->is_enabled && !gameBoard.twoDimensionalVectorOfBoardPointer.at(i)->at(j)->is_flagged) {
                                    cout << "You Lost!" << endl;
                                    faceTexture.loadFromFile("files/images/face_lose.png"); //changing happy face to dead face
                                    gameClock.stop();
                                    gameBoard.loser = true;
                                    gameBoard.disableAllTiles();
                                    debugButtonEnabled = false;
                                    pauseButtonEnabled = false;
                                }
                            }
                        }
                    }

                    if (faceSprite.getGlobalBounds().contains(gameWindowClick)) {//if someone pressed the face button
                        cout << "RESTARTING" << endl;

                        if (gameBoard.loser)//if you're a loser but you tryna reset
                            gameBoard.loser = false; //you're no longer a loser

                        if (gameBoard.winner)//if you're a winner but you tryna reset
                            gameBoard.winner = false; //you're no longer a winner

                        Board newGameBoard; //creates a new board with new random mine information, etc.
                        gameBoard.clear();//clear memory from old board using constructor
                        gameBoard = newGameBoard; //sets the current board to be the new board
                        faceTexture.loadFromFile("files/images/face_happy.png");//new boards start off with a happy face
                        gameClock.restart();//put clock back to zero
                        gameClock.start();//needed bc if lose clock stops
                        pauseButtonEnabled = true;
                        debugButtonEnabled = true;
                        //Note: don't need to enableAllTiles because newGameBoard comes with all tiles initialized to being enabled
                    }

                    if (debugSprite.getGlobalBounds().contains(gameWindowClick) &&debugButtonEnabled) {//if someone pressed the debugger button
                        cout << "debug button pressed" << endl;
                        gameBoard.toggleDebugMode();
                        if (gameBoard.is_debugMode)
                            gameBoard.disableAllTiles(); //not allowed to interact with game Tiles when "debug mode" is activated
                        else if(!gameBoard.is_debugMode)
                            gameBoard.enableAllTiles();
                    }

                    if (pauseSprite.getGlobalBounds().contains(gameWindowClick) &&pauseButtonEnabled) {//if someone pressed the debugger button
                        if (gameClock.isPaused())
                            gameClock.start();
                        else
                            gameClock.stop();
                        cout << "pause button pressed" << endl;
                        gameBoard.togglePauseMode();//for drawing the correct tiles only (couldn't put time logic into this function as the time information was defined in the main)
                        if (gameBoard.is_paused){
                            gameClock.stop();
                            gameBoard.disableAllTiles();
                            debugButtonEnabled= false;
                            pauseTexture.loadFromFile("files/images/play.png");}//change sprite to play

                        else if (!gameBoard.is_paused){
                            gameBoard.enableAllTiles();
                            debugButtonEnabled= true;
                            gameClock.start();
                            pauseTexture.loadFromFile("files/images/pause.png");}//change sprite to pause
                        }

                    if (leaderboardSprite.getGlobalBounds().contains(gameWindowClick) &&!gameBoard.leaderBoard) {//if someone pressed the leaderBoard button
                        cout << "leaderboard button pressed" << endl;

                        //first, we have to essentially pause the game (don't togglePauseMode because the game is technically not paused and if someone pauses the game before they open the leaderboard, the game should not resume right when the leaderboard window closes, the game must also be unpaused first)
                        gameClock.stop();
                        gameBoard.disableAllTiles();
                        gameBoard.toggleLeaderBoard();}
                }

                else if (gameEvent.mouseButton.button == sf::Mouse::Right) {//if user right-clicks the mouse:

                    //Go through every Tile object in the board
                    for (unsigned i = 0; i < gameBoard.twoDimensionalVectorOfBoardPointer.size(); i++) {
                        for (unsigned j = 0; j < gameBoard.twoDimensionalVectorOfBoardPointer.at(i)->size(); j++) {

                            //making sure that the game is not in state (debug mode, paused, winner screen, etc.) before a flag is placed
                            if (gameBoard.twoDimensionalVectorOfBoardPointer.at(i)->at(j)->sprite.getGlobalBounds().contains(gameWindowClick) && !gameBoard.is_paused && !gameBoard.is_debugMode && !gameBoard.loser && !gameBoard.winner) {

                                //if a flag was placed down
                                if (!gameBoard.twoDimensionalVectorOfBoardPointer.at(i)->at(j)->is_flagged && !gameBoard.twoDimensionalVectorOfBoardPointer.at(i)->at(j)->is_revealed) {
                                    gameBoard.flagsToPlace--;//decrement mineCount
                                    gameBoard.twoDimensionalVectorOfBoardPointer.at(i)->at(j)->toggleFlag();}

                                //if a flag was removed
                                else if (gameBoard.twoDimensionalVectorOfBoardPointer.at(i)->at(j)->is_flagged && !gameBoard.twoDimensionalVectorOfBoardPointer.at(i)->at(j)->is_revealed && gameBoard.flagsToPlace < gameBoard.mineCount) {
                                    gameBoard.flagsToPlace++;//increment mineCount
                                    gameBoard.twoDimensionalVectorOfBoardPointer.at(i)->at(j)->toggleFlag();}
                            }
                        }
                    }
                }
            }
        }

        if(gameBoard.checkIfWinner()) {//goes into if-statement if the user has won the game
            gameClock.stop();//stop the clock first thing to get most accurate time for leaderboard
            gameBoard.flagsToPlace=0;//instructions told me to do this

            if(!gameBoard.winner) {//this is statement will make sure the leaderboard window appears initially when won but not again once it's closed. additionally the gameWon state of the board will remain even if the user closes the leaderboard window
                gameBoard.leaderBoard = true;

                //checking if user deserves to have their score on the leaderboard
                newWinnerPosition = -1;//indicates the name with a star* next to it is nonexistent (at least for now...)

                Player fifthPlacePlayer=vectorOfActiveHighscores.back();//setting 5th place as the last player in the ACTIVE high scores vector
                Player lastPlacePlayer=vectorOfALLHighscoreFileLines.back();//setting last place as the last player in the vector including ALL of the scores

                //setting up a Player for the user who just won the game so we can add them to the active leaderboard and the .txt file
                Player tempPlayer;
                tempPlayer.name = userInput;
                tempPlayer.timeInSec = currentUserSecs + (currentUserMins * 60);//getting the clock's elapsed time has too many decimal points, this is the time without them from the variable used for the timer


                if (tempPlayer.timeInSec<fifthPlacePlayer.timeInSec) {//if the user's time was quicker than the slowest time on the leaderboard
                    vectorOfActiveHighscores.pop_back();//remove the previous slowest score

                    for (auto iter = vectorOfActiveHighscores.begin(); iter != vectorOfActiveHighscores.end(); iter++) {//this for-loop will only work for inserting the new high score if the new score is top 1-4 (not 5 because of the way the .insert() function works)
                        newWinnerPosition++;//will keep incrementing where the star* goes until the insert is successful
                        if (tempPlayer.timeInSec < iter->timeInSec) {
                            vectorOfActiveHighscores.insert(iter, tempPlayer);
                            break;}
                    }
                    // check if the new high score was not inserted already, and perhaps it'll actually take the 5th spot
                    if (vectorOfActiveHighscores.size() < 5) {//because we may only display 5 highscores at a time in the leaderboard window
                        newWinnerPosition++;//to make sure star* goes from spot 4 to spot 5
                        vectorOfActiveHighscores.push_back(tempPlayer);}//can put ths one at end because this vector only holds data for top 5
                }

                    for (auto iter = vectorOfALLHighscoreFileLines.begin(); iter != vectorOfALLHighscoreFileLines.end(); iter++) {//this for-loop will insert the new high score into vectorOfAllHighscoreFileLines
                        if (tempPlayer.timeInSec< iter->timeInSec) {
                            vectorOfALLHighscoreFileLines.insert(iter, tempPlayer);//This will only add to vectorOfAllHighscoreFileLines
                            break;}
                    }

                    if (tempPlayer.timeInSec>=lastPlacePlayer.timeInSec)//if the player won the game with the worst time so far in history
                        vectorOfALLHighscoreFileLines.push_back(tempPlayer);

                    highscoreFile.open("files/leaderboard.txt", std::ios::out | std::ios::trunc);//re-opends leaderboard text file but clears it.
                    string tempFileMinsStr;
                    string tempFileSecsStr;
                    for (auto iter = vectorOfALLHighscoreFileLines.begin(); iter != vectorOfALLHighscoreFileLines.end(); iter++) {
                        short tempFileMins=(iter->timeInSec)/60;
                        short tempFileSecs=(iter->timeInSec)%60;

                        if(tempFileMins>=10)//no need to add the zero beforehand
                            tempFileMinsStr= to_string(tempFileMins);
                        else if (tempFileMins<10)//must add zero before mins digit
                            tempFileMinsStr="0"+ to_string(tempFileMins);

                        if(tempFileSecs>=10)//no need to add the zero beforehand
                            tempFileSecsStr=to_string(tempFileSecs);
                        else if (tempFileSecs<10)//must add zero before seconds digit
                            tempFileSecsStr="0"+to_string(tempFileSecs);

                        highscoreFile<<tempFileMinsStr+":"+tempFileSecsStr+","+iter->name<<endl;//writes updated highscores into the .txt file
                    }
                    highscoreFile.close();//good practice :)

            }
            gameBoard.winner = true; //now the leaderboard will not continuously "pop-up" when in the winning state
            faceTexture.loadFromFile("files/images/face_win.png");//new face picture for winning
            gameBoard.disableAllTiles();//cannot interact with game tiles when you are in the "win-screen"
            debugButtonEnabled = false;
            pauseButtonEnabled = false;
        }
    }
    return 0;}