#include <TFT_eSPI.h>  // Include the graphics library (this includes the sprite functions)

TFT_eSPI tft = TFT_eSPI();  // Create an instance of the display
TFT_eSprite barSprite = TFT_eSprite(&tft);  // Create an instance of the sprite for the bar
TFT_eSprite textSprite = TFT_eSprite(&tft);  // Create an instance of the sprite for the text

const int potPin = A0;  // Pin connected to the potentiometer
int potValue = 0;       // Variable to store the potentiometer value
const int totalRectangles = 20;  // Total number of rectangles in the bar
const int barHeight = 40;  // Height of each rectangle in the bar
const int barSpacing = 2;  // Spacing between each rectangle in the bar

uint16_t grayColor = TFT_DARKGREY;  // Default gray color for background rectangles, change TFT_DARKGREY for other colors you like.

// Color thresholds and corresponding colors
struct ColorRange {
  int minValue;
  int maxValue;
  uint16_t color;
};

ColorRange colorRanges[] = {
  {0, 50, TFT_GREEN}, // Here change the values and the colors according to the pot reading.
  {50, 100, TFT_YELLOW}, // Here change the values and the colors according to the pot reading.
  {100, 150, TFT_ORANGE}, // Here change the values and the colors according to the pot reading.
  {150, 200, TFT_RED} // Here change the values and the colors according to the pot reading.
};
const int numColorRanges = sizeof(colorRanges) / sizeof(ColorRange);

void setup() {
  Serial.begin(115200);  // Initialize serial communication for debugging

  tft.init();
  tft.setRotation(1);  // Set the screen rotation
  tft.fillScreen(TFT_BLACK);  // Clear the screen with black color
  Serial.println("Display initialized");

  // Create a sprite for the bar (width 200, height 40)
  barSprite.createSprite(200, barHeight);
  barSprite.fillSprite(TFT_BLACK);  // Clear the sprite with black color

  // Create a sprite for the text (width 240, height 40)
  textSprite.createSprite(240, 40);
  textSprite.fillSprite(TFT_BLACK);  // Clear the sprite with black color
  textSprite.setTextSize(2);  // Set the text size
  textSprite.setTextColor(TFT_WHITE, TFT_BLACK);  // Set the text color to white with a black background
}

uint16_t getColorForValue(int value) {
  for (int i = 0; i < numColorRanges; ++i) {
    if (value >= colorRanges[i].minValue && value <= colorRanges[i].maxValue) {
      return colorRanges[i].color;
    }
  }
  return TFT_WHITE;  // Default color if no range is matched
}

void loop() {
  potValue = analogRead(potPin);  // Read the potentiometer value
  int mappedValue = map(potValue, 0, 1023, 0, 200);  // Map the potentiometer value to 0-200

  // Calculate the width of each rectangle
  int rectWidth = (200 - (totalRectangles - 1) * barSpacing) / totalRectangles;

  // Clear the previous bar from the sprite
  barSprite.fillSprite(TFT_BLACK);

  // Draw the gray background rectangles
  int x = 0;
  for (int i = 0; i < totalRectangles; i++) {
    barSprite.fillRect(x, 0, rectWidth, barHeight, grayColor);
    x += rectWidth + barSpacing;
  }

  // Draw the colored rectangles on top
  x = 0;
  for (int i = 0; i < totalRectangles; i++) {
    int segmentValue = (i + 1) * (200 / totalRectangles);

    // Determine the color based on the current position
    uint16_t barColor = getColorForValue(segmentValue);

    // Draw the rectangle if it falls within the mapped value range
    if (mappedValue >= segmentValue) {
      barSprite.fillRect(x, 0, rectWidth, barHeight, barColor);
    }
    x += rectWidth + barSpacing;
  }

  // Push the bar sprite to the screen
  barSprite.pushSprite(10, 100);

  // Clear the previous text from the sprite
  textSprite.fillSprite(TFT_BLACK);

  // Display the numerical reading on the sprite
  textSprite.setCursor(10, 0);
  textSprite.printf("Reading: %d", mappedValue);

  // Push the text sprite to the screen
  textSprite.pushSprite(0, 150);

  delay(100);  // Add a small delay for stability
}