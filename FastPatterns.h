#ifndef FASTLED_
#define FASTLED_

#include <FastLED.h>

#define PATTERN_COUNT 16
#define INVALID_ID 99999

extern unsigned long currentMillis;

struct LedInfo {
  uint16_t id;
  uint16_t pattern;
  uint8_t brightness;
  CRGB colour;
};

/* class FastPatterns {
 private:
  int baseSpeed = 100;  // Base Speed for patterns in milliseconds.
  int patternSpeed;     // Speed multiplier for the pattern, set this in your
                        // pattern initialiser.
  enum patternIDs {  // The pattern IDs if you add another add it before TWINKLE
    SOLID,
    RAINBOW_CYCLE,
    THEATER_CHASE,
    SCANNER,
    FADE,
    TWINKLE,
    MAXMEMBER
  };

 public:
  bool backwards;
  String patternList[MAXMEMBER] =
      {  // List the names of the available patterns, make sure they stay in the
         // same order as the IDs above
          "Solid Colour",  "Rainbow Cycle", "Theatre Chase",
          "Cylon Scanner", "Colour Fade",   "White Twinkles"};

  uint8_t
      activePattern;  // Index of the active pattern within the pattern table
  uint8_t _pattern_count;  // The total number of patterns available
  int _led_count;          // The number of LEDs
  int interval;            // Calculated interval
  uint8_t userSpeed = 5;   // Divider for the pattern speed, higher is faster.

  unsigned long
      lastUpdate;  // When the last Pattern update occurred in millis()

  CRGB colourA, colourB;  // Which colours are in use for two colour patterns
  uint16_t totalSteps;    // total number of steps in the pattern
  uint16_t currentStep;   // current step within the pattern
  CRGB* _led_array;       // The array to define the LEDs
  CRGBPalette16 currentPalette;  // Current colour palette in use

  // typedef void(*completionCallBack)(void);
  // completionCallBack OnComplete;

  FastPatterns(int led_count);

  // A default completion function to use if you don't write your own.
  void DefaultOnComplete();

  // Update the pattern
  void Update();

  // change to a different pattern by it's number.
  void SwitchPattern(int pID);

  // Increment currentStep, reset at the end and call the completion function,
  // counts either up or down depending on the value of direction.
  void Increment();

  // Reverse Pattern direction
  void Reverse();

  // Initializer and updater for Rainbow Cycle
  void RainbowCycle();
  void RainbowCycleUpdate();

  // Initializer and updater for Theatre Chase
  void TheaterChase();
  void TheaterChaseUpdate();

  // Initializer and updater for Scanner
  void Scanner();
  void ScannerUpdate();

  // Initializer and updater for Fade
  void Fade();
  void FadeUpdate();

  // Initializer and updater for Twinkle
  void Twinkle();
  void TwinkleUpdate();
}; */

class PatternBase {
 protected:
  char* _pattern_name;
  uint16_t _pattern_id;
  uint8_t* _pattern_mask;
  uint8_t* _brightness_array;
  CRGB* _led_array;
  CRGB* _colour_array;
  uint16_t _led_count;
  uint16_t _colour_count;
  uint16_t _base_speed;
  uint16_t _user_speed;
  uint16_t _interval;
  uint16_t _total_steps;
  uint16_t _current_step;
  uint _last_update;
  bool _reverse = false;
  void write_led(uint16_t led, CRGB colour);
  void increment();

 public:
  PatternBase(CRGB* led_array, uint8_t* pattern_mask, uint8_t* brightness_array,
              uint led_count);
  const char* get_name() const { return _pattern_name; }
  uint16_t get_id() { return _pattern_id; }
  virtual void complete(){};
  virtual void update() {}
  void loop();
  void set_speed(uint8_t speed);
};

class PatternManager {
 private:
  PatternBase* _pattern_array[PATTERN_COUNT];
  uint8_t* _pattern_mask;
  uint8_t* _brightness_array;
  uint16_t _led_count;

 public:
  CRGB* _led_array;
  PatternManager(int led_count);
  void loop();
  LedInfo get_led_info(uint16_t led);
  bool set_led_colour(uint16_t led, CRGB colour);
  bool set_led_pattern(uint16_t led, uint16_t pattern);
  bool set_led_brightness(uint16_t led, uint8_t brightness);
  ~PatternManager();
};

class RainbowCycle : public PatternBase {
 private:
 public:
  RainbowCycle(CRGB* led_array, uint8_t* pattern_mask,
               uint8_t* brightness_array, uint led_count);
  void update();
};

class TheatreChase : public PatternBase {
 private:
 public:
  TheatreChase(CRGB* led_array, uint8_t* pattern_mask,
               uint8_t* brightness_array, uint led_count);
  void update();
};

class Scanner : public PatternBase {
 private:
 public:
  Scanner(CRGB* led_array, uint8_t* pattern_mask,
               uint8_t* brightness_array, uint led_count);
  void update();
};

class ColourFade : public PatternBase {
 private:
 public:
  ColourFade(CRGB* led_array, uint8_t* pattern_mask, uint8_t* brightness_array,
          uint led_count);
  void update();
  void complete();
};

class Twinkle : public PatternBase {
 private:
 public:
  Twinkle(CRGB* led_array, uint8_t* pattern_mask, uint8_t* brightness_array,
             uint led_count);
  void update();
};

#endif  // FASTLED_