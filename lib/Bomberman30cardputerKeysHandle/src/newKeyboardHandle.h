#ifndef NEW_KEYBOARD_HANDLE_H
#define NEW_KEYBOARD_HANDLE_H

#include <Arduino.h>

class NewKeyboardHandle {
public:
    // פריים ראשון בלבד
    static bool ifKeyJustPress(char key);
static bool ifKeyJustRelease(char key);

    // מחזיק X מילישניות, מופעל פעם אחת עד שחרור
    static bool isKeyPressForDuration(char key, int durationInMs);

    // מפעיל כל X מילישניות בזמן שהמקש מוחזק (הפעלות חוזרות)
    static bool KeyPressWorkInMilliSecond(char key, int intervalMs);

    // פונקציה חדשה: מפעיל את הכפתור מיד ואז כל intervalMs כל עוד המקש לחוץ
    static bool ApplyPushedKeyEveryMS(char key, int intervalMs);
    bool Key_Press_1_Click_And_After_Few_MS_RepeatClick(
        char key, 
        int delayBeforeRepeatMs,      // אחרי כמה זמן מתחיל הרפיט
        int repeatIntervalMs          // כל כמה זמן רפיט נוסף
    ) ;
private:
    // המערכים מוגדרים גם כאן כהצהרות (ההגדרה בפנל ה־cpp)
    static unsigned long lastPressTime[256];
    static bool wasPressed[256];
    static bool durationTriggered[256];
    static unsigned long lastWorkTime[256];
    static unsigned long applyKeyTime[256];


};

#endif // NEW_KEYBOARD_HANDLE_H
