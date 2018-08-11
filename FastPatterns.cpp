#include <FastPatterns.h>

/* FastPatterns::FastPatterns(int led_count) {
  _led_count = led_count;
  _led_array = new CRGB[_led_count];

  _pattern_count = MAXMEMBER;
  activePattern = RAINBOW_CYCLE;
  // direction = FORWARD;
  backwards = false;
}

void FastPatterns::DefaultOnComplete() {
  switch (activePattern) {
    case RAINBOW_CYCLE:
      break;
    case THEATER_CHASE:
      break;
    case SCANNER:
      break;
    case FADE:
      colourA = colourB;
      colourB = CRGB(random8(), random8(), random8());
      break;
    case TWINKLE:
      break;
    default:
      break;
  }
}

void FastPatterns::Update() {
  interval = (baseSpeed * patternSpeed) / userSpeed;
  if ((currentMillis - lastUpdate) > (interval)) {
    lastUpdate = currentMillis;
    switch (activePattern) {
      case RAINBOW_CYCLE:
        RainbowCycleUpdate();
        break;
      case THEATER_CHASE:
        TheaterChaseUpdate();
        break;
      case SCANNER:
        ScannerUpdate();
        break;
      case FADE:
        FadeUpdate();
        break;
      case TWINKLE:
        TwinkleUpdate();
        break;
      default:
        break;
    }
  }
}

void FastPatterns::SwitchPattern(int pID) {
  switch (pID) {
    case RAINBOW_CYCLE:
      RainbowCycle();
      break;
    case THEATER_CHASE:
      TheaterChase();
      break;
    case SCANNER:
      Scanner();
      break;
    case FADE:
      Fade();
      break;
    case TWINKLE:
      Twinkle();
      break;
    default:
      break;
  }
}

void FastPatterns::Increment() {
  if (!backwards) {
    currentStep++;
    if (currentStep >= totalSteps) {
      currentStep = 0;
      DefaultOnComplete();
    }
  } else {
    --currentStep;
    if (currentStep <= 0) {
      currentStep = totalSteps - 1;
      DefaultOnComplete();
    }
  }
}

void FastPatterns::Reverse() {
  if (!backwards) {
    // direction = REVERSE;
    backwards = true;
    currentStep = totalSteps - 1;
  } else {
    // direction = FORWARD;
    backwards = false;
    currentStep = 0;
  }
}

void FastPatterns::RainbowCycle() {
  Serial.println("FastPatterns: Initialising Pattern: Rainbow Cycle");
  activePattern = RAINBOW_CYCLE;
  totalSteps = 255;
  patternSpeed = 1;
  currentStep = 0;
}

void FastPatterns::RainbowCycleUpdate() {
  fill_rainbow(_led_array, _led_count, currentStep, 255 / _led_count);
  Increment();
}

void FastPatterns::TheaterChase() {
  Serial.println("FastPatterns: Initialising Pattern: Theatre Chase");
  colourA = CRGB(random8(), random8(), random8());
  colourB = CRGB(random8(), random8(), random8());
  activePattern = THEATER_CHASE;
  totalSteps = _led_count;
  patternSpeed = 15;
  currentStep = 0;
}

void FastPatterns::TheaterChaseUpdate() {
  for (int i = 0; i < _led_count; i++) {
    if ((i + currentStep) % 3 == 0) {
      _led_array[i] = colourA;
    } else {
      _led_array[i] = colourB;
    }
  }
  Increment();
}

void FastPatterns::Scanner() {
  Serial.println("FastPatterns: Initialising Pattern: Cylon Scanner");
  activePattern = SCANNER;
  patternSpeed = 2;
  totalSteps = (_led_count - 1) * 2;
  colourA = CRGB::Red;
  currentStep = 0;
}

void FastPatterns::ScannerUpdate() {
  for (int i = 0; i < _led_count; i++) {
    if (i == currentStep) {  // Scan Pixel to the right
      _led_array[i] = colourA;
    } else if (i == totalSteps - currentStep) {  // Scan Pixel to the left
      _led_array[i] = colourA;
    } else {  // Fading tail
      _led_array[i].fadeToBlackBy(100);
    }
  }
  Increment();
}

void FastPatterns::Fade() {
  Serial.println("FastPatterns: Initialising Pattern: Colour Fade");
  colourA = CRGB(random8(), random8(), random8());
  colourB = CRGB(random8(), random8(), random8());
  activePattern = FADE;
  patternSpeed = 1;
  totalSteps = 255;
  currentStep = 0;
}

void FastPatterns::FadeUpdate() {
  // Calculate linear interpolation between colourA and colourB
  uint8_t newRed = ((colourA.red * (totalSteps - currentStep)) +
                    (colourB.red * currentStep)) /
                   totalSteps;
  uint8_t newGreen = ((colourA.green * (totalSteps - currentStep)) +
                      (colourB.green * currentStep)) /
                     totalSteps;
  uint8_t newBlue = ((colourA.blue * (totalSteps - currentStep)) +
                     (colourB.blue * currentStep)) /
                    totalSteps;

  fill_solid(_led_array, _led_count, CRGB(newRed, newGreen, newBlue));
  Increment();
}

void FastPatterns::Twinkle() {
  Serial.println("FastPatterns: Initialising Pattern: Twinkle");
  activePattern = TWINKLE;
  patternSpeed = 2;
  totalSteps = 32;
  colourA = CRGB::White;
  currentStep = 0;
}

void FastPatterns::TwinkleUpdate() {
  int chance = random(50);
  if (chance < 6) {
    _led_array[random(_led_count - 1)] = colourA;
  }
  for (int i = 0; i < _led_count; i++) {
    _led_array[i].fadeToBlackBy(10);
  }
  Increment();
}
 */
PatternManager::PatternManager(int led_count) {
  _led_count = led_count;
  _led_array = new CRGB[led_count];
  _pattern_mask = new uint8_t[led_count];
  _brightness_array = new uint8_t[led_count];

  // Add the pattern classes into the managers array, looks ugly but a loop is
  // harder. do stuff
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
  _pattern_name = (char *)"undefined";
  _pattern_id = 0;
  _interval = _base_speed / _user_speed;
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

void PatternBase::loop() {
  uint current_millis = millis();
  if (_last_update + _interval < current_millis) {
    update();
  }
}

void PatternBase::set_speed(uint8_t speed) {
  _user_speed = speed;
  _interval = _base_speed / _user_speed;
}

RainbowCycle::RainbowCycle(CRGB *led_array, uint8_t *pattern_mask,
                           uint8_t *brightness_array, uint led_count)
    : PatternBase(led_array, pattern_mask, brightness_array, led_count) {
  _pattern_name = (char *)"Rainbow Cycle";
  _pattern_id = 1;
  _total_steps = 255;
  _base_speed = 1;
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
  _pattern_name = (char *)"Theatre Chase";
  _pattern_id = 2;
  _total_steps = _led_count;
  _base_speed = 15;
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
  _pattern_name = (char *)"Scanner";
  _pattern_id = 3;
  _total_steps = (_led_count - 1) * 2;
  _base_speed = 2;
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
  _pattern_name = (char *)"Colour Fade";
  _pattern_id = 4;
  _total_steps = 255;
  _base_speed = 1;
  _current_step = 0;
  _colour_count = 2;
  _colour_array = new CRGB[_colour_count];
  _colour_array[0] = CRGB(random8(), random8(), random8());
  _colour_array[1] = CRGB(random8(), random8(), random8());
}

void ColourFade::update() {
  uint8_t new_r = ((_colour_array[0].r * (_total_steps - _current_step)) +
                   (_colour_array[1].r * _current_step)) /
                  _total_steps;
  uint8_t new_g = ((_colour_array[0].g * (_total_steps - _current_step)) +
                   (_colour_array[1].g * _current_step)) /
                  _total_steps;
  uint8_t new_b = ((_colour_array[0].b * (_total_steps - _current_step)) +
                   (_colour_array[1].b * _current_step)) /
                  _total_steps;
  CRGB new_colour = CRGB(new_r, new_g, new_b);
  for (int i; i < _led_count; i++) {
    write_led(i, new_colour);
  }
  increment();
}

void ColourFade::complete() {
  _colour_array[0] = _colour_array[1];
  _colour_array[1] = CRGB(random8(), random8(), random8());
}

Twinkle::Twinkle(CRGB *led_array, uint8_t *pattern_mask,
                 uint8_t *brightness_array, uint led_count)
    : PatternBase(led_array, pattern_mask, brightness_array, led_count) {
  _pattern_name = (char *)"Twinkle Twinkle";
  _pattern_id = 3;
  _total_steps = 32;
  _base_speed = 2;
  _current_step = 0;
  _colour_count = 1;
  _colour_array = new CRGB[_colour_count];
  _colour_array[0] = CRGB::White;
}

void Twinkle::update() {
  int chance = random(50);
  if (chance < 6) {
    write_led(random(_led_count - 1), _colour_array[0]);
  }
  for (int i = 0; i < _led_count; i++) {
    CRGB temp_colour = _led_array[i];
    temp_colour.fadeToBlackBy(10);
    write_led(i, temp_colour);
  }
  increment();
}