#include <FastPatterns.h>

PatternManager::PatternManager(int led_count) : _led_count(led_count) {
  Serial.println("Initialising Pattern manager.");
  _led_array = new CRGB[led_count];
  _pattern_mask = new uint8_t[led_count];
  _brightness_array = new uint8_t[led_count];

  // Add the pattern classes into the array.
  _pattern_array[0] = new RainbowCycle(_led_array, _pattern_mask,
                                       _brightness_array, _led_count);
  _pattern_array[1] = new TheatreChase(_led_array, _pattern_mask,
                                       _brightness_array, _led_count);
  _pattern_array[2] =
      new Scanner(_led_array, _pattern_mask, _brightness_array, _led_count);
  _pattern_array[3] =
      new ColourFade(_led_array, _pattern_mask, _brightness_array, _led_count);
  _pattern_array[4] =
      new Twinkle(_led_array, _pattern_mask, _brightness_array, _led_count);
  Serial.println("Pattern Manager initialised..");
}

void PatternManager::loop() {
  for (int i = 0; i < PATTERN_COUNT; i++) {
    _pattern_array[i]->loop();
  }
}

LedInfo PatternManager::get_led_info(uint16_t led) {
  LedInfo led_info;
  if (led < _led_count) {
    led_info.id = led;
    led_info.pattern = _pattern_mask[led];
    led_info.colour = _led_array[led];
    led_info.brightness = _brightness_array[led];
  } else {
    led_info.id = INVALID_ID;
    led_info.pattern = 0;
    led_info.colour = CRGB::Black;
    led_info.brightness = 0;
  }
  return led_info;
}

bool PatternManager::set_led_colour(uint16_t led, CRGB colour) {
  if (led < _led_count) {
    _led_array[led] = colour;
    _pattern_mask[led] = 0;
    uint8_t inverse_brightness = 256 - _brightness_array[led];
    _led_array[led].fadeLightBy(inverse_brightness);
    return true;
  } else {
    return false;
  }
}

bool PatternManager::set_led_pattern(uint16_t led, uint16_t pattern) {
  if (led < _led_count) {
    _pattern_mask[led] = pattern;
    return true;
  } else {
    return false;
  }
}

bool PatternManager::set_led_brightness(uint16_t led, uint8_t brightness) {
  if (led < _led_count && 0 <= brightness <= 256) {
    _pattern_mask[led] = brightness;
    return true;
  } else {
    return false;
  }
}

PatternManager::~PatternManager() {}

PatternBase::PatternBase(CRGB *led_array, uint8_t *pattern_mask,
                         uint8_t *brightness_array, uint led_count)
    : _pattern_mask(pattern_mask),
      _led_array(led_array),
      _brightness_array(brightness_array),
      _led_count(led_count) {
  _pattern_name = F("undefined");
  _pattern_id = 0;
  _last_update = millis();
}

void PatternBase::increment() {
  if (!_reverse) {
    _current_step++;
    if (_current_step >= _total_steps) {
      _current_step = 0;
      complete();
    }
  } else {
    --_current_step;
    if (_current_step <= 0) {
      _current_step = _total_steps - 1;
      complete();
    }
  }
}

void PatternBase::write_led(uint16_t led, CRGB colour) {
  if (_pattern_mask[led] == _pattern_id) {
    _led_array[led] = colour;
    uint8_t inverse_brightness = 256 - _brightness_array[led];
    _led_array[led].fadeLightBy(inverse_brightness);
  }
}

void PatternBase::set_base_speed(uint8_t speed) {
  _base_speed = speed;
  _interval = _base_speed * _user_speed;
}

void PatternBase::loop() {
  unsigned int current_millis = millis();
  if (current_millis > _last_update + _interval) {
    update();
    _last_update = current_millis;
  }
}

bool PatternBase::set_user_speed(uint8_t speed) {
  if (1<=speed<=100){
    _user_speed = speed / 50;
    _interval = _base_speed * _user_speed;
    return true;
  } else {
    return false;
  } 
}

RainbowCycle::RainbowCycle(CRGB *led_array, uint8_t *pattern_mask,
                           uint8_t *brightness_array, uint led_count)
    : PatternBase(led_array, pattern_mask, brightness_array, led_count) {
  _pattern_name = F("Rainbow Cycle");
  _pattern_id = 1;
  _total_steps = 255;
  set_base_speed(30);
  _current_step = 0;
  _colour_count = 1;
  _colour_array = new CRGB[_colour_count];
  _colour_array[0] = CRGB::Black;
}

void RainbowCycle::update() {
  CRGB temp_array[_led_count];
  fill_rainbow(temp_array, _led_count, _current_step, 255 / _led_count);
  for (int i = 0; i < _led_count; i++) {
    write_led(i, temp_array[i]);
  }
  increment();
}

TheatreChase::TheatreChase(CRGB *led_array, uint8_t *pattern_mask,
                           uint8_t *brightness_array, uint led_count)
    : PatternBase(led_array, pattern_mask, brightness_array, led_count) {
  _pattern_name = F("Theatre Chase");
  _pattern_id = 2;
  _total_steps = _led_count;
  set_base_speed(175);
  _current_step = 0;
  _colour_count = 2;
  _colour_array = new CRGB[_colour_count];
  _colour_array[0] = CRGB(random8(), random8(), random8());
  _colour_array[1] = CRGB(random8(), random8(), random8());
}

void TheatreChase::update() {
  for (int i = 0; i < _led_count; i++) {
    if ((i + _current_step) % 3 == 0) {
      write_led(i, _colour_array[0]);
    } else {
      write_led(i, _colour_array[1]);
    }
  }
  increment();
}

Scanner::Scanner(CRGB *led_array, uint8_t *pattern_mask,
                 uint8_t *brightness_array, uint led_count)
    : PatternBase(led_array, pattern_mask, brightness_array, led_count) {
  _pattern_name = F("Scanner");
  _pattern_id = 3;
  _total_steps = (_led_count - 1) * 2;
  set_base_speed(40);
  _current_step = 0;
  _colour_count = 1;
  _colour_array = new CRGB[_colour_count];
  _colour_array[0] = CRGB::Red;
}

void Scanner::update() {
  for (int i = 0; i < _led_count; i++) {
    if ((i == _current_step) ||
        (i == _total_steps - _current_step)) {  // Scan Pixel to the right
      write_led(i, _colour_array[0]);
    } else {  // Fading tail
      CRGB temp_colour = _led_array[i];
      temp_colour.fadeToBlackBy(100);
      write_led(i, temp_colour);
    }
  }
  increment();
}

ColourFade::ColourFade(CRGB *led_array, uint8_t *pattern_mask,
                       uint8_t *brightness_array, uint led_count)
    : PatternBase(led_array, pattern_mask, brightness_array, led_count) {
  _pattern_name = F("Colour Fade");
  _pattern_id = 4;
  _total_steps = 255;
  set_base_speed(15);
  _current_step = 0;
  _colour_count = 2;
  _colour_array = new CRGB[_colour_count];
  _colour_array[0] = CRGB(random8(), random8(), random8());
  _colour_array[1] = CRGB(random8(), random8(), random8());
  _gradient = CRGBPalette256(_colour_array[0],_colour_array[1]);
}

void ColourFade::update() {
  CRGB new_colour = ColorFromPalette(_gradient, _current_step);
  for (int i = 0; i < _led_count; i++) {
    write_led(i, new_colour);
  }
  increment();
}

void ColourFade::complete() {
  _colour_array[0] = _colour_array[1];
  _colour_array[1] = CRGB(random8(), random8(), random8());
  _gradient = CRGBPalette256(_colour_array[0], _colour_array[1]);
}

Twinkle::Twinkle(CRGB *led_array, uint8_t *pattern_mask,
                 uint8_t *brightness_array, uint led_count)
    : PatternBase(led_array, pattern_mask, brightness_array, led_count) {
  _pattern_name = F("Twinkle Twinkle");
  _pattern_id = 5;
  _total_steps = 32;
  set_base_speed(50);
  _current_step = 0;
  _colour_count = 1;
  _colour_array = new CRGB[_colour_count];
  _colour_array[0] = CRGB::White;
}

void Twinkle::update() {
  int chance = random(50);
  if (chance < 10) {
    write_led(random(_led_count - 1), _colour_array[0]);
  }
  for (int i = 0; i < _led_count; i++) {
    CRGB temp_colour = _led_array[i];
    temp_colour.fadeToBlackBy(10);
    write_led(i, temp_colour);
  }
  increment();
}