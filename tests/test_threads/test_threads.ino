//#include "application.h"
SYSTEM_MODE(MANUAL);//do not connect to cloud

// LED Param, defines pin and delay between blinks
typedef struct {
    int pin;
    int delay;
} LED_PARAM;

// LED pins
int led1 = D6;
int led2 = D7;
int led3 = D1;

// Defines thread parameters
LED_PARAM ledParams[3] = {
    {led1, 500},
    {led2, 1000},
};

Thread* led1Thread;
Thread* led2Thread;

os_thread_return_t ledBlink(void* param){
    LED_PARAM *p = (LED_PARAM*)param;
    
    // Start never ending loop
    for(;;) {
        // Blink led
        digitalWrite(p->pin, HIGH);
        delay(p->delay);
        digitalWrite(p->pin, LOW);
        delay(p->delay);
    }
}

void setup() {
    Serial.begin(115200);
    
    // Set pin mode
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    
    // Start new threads
    led1Thread = new Thread("sample", ledBlink, &ledParams[0]);
    led2Thread = new Thread("sample", ledBlink, &ledParams[1]);
}

void loop() {
    // Nothing here.
}
