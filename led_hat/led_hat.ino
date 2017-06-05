#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Flora is connected to the NeoPixels?
#define PIN            6

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(64, PIN, NEO_GRB + NEO_KHZ800);

// Shapes
byte smile[] = {B00000000, B00000000, B01111110, B01000010, B00000000, B00100100, B00100100, B00000000};
byte hhLogo[] = {B00000000, B01010000, B01111010, B01011110, B10001010, B01000001, B00110010, B00001100};
byte diamond[] = {B00000000, B00011000, B00111100, B01111110, B01111110, B00111100, B00011000, B00000000};
byte hacksterLogo[] = {B00000000, B00100100, B00100100, B01110100, B10111101, B00101110, B00100100, B00100100};
byte subscribe[] = {B00000000, B01110000, B00100100, B00101110, B00100100, B00110000, B00100000, B00000000};

void setup() {
  pixels.begin();
  pixels.setBrightness(45); //To reduce power consumption
}

void clear() {
  for(int i=0;i<64;i++){
    pixels.setPixelColor(i, pixels.Color(0,0,0));
  }
}

void setPixelForColor(int row, int col, uint32_t color) {
  if ((row % 2) == 1){
    pixels.setPixelColor(row*8+(7-col), color); 
  } else {
    pixels.setPixelColor(row*8+col, color); 
  }
}

void rain(int cycles, int delayTime) {
  int rain[] = {0,0,0,0,0,0,0,0};
  int values[] = {0,0,0,0,0,0,0,0};

  for (int v=0; v<8; v++) {
    values[v] = random(0, 10) * 10;
  }

  for (int c=0; c<cycles; c++) {
    for (int f=0; f<10; f++) {
      for (int col = 0; col < 8; col++) {
        int row = map(values[col], 0, 100, 0, 8);
        if ((row % 2) == 1) {
          pixels.setPixelColor((64-row*8)+(7-col), pixels.Color(200,200,200));
        } else {
          pixels.setPixelColor((64-row*8)+col, pixels.Color(200,200,200));
        }
      }
      
      pixels.show();
      delay(delayTime);
      
      for (int v=0; v<8; v++) {
        values[v] = (values[v] + 10) % 100;
      }
      
      clear();
      delay(100);
     }
   }
}

void sineWave(int cycles, int delayLength) {
  int wave[] = {0,0,0,0,0,0,0,0};
   int values[] = {0,0,0,0,0,0,0,0};

   for (int c = 0; c < cycles; c++) {
    for (int p = 0; p < 360/10; p++) {
       wave[0] = (wave[0] + 10) % 360;
       values[0] = int(50.0*(sin(1*(wave[0]*PI/180)) + 1));
    
       for(int i=1; i<8; i++) {
        wave[i] = (wave[i-1] + int(360.0/8.0)) % 360;
        values[i] = int(50.0*(sin(1*(wave[i]*PI/180)) + 1));
       }
      
      clear();
      
      //Draw the audio levels
      for (int col = 0; col < 8; col++) {
        for(int row = 0; row < map(values[col], 0, 100, 2, 8); row++){
          if ((row % 2) == 1) {
            pixels.setPixelColor(row*8+(7-col), pixels.Color(row*30,  96 - 10*row, 0));
          } else {
            pixels.setPixelColor(row*8+col, pixels.Color(row*30,  96 - 12*row, 0));
          }
          
        }
      }
      
      pixels.show();
      delay(delayLength);
    }
   }

   clear();
   pixels.show();
}

void audioLevels(int cycles, int delayLength) {
  int wave[] = {0,0,0,0};
   int values[] = {0,0,0,0};

   for (int c = 0; c < cycles; c++) {
    for (int p = 0; p < 360/10; p++) {
       wave[0] = (wave[0] + 10) % 360;
       values[0] = int(50.0*(sin(1*(wave[0]*PI/180)) + 1));
    
       for(int i=1; i<4; i++) {
        wave[i] = (wave[i-1] + 102) % 360;
        values[i] = int(50.0*(sin(1*(wave[i]*PI/180)) + 1));
       }
      
      clear();
      
      //Draw the audio levels
      for (int col = 0; col < 4; col++) {
        for(int row = 0; row < map(values[col], 0, 100, 2, 8); row++){
          if ((row % 2) == 1) {
            pixels.setPixelColor(row*8+(7-2*col), pixels.Color(0,  row*30, 96 - 10*row));
            pixels.setPixelColor(row*8+(7-2*col-1), pixels.Color(0,  row*30, 96 - 10*row));
          } else {
            pixels.setPixelColor(row*8+2*col, pixels.Color(0,  row*30, 96 - 12*row));
            pixels.setPixelColor(row*8+2*col+1, pixels.Color(0, row*30,  96 - 12*row));
          }
          
        }
      }
      
      pixels.show();
      delay(delayLength);
    }
   }

   clear();
   pixels.show();
}

void drawShape(byte shape[], uint32_t color, boolean flash, int cycles, int delayTime) {
  for(int c = 0; c < cycles; c++) {
    for(int i=0;i<8;i++) {
      for(int j=0; j<8; j++) {
        if(bitRead(shape[i], j) == 1) {
          setPixelForColor(i, j, color);
        }
      }
    }
  
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayTime); // Delay for a period of time (in milliseconds).
  
    if (flash) {
      clear();
      pixels.show();
      delay(delayTime); // Delay for a period of time (in milliseconds).
    }
  }

  clear();
  pixels.show();
}

void animate(byte shape[][8], int frames, uint32_t color, int cycles, int delayTime) {
  for (int c=0; c<cycles; c++) {
    for (int s=0; s<frames; s++) {
      drawShape(shape[s], color, false, 1, delayTime);
    }
  }

  clear();
  pixels.show();
}

void rainbowAnimation(byte shape[][8], int frames, int cycles, int delayTime) {
  for (int c=0; c<cycles; c++) {
    for (int r=0; r<256; r+=35) {
      for (int s=0; s<frames; s++) {
        drawShape(shape[s], Wheel(r), false, 1, delayTime);
      }
    }
  }

  clear();
  pixels.show();
}

void tunnelAnimation(uint32_t color, boolean rainbow, int cycles, int delayTime) {
  byte animation[][8] = {{B00000000, B00000000, B00000000, B00011000, B00011000, B00000000, B00000000, B00000000},
  {B00000000, B00000000, B00111100, B00100100, B00100100, B00111100, B00000000, B00000000},
  {B00000000, B01111110, B01000010, B01000010, B01000010, B01000010, B01111110, B00000000},
  {B11111111, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B11111111}};

  if (rainbow) {
    rainbowAnimation(animation, 4, cycles, delayTime);
  } else {
    animate(animation, 4, color, cycles, delayTime); 
  }
}

void twoColor(byte shape[], uint32_t color1, uint32_t color2, int cycles, int delayTime) {
  for(int c = 0; c < cycles; c++) {
    for(int i=0;i<8;i++){
      for(int j=0; j<8; j++) {
        if(bitRead(shape[i], j) == 1){
          setPixelForColor(i, j, color1);
        } else {
          setPixelForColor(i, j, color2);
        }
      }
    }
  
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayTime); // Delay for a period of time (in milliseconds).
  }
  
  clear();
  pixels.show();
}

void rainbowShape(byte shape[], uint8_t wait){
  for(int r=0; r<256; r++) {
    for(int i=0;i<8;i++){
      for(int j=0; j<8; j++) {
        if(bitRead(shape[i], j) == 1){
          setPixelForColor(i, j, Wheel((i+j+r) & 255));
        }
      }
    }
    
    pixels.show();
    delay(wait);
  }

  clear();
  pixels.show();
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<64; i++) {
      pixels.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixels.show();
    delay(wait);
  }
  clear();
  pixels.show();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void loop() {
  rain(5, 20);
  tunnelAnimation(pixels.Color(0,0,255), true, 5, 50);
  rainbowShape(diamond, 20);
  sineWave(5, 25);
  audioLevels(5, 25);
  drawShape(smile, pixels.Color(255,227,0), true, 10, 500);
  rainbow(20);

  /*drawShape(hacksterLogo, pixels.Color(28,172,247), false, 30, 500);
  drawShape(subscribe, pixels.Color(255,227,0), false, 30, 500);
  drawShape(hhLogo, pixels.Color(0,0,255), false, 20, 500);*/
}
