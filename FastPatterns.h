#ifndef FASTLED_
#define FASTLED_

#include <FastLED.h>

#define PATTERN_COUNT 5
#define INVALID_ID 99999

struct LedInfo {
  uint16_t id;
  uint8_t pattern;
  uint8_t brightness;
  CRGB colour;
};

class PatternBase {
 protected:
  String _pattern_name;
  uint16_t _pattern_id;
  uint8_t* _pattern_mask;
  uint8_t* _brightness_array;
  CRGB* _led_array;
  CRGB* _colour_array;
  uint16_t _led_count;
  uint16_t _colour_count = 1;
  uint16_t _base_speed = 1;
  float _user_speed = 1.00;
  uint16_t _interval = 1;
  uint16_t _total_steps = 255;
  uint16_t _current_step = 0;
  uint _last_update;
  bool _reverse = false;
  void write_led(uint16_t led, CRGB colour);
  void set_base_speed(uint8_t speed);
  void increment();

 public:
  PatternBase(CRGB* led_array, uint8_t* pattern_mask, uint8_t* brightness_array,
              uint led_count);
  String get_name() { return _pattern_name; }
  uint16_t get_id() { return _pattern_id; }
  virtual void complete(){}
  virtual void update() {}
  void loop();
  bool set_user_speed(uint8_t speed);
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
  uint16_t get_led_count(){ return _led_count;}
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
  Scanner(CRGB* led_array, uint8_t* pattern_mask, uint8_t* brightness_array,
          uint led_count);
  void update();
};

class ColourFade : public PatternBase {
 private:
  CRGBPalette256 _gradient;
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