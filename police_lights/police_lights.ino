#define ON HIGH
#define OFF LOW

namespace {

template <typename TResult, typename... T>
using Func = TResult (*)(T...);

template <typename... T>
using Action = Func<void, T...>;

using SwitchLightAlgorithm = Func<uint8_t, uint8_t>;

static constexpr uint8_t kLights = 4;
static constexpr uint8_t kStartPin = 3;
static constexpr uint32_t kSwitchDelay = 250;

namespace select {

uint8_t Next(uint8_t current) {
  return current + 1;
}

uint8_t Previous(uint8_t current) {
  return current - 1;
}

uint8_t Random(uint8_t current) {
  return random(0, kLights);
}

}  // namespace select

static constexpr uint8_t kChangeAlgorithmPin = 2;
static constexpr auto kEnableChangeAlgorithm = HIGH;
static constexpr uint8_t kAlgorithmCount = 3;

static constexpr SwitchLightAlgorithm kAlgorithms[kAlgorithmCount] = {
    select::Next,
    select::Previous,
    select::Random,
};

uint8_t current_light = 0;
uint8_t current_algorithm = 0;

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

template <typename T>
void Switch(T& value, Func<T, T> switcher, T modulus) {
  value = switcher(value) % modulus;
}

bool ShouldChangeAlgorithm() {
  return digitalRead(kChangeAlgorithmPin) == kEnableChangeAlgorithm;
}

void ChangeAlgorithm() {
  Switch(current_algorithm, select::Next, kAlgorithmCount);
}

SwitchLightAlgorithm CurrentAlgorithm() {
  return kAlgorithms[current_algorithm];
}

void SwitchLight() {
  Switch(current_light, CurrentAlgorithm(), kLights);
}

}  // namespace

void setup() {
  ForAllLights([](uint8_t light) {
    pinMode(LightPin(light), OUTPUT);
  });

  pinMode(kChangeAlgorithmPin, INPUT);
}

void loop() {
  if (ShouldChangeAlgorithm()) {
    ChangeAlgorithm();
  }

  SetCurrentLight(OFF);
  SwitchLight();
  SetCurrentLight(ON);
  delay(kSwitchDelay);
}
