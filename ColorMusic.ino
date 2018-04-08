/*
   Управление лентой на WS2812 с компьютера
   Оргинал AlexGyver http://alexgyver.ru/
   2017
*/
//----------------------НАСТРОЙКИ-----------------------
#define NUM_LEDS 98          // число светодиодов в ленте
#define DI_PIN 12            // пин, к которому подключена лента

#define start_flashes 1      // мигание при запуске

#define serialRate 115200                                 // скорость передачи данных через COM порт
uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;       // кодовое слово Ada
#include <FastLED.h>                                      // подключаем библиотеку
CRGB leds[NUM_LEDS];                                      // инициализируем ленту

void setup()
{
  FastLED.addLeds<WS2812, DI_PIN, GRB>(leds, NUM_LEDS);
  if (start_flashes) {
    LEDS.showColor(CRGB(255, 0, 0));
    delay(500);
    LEDS.showColor(CRGB(0, 255, 0));
    delay(500);
    LEDS.showColor(CRGB(0, 0, 255));
    delay(500);
    LEDS.showColor(CRGB(0, 0, 0));
  }

  Serial.begin(serialRate);
  Serial.print("Ada\n");     // Связаться с компом
}

void loop() 
{
  for (i = 0; i < sizeof prefix; ++i) {
waitLoop: while (!Serial.available()) ;;
    if (prefix[i] == Serial.read()) continue;
    i = 0;
    goto waitLoop;
  }

  while (!Serial.available()) ;;
  hi = Serial.read();
  while (!Serial.available()) ;;
  lo = Serial.read();
  while (!Serial.available()) ;;
  chk = Serial.read();
  if (chk != (hi ^ lo ^ 0x55))
  {
    i = 0;
    goto waitLoop;
  }

  memset(leds, 0, NUM_LEDS * sizeof(struct CRGB));
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    byte r, g, b;
    // читаем данные для каждого цвета
    while (!Serial.available());
    r = Serial.read();
    while (!Serial.available());
    g = Serial.read();
    while (!Serial.available());
    b = Serial.read();
    leds[i].r = r;
    leds[i].g = g;
    leds[i].b = b;
  }
  FastLED.show();  // записываем цвета в ленту
}
