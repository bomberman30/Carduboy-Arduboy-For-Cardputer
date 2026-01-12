#define EEPROM_SIZE 64
#define TEST_ADDR 0

#include "ArduboyGamesController.h"
void drawFPS()
{
    static unsigned long lastMillis = 0;
    static uint16_t fpsValue = 0;
    static uint32_t frameCount = 0;

    frameCount++;
    unsigned long currentMillis = millis();

    // עדכון החישוב פעם בשנייה (או חצי שנייה)
    if (currentMillis - lastMillis >= 500)
    {
        fpsValue = (frameCount * 1000) / (currentMillis - lastMillis);
        lastMillis = currentMillis;
        frameCount = 0;

        // ציור ה-FPS למסך
        // נשתמש ב-setTextColor עם צבע רקע כדי למחוק את המספר הקודם בלי "לנקות" את כל המסך
        M5Cardputer.Display.setTextColor(TFT_GREEN, TFT_BLACK);
        M5Cardputer.Display.setCursor(0, 0);               // פינה שמאלית עליונה
        M5Cardputer.Display.printf("FPS: %u  ", fpsValue); // הרווחים בסוף מוחקים שאריות של ספרות קודמות
    }
}

void ArduboyGamesController::begin()
{
    // eeprom test
/*     arduboy.begin();
    arduboy.setFrameRate(15);

    delay(1000);

    arduboy.println("EEPROM test start");
    arduboy.display();
    delay(1000);

    if (!EEPROM.begin(EEPROM_SIZE))
    {
        arduboy.println("EEPROM.begin FAILED");
        arduboy.display();
        delay(1000);

        while (1)
            ;
    }

    uint8_t value = EEPROM.read(TEST_ADDR);
    arduboy.print("Value before: ");
    arduboy.println(value);
    arduboy.display();
    delay(1000);

    value++; // מגדיל כל אתחול
    EEPROM.write(TEST_ADDR, value);

    if (EEPROM.commit())
    {
        arduboy.print("ok new value: ");
        arduboy.println(value);
    }
    else
    {
        arduboy.println("EEPROM.commit FAILED");
    }
    arduboy.display();
    delay(1000); */

    // end of eeprom test

        arduboy.setFrameRate(15);
        GameCircuitDude = new CircuitDude(arduboy); // העברת reference

        GameCircuitDude->setup(); 
}
void ArduboyGamesController::loop()
{

    // pause render until it's time for the next frame
 
    GameCircuitDude->loop();

    // drawFPS();
}
