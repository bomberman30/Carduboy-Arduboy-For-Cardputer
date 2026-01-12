#pragma once
#include <M5Cardputer.h>
#include <M5Unified.h>
#include <Arduboy2.h>
#include "Games/CircuitDude.h"

class ArduboyGamesController
{
private:
public:
    Arduboy2 arduboy;

    CircuitDude *GameCircuitDude;
    ArduboyGamesController(/* args */) {}
    ~ArduboyGamesController() {}
    void begin();
    void loop();
};
