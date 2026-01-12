
#include "ArduboyGamesController.h"
ArduboyGamesController gameController;
void setup()
{
  
  // M5.begin();
  // auto cfg = M5.config();
  // M5Cardputer.begin(cfg);
  // M5Cardputer.Display.setRotation(1);
  // M5Cardputer.Display.fillScreen(BLACK);
  gameController.begin();

  // here we set the framerate to 15, we do not need to run at
  // default 60 and it saves us battery life
  
}

void loop() {
  gameController.loop();
  
}