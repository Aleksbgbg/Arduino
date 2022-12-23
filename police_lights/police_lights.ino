#define ON HIGH
#define OFF LOW

namespace {

static constexpr uint8_t kLights = 4;
static constexpr uint8_t kStartPin = 3;
static constexpr uint8_t kMaxLight = kLights - 1;
static constexpr uint32_t kSwitchDelay = 50;

uint8_t current_light = kMaxLight;

template <typename... T>
using Action = void (*)(T...);

void ForAllLights(Action<uint8_t> action) {
  for (uint8_t light = 0; light < kLights; ++light) {
    action(light);
  }
}

uint8_t LightPin(uint8_t light_index) {
  return kStartPin + light_index;
}

void SetCurrentLight(uint8_t state) {
  digitalWrite(LightPin(current_light), state);
}

uint8_t SwitchToNextLight() {
  current_light = (current_light + 1) % kLights;
}

}  // namespace

void setup() {
  ForAllLights([](uint8_t light) {
    pinMode(LightPin(light), OUTPUT);
  });
}

void loop() {
  SetCurrentLight(OFF);
  SwitchToNextLight();
  SetCurrentLight(ON);
  delay(kSwitchDelay);
}
