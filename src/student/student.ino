
// =====================
// Include Libraries.
// =====================
// Exercise 2: Connect to wifi.

// Exercise 3: Display and NTP time.

// Exercise 4: Temp/Humidity Sensor

// Exercise 5: Adafruit


// =====================
// Variables/Constants
// =====================
// Exercise 2: Connect to wifi.

// Exercise 3: OLED display and NTP time.

// Exercise 4: Temp/Humidity Sensor

// Exercise 5: Adafruit


// =====================
// Functions
// =====================

void connectToAdafruit() 
{
  // Exercise 5: Connect to the Adafruit IO platform.
}

void setup()
{
  // Exercise 1: Blink the on-board LED.

  // Exercise 2: Connect to wifi.

  // Exercise 3: OLED display

  // Exercise 3: NTP Time

  // Exercise 4: Temp/Humidity Sensor

  // Exercise 5: Adafruit
}

void LedTask()
{
  // Exercise 1: Blink the LED. 
}

void DisplayTask()
{
  // Exercise 3: Refresh the time of day.

  // Exercise 4: Refresh the temp/humidity.
}


void SensorTask()
{
  // Exercise 4: Read the temp/humidity every 2 seconds.
}

void CloudTask()
{
  // Exercise 5: Send temp/humidity to the Adafruit IO cloud.
}

typedef struct
{
  unsigned long previousMillis;
  unsigned long elapsedMillis;
  unsigned long timeoutMillis;
  void (*callback)();
} Timer_t;

// =====================
// Scheduler Table
// =====================
static Timer_t schedulerTable[] = 
{
  {0, 0, 500, &LedTask},  // Exercise 1: Blink LED
  // Add tasks here
};

void runScheduler()
{ 
  // Run each timer in the scheduler table, and call 
  for (int i = 0; i < sizeof(schedulerTable)/sizeof(Timer_t); i++)
  {
    // Note: millis() will overflow after ~50 days.  
    unsigned long currentMillis = millis();
    Timer_t *t = &schedulerTable[i];    
    t->elapsedMillis += currentMillis - t->previousMillis;
    t->previousMillis = currentMillis;
    if (t->elapsedMillis >= t->timeoutMillis)
    {
      t->elapsedMillis = 0;
      t->callback();
    }
  }
}

void loop()
{  
  runScheduler();
}
