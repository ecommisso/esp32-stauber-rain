//Elizabeth Commisso
//Creative Embedded Systems Module 1

#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

const char* text[] = {
  "See, the rain is nice but",
  "I-I don't really like getting wet",
  "Wha-",
  "What in the world",
  "No I-",
  "I went under the awning",
  "It's-it's still raining",
  "Why is it still raining",
  "I did what I was supposed to",
  "That's not fair"
};

const int numLines = 10;
const int increment = 10; //how much the rain lines grow

struct Line {
  int x1;
  int y1;
  int length;
};

Line lines[numLines];

void setup() {
  tft.init();
  tft.setRotation(1); // horizontal
  tft.fillScreen(TFT_BLACK); // clear the screen

  //set screen Back Light brightness
  pinMode(TFT_BL, OUTPUT);
  ledcSetup(0, 5000, 8); // 0-15, 5000, 8
  ledcAttachPin(TFT_BL, 0); // TFT_BL, 0 - 15
  ledcWrite(0, 150); // 0-15, 0-255 (with 8 bit resolution); 0=totally dark;255=totally shiny
  // CHANGED TO BE BRIGHTER WHEN TESTING BEFORE INSTALLATION
  // WAS HARD TO SEE AT 50 in the lighting

  initLines(); // Initialize lines
}

void loop() {
  moveLines(); 
  drawText(); 
  delay(20); 
}

//initialize the position of each line
void initLine(int i){
  lines[i].x1 = random(-240, 240); // random X position within screen width
  lines[i].y1 = 0;
  lines[i].length = tft.height();
}

void initLines() {
  for (int i = 0; i < numLines; i++) {
    initLine(i);
  }
}

void moveLines() {
  for (int i = 0; i < numLines; i++) {
    Line &line = lines[i];
    
    //make the line longer until we reach the height, i.e. bottom of the screen
    if (line.y1 < tft.height()) {
      line.length += increment;
    } else {
      initLine(i); //make a new line, starting at a random x
    }
    
    //increment the positions so they move in a diagonal
    int x2 = line.x1 + line.length;
    int y2 = line.y1 + line.length;

    //draw the line then clear parts of it so it appears as if it's raining quickly 
    //the "dotted" effect of rain falling hard in the wind
    tft.drawLine(line.x1, line.y1, x2, y2, TFT_BLUE); 
    tft.drawLine(line.x1, line.y1, x2, y2 - increment, TFT_BLACK); // clear line
  }
}

void drawText() {
  static int currentLine = 0;
  static int textDelay = 0; 

  // clear the screen
  tft.fillScreen(TFT_BLACK);

  // display the current line at the updated position
  int16_t textWidth = tft.textWidth(text[currentLine]);
  int16_t xPos = (tft.width() - textWidth) / 2;

  tft.setCursor(xPos, currentLine * 10); // change y pos so we can move the text downward on each line
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1.5);
  tft.println(text[currentLine]); //print 1 line time at a time
  
  // move to the next line with a longer delay
  if (textDelay >= 50) { // change value to change speed of falling
    currentLine = (currentLine + 1) % numLines;
    textDelay = 0; // reset delay
  } else {
    textDelay++;
  }
}
