#include "newKeyboardHandle.h"
#include <M5Cardputer.h>

// הגדרה של המערכים (הצטרפות להצהרות שב־h)
unsigned long NewKeyboardHandle::lastPressTime[256] = {0};
bool NewKeyboardHandle::wasPressed[256] = {0};
bool NewKeyboardHandle::durationTriggered[256] = {0};
unsigned long NewKeyboardHandle::lastWorkTime[256] = {0};
unsigned long NewKeyboardHandle::applyKeyTime[256] = {0};

// to test for Key_Press_1_Click_And_After_Few_MS_RepeatClick
bool wasPressed[256] = {0};
unsigned long applyKeyTime[256] = {0};
unsigned long repeatStartTime[256] = {0};

// 1) פריים הראשון בלבד
bool NewKeyboardHandle::ifKeyJustPress(char key)
{
    uint8_t idx = (uint8_t)key;
    bool pressed = M5Cardputer.Keyboard.isKeyPressed(key);
    if (pressed && !wasPressed[idx])
    {
        wasPressed[idx] = true;
        return true;
    }
    if (!pressed)
    {
        wasPressed[idx] = false;
    }
    return false;
}

bool NewKeyboardHandle::ifKeyJustRelease(char key)
{
    uint8_t idx = (uint8_t)key;
    bool pressed = M5Cardputer.Keyboard.isKeyPressed(key);

    // אם בעבר היה לחוץ ועכשיו לא — זה פריים של שחרור
    if (!pressed && wasPressed[idx])
    {
        wasPressed[idx] = false;
        return true;
    }

    // עדכון סטטוס
    if (pressed)
    {
        wasPressed[idx] = true;
    }

    return false;
}

// 2) מחזיק X מילישניות — מופעל פעם אחת רק אחרי שחלף הזמן, ולא מופעל שוב עד לשחרור
bool NewKeyboardHandle::isKeyPressForDuration(char key, int durationInMs)
{
    uint8_t idx = (uint8_t)key;
    bool pressed = M5Cardputer.Keyboard.isKeyPressed(key);
    unsigned long now = millis();

    if (pressed)
    {
        // תחילת לחיצה — נשמור זמן התחלה ונאפס את הדגל
        if (lastPressTime[idx] == 0)
        {
            lastPressTime[idx] = now;
            durationTriggered[idx] = false;
        }
        // אם עבר הזמן והפעולה עדיין לא הופעלה — הפעל פעם אחת
        if (!durationTriggered[idx] && (now - lastPressTime[idx] >= (unsigned long)durationInMs))
        {
            durationTriggered[idx] = true;
            return true;
        }
    }
    else
    {
        // שחרור — איפוס כדי לאפשר הפעלה בפעם הבאה
        lastPressTime[idx] = 0;
        durationTriggered[idx] = false;
    }

    return false;
}

// 3) מפעיל כל intervalMs כל עוד המקש לחוץ (הפעלה חוזרת)
bool NewKeyboardHandle::KeyPressWorkInMilliSecond(char key, int intervalMs)
{
    uint8_t idx = (uint8_t)key;
    bool pressed = M5Cardputer.Keyboard.isKeyPressed(key);
    unsigned long now = millis();

    if (pressed)
    {
        // אם זו הפעלה ראשונה — החזר מיד
        if (lastWorkTime[idx] == 0)
        {
            lastWorkTime[idx] = now;
            return true;
        }
        // הפעלות חוזרות כל intervalMs
        if (now - lastWorkTime[idx] >= (unsigned long)intervalMs)
        {
            lastWorkTime[idx] = now;
            return true;
        }
    }
    else
    {
        // איפוס כששוחרר המקש
        lastWorkTime[idx] = 0;
    }
    return false;
}

// פונקציה חדשה: ApplyPushedKeyEveryMS
// - תחזיר true מיד ברגע שמתחילים ללחוץ, ואז כל intervalMs כל עוד המקש לחוץ.
// - תאפס כשהמקש משוחרר.
bool NewKeyboardHandle::ApplyPushedKeyEveryMS(char key, int intervalMs)
{
    uint8_t idx = (uint8_t)key;
    bool pressed = M5Cardputer.Keyboard.isKeyPressed(key);
    unsigned long now = millis();

    if (pressed)
    {
        if (applyKeyTime[idx] == 0)
        {
            // הפעלה ראשונה מיד
            applyKeyTime[idx] = now;
            return true;
        }
        if (now - applyKeyTime[idx] >= (unsigned long)intervalMs)
        {
            applyKeyTime[idx] = now;
            return true;
        }
    }
    else
    {
        // איפוס כשמשחררים את המקש
        applyKeyTime[idx] = 0;
    }

    return false;
}

bool NewKeyboardHandle::Key_Press_1_Click_And_After_Few_MS_RepeatClick(
    char key,
    int delayBeforeRepeatMs, // אחרי כמה זמן מתחיל הרפיט
    int repeatIntervalMs     // כל כמה זמן רפיט נוסף
)
{
    uint8_t idx = (uint8_t)key;
    bool pressed = M5Cardputer.Keyboard.isKeyPressed(key);
    unsigned long now = millis();

    // אם המקש לא לחוץ — לאפס הכל
    if (!pressed)
    {
        wasPressed[idx] = false;
        applyKeyTime[idx] = 0;
        repeatStartTime[idx] = 0;
        return false;
    }

    // --- לחיצה ראשונה ---
    if (!wasPressed[idx])
    {
        wasPressed[idx] = true;
        applyKeyTime[idx] = now;  // זמן התחלת הלחיצה
        repeatStartTime[idx] = 0; // עדיין לא התחיל רפיט
        return true;              // החזרה הראשונה!
    }

    // --- התחלת רפיט אחרי זמן החזקה ---
    if (repeatStartTime[idx] == 0)
    {
        if (now - applyKeyTime[idx] >= (unsigned long)delayBeforeRepeatMs)
        {
            repeatStartTime[idx] = now; // זה הזמן שממנו מתחילים רפיט
            return true;                // רפיט ראשון
        }
        return false;
    }

    // --- רפיט כל repeatIntervalMs ---
    if (now - repeatStartTime[idx] >= (unsigned long)repeatIntervalMs)
    {
        repeatStartTime[idx] = now;
        return true;
    }

    return false;
}