namespace {

static constexpr uint32_t kSwitchDelay = 250;

struct Led {
  uint8_t pin;
  uint8_t state;
};

Led Leds[] = {
    {.pin = 3, .state = HIGH},
    {.pin = 4, .state = LOW},
};

}  // namespace

void setup() {
  for (const Led& led : Leds) {
    pinMode(led.pin, OUTPUT);
  }
}

void loop() {
  for (Led& led : Leds) {
    led.state = !led.state;
    digitalWrite(led.pin, led.state);
  }
  delay(kSwitchDelay);
}
