//NOTE: This is a header file created by Christos Skoundridakis that creates a class for a StopWatch with active functionality for starting, stopping, and restarting elapsed time using sfml clock

#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class StopWatch{
    //The following are PRIVATE variables by default
    Clock gameClock;
    Time gameTime;
    bool paused= false;

public:
    StopWatch(){
        gameTime=Time::Zero; //time starts at zero (doesn't change unless we change value)
        gameClock.restart(); //we restart this clock that automatically counts
    }

    void start(){
        if(paused){//means we about to resume time
            gameClock.restart();//keeps track of time from last resume
            paused=false;}
    }

    void stop(){
        if(!paused){//means we about to stop time
            gameTime+=gameClock.getElapsedTime();//gameTime updates to latest time (basically temp variable)
            paused=true;}
    }

    void restart(){//basically same as constructor but don't want a new object
        gameTime=Time::Zero; //time starts at zero (doesn't change unless we change value)
        gameClock.restart();//we restart this clock that automatically counts (this restart is from sfml not my function)
    }

    Time getElapsedTime(){
        if(!paused)//if game is running
            return gameTime+gameClock.getElapsedTime();
        else
            return gameTime;// if game is paused
    }

    bool isPaused(){
        return paused;}
};