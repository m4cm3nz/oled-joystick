#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define JOYSTICK_X A0
#define JOYSTICK_Y A1

void setup() {
  Serial.begin(9600);
  pinMode(JOYSTICK_X, INPUT);
  pinMode(JOYSTICK_Y, INPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.display();
  delay(2000);
  display.clearDisplay();
}

void drawDistortedCircle(int x0, int y0, int radius, int xOffset, int yOffset) {
  for (int i = 0; i < 360; i += 5) {
    float theta = i * 3.14159 / 180;
    float distortionFactor = 1 + ((xOffset * cos(theta)) + (yOffset * sin(theta))) / 50.0;
    int x = x0 + (radius * 1.6 * cos(theta)) * distortionFactor;
    int y = y0 + (radius * sin(theta)) * distortionFactor;
    display.drawPixel(x, y, SSD1306_WHITE);
  }
}


void loop() {
  int xVal = analogRead(JOYSTICK_X);
  int yVal = analogRead(JOYSTICK_Y);

  int xOffset = map(xVal, 0, 1023, -32, 32);
  int yOffset = map(yVal, 0, 1023, 16, -16);

  display.clearDisplay();

  // Desenhar círculos distorcidos (curvas de nível) com ajustes proporcionais
  drawDistortedCircle(SCREEN_WIDTH / 2 + xOffset / 2, SCREEN_HEIGHT / 2 + yOffset / 2, 8, xOffset, yOffset);
  drawDistortedCircle(SCREEN_WIDTH / 2 + xOffset / 4, SCREEN_HEIGHT / 2 + yOffset / 4, 16, xOffset / 1.6, yOffset / 1.6);
  drawDistortedCircle(SCREEN_WIDTH / 2 + xOffset / 6, SCREEN_HEIGHT / 2 + yOffset / 6, 24, xOffset / 3.2, yOffset / 3.2);
  drawDistortedCircle(SCREEN_WIDTH / 2 + xOffset / 8, SCREEN_HEIGHT / 2 + yOffset / 8, 32, xOffset / 6.4, yOffset / 6.4);

  display.display();
  delay(150);
}

