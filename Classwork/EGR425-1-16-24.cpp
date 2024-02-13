#include <Arduino.h>
#include <M5Core2.h>

/////////////////////////////////////////////
// CHALLENGE: 
//    MODE 1: GREEN=>YELLOW=>RED=>GREEN... (every 1s)
//    MODE 2: RED=>BLACK=>RED (every 1s)
//    BTN_A toggles between modes 1 and 2; init to mode 1
/////////////////////////////////////////////

// Create all my possible states and modes
enum State { S_GREEN, S_YELLOW, S_RED, S_BLACK };
enum Mode { M_NORMAL, M_EMERGENCY };

// Initialize variables
static bool stateChangedThisLoop = false;
static unsigned long lastTS = 0;
static State state = S_GREEN;
static Mode mode = M_NORMAL;

void setup() {
  // Initialize the device so it is responsive
  M5.begin();
}

void loop() {
    ////////////////////////////////////////////////////////////////////
    // STATE TRANSITIONS
    ////////////////////////////////////////////////////////////////////
    // Process state transition based on button press (mode change)
    M5.update();
    if (M5.BtnA.wasPressed()) {
        if (mode == M_NORMAL) {
            mode = M_EMERGENCY;
            state = S_BLACK;
        } else { // mode == M_EMERGENCY
            mode = M_NORMAL;
            state = S_GREEN;
        }

        // Update the timestamp and reset the state change flag
        stateChangedThisLoop = true;
        lastTS = millis();
    }

    // Process state transition based on time (1s interval)
    if (millis() >= lastTS + 1000) {
        // Update the timestamp and reset the state change flag
        stateChangedThisLoop = true;
        lastTS = millis();

        // Update the state based on the current mode
        if (mode == M_NORMAL) {
            if (state == S_GREEN)
                state = S_YELLOW;
            else if (state == S_YELLOW)
                state = S_RED;
            else if (state == S_RED)
                state = S_GREEN;
        } else { // mode == M_EMERGENCY
            if (state == S_RED)
                state = S_BLACK;
            else if (state == S_BLACK)
                state = S_RED;
        }
        
    }

    ////////////////////////////////////////////////////////////////////
    // STATE EXECUTION/OUTPUT
    ////////////////////////////////////////////////////////////////////
    // Take action based on the current state (i.e., set the LCD screen color)
    if (stateChangedThisLoop) {
        if (state == S_GREEN)
            M5.Lcd.fillScreen(GREEN);
        else if (state == S_YELLOW)
            M5.Lcd.fillScreen(YELLOW);
        else if (state == S_RED)
            M5.Lcd.fillScreen(RED);
        else if (state == S_BLACK)
            M5.Lcd.fillScreen(BLACK);
    }
}