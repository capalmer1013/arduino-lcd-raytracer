
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void setup(void) {
  randomSeed(analogRead(3));
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));
  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  main_f();
}

void main_f(){
  const int image_width=tft.width();
  const int image_height=tft.height();

  for(int j=image_height-1; j>=0; --j){
    for(int i=0; i< image_width; ++i){
      double r = double(i)/(image_width-1);
      double g = double(j)/(image_height-1);
      double b = 0.25;
      unsigned int color = rgb_to_565(r, g, b);
      //Serial.println(color);
      tft.drawPixel(i, j,  color);
    }
  }
}


unsigned int rgb_to_565(double r, double g, double b){
  // |r|r|r|r|r|g|g|g|g|g|g|b|b|b|b|b|
  //  f e d c b a 9 8 7 6 5 4 3 2 1 0
  
  unsigned int ir = (unsigned int)(r*32) << 11;
  unsigned int ig = (unsigned int)(g*64) <<5;
  unsigned int ib = (b*32);
  return ir | ig | ib;
}



void loop(void) {
}
