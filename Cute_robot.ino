#include <Adafruit_GFX.h>       // Core graphics library
#include <Adafruit_SH1106.h>    // SH1106 OLED driver

#define SCREEN_WIDTH 128        // OLED display width, in pixels
#define SCREEN_HEIGHT 64        // OLED display height, in pixels

// Declaration for SH1106 display connected via I2C
#define OLED_RESET -1           // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SH1106 display(OLED_RESET);

#include <FluxGarage_RoboEyes.h>
roboEyes roboEyes; // create RoboEyes instance

#define ir1 7
#define ir2 8
#define ir3 9
#define touch 10
int smokeA0 = A0;
int led1 = 3;
int led2 = 4;
int led3 = 5;


float sensorValue;

void setup() {
  Serial.begin(9600);

  // Startup OLED Display
  display.begin(SH1106_SWITCHCAPVCC, 0x3C); // Initialize SH1106, address is usually 0x3C
  display.clearDisplay();                   // Clear display buffer
  display.display();                        // Display cleared buffer

  // Setup IR pin
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(ir3, INPUT);
  pinMode(touch, INPUT);
  pinMode (smokeA0,INPUT);
  pinMode (led1,OUTPUT);
  pinMode (led2,OUTPUT);
  pinMode (led3,OUTPUT);

  // Startup robo eyes
  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 100); // screen-width, screen-height, max framerate

  // Define some automated eyes behaviour
  roboEyes.setAutoblinker(ON, 3, 2); // Start auto blinker animation cycle -> bool active, int interval, int variation
  //roboEyes.setIdleMode(ON, 2, 2);    // Start idle animation cycle -> turn on/off, set interval and variation

  // Optional Customization (uncomment to activate):
  // roboEyes.setWidth(36, 36);         // byte leftEye, byte rightEye
  // roboEyes.setHeight(36, 36);        // byte leftEye, byte rightEye
  // roboEyes.setBorderradius(8, 8);    // byte leftEye, byte rightEye
  // roboEyes.setSpacebetween(10);      // int space between eyes
  // roboEyes.setCyclops(ON);           // Use one eye only

  // Set mood, curiosity, and position
  //roboEyes.setMood(TIRED);         // Mood expressions: TIRED, ANGRY, HAPPY, DEFAULT
  // roboEyes.setPosition(DEFAULT);     // Eye direction: N, NE, E, SE, S, SW, W, NW, DEFAULT
  // roboEyes.setCuriosity(ON);         // Eyes stretch vertically on edges

  // Flickering effects (optional)
  // roboEyes.setHFlicker(ON, 2);       // Horizontal flicker with amplitude
  // roboEyes.setVFlicker(ON, 2);       // Vertical flicker with amplitude

  // Prebuilt one-shot animations (optional)
  //roboEyes.anim_confused();          // Eyes shaking left and right
  // roboEyes.anim_laugh();             // Eyes bouncing up and down
}

void playFearAnimation() {
  // Set wide eyes to show fear
  
  roboEyes.setHeight(42, 42);        // make eyes tall
  roboEyes.setPosition(N);           // eyes look up
  roboEyes.setVFlicker(ON, 2);       // vertical jitter
  roboEyes.setMood(TIRED);

  unsigned long startTime = millis();
  while (millis() - startTime < 1000) { // run for 1 second
    roboEyes.update();
  }

  // Reset to default state
  roboEyes.setHeight(36, 36);
  roboEyes.setPosition(DEFAULT);
  roboEyes.setVFlicker(OFF, 0);
}

void loop() {
  int ls= digitalRead(ir1);
  int rs= digitalRead(ir2);
  int ds= digitalRead(ir3);
  int ts= digitalRead(touch);
  sensorValue = analogRead(smokeA0);
  Serial.println( sensorValue);
  if(ds==LOW && ts==LOW && ls==LOW && rs==LOW)
  {
    roboEyes.setWidth(36, 36);
    roboEyes.setHeight(36, 36);
    roboEyes.setAutoblinker(ON, 3, 2);
    roboEyes.setIdleMode(ON, 2, 2);
    roboEyes.setMood(DEFAULT);
  }
  else
  {
    if(ds==LOW)
    {
      if(ts==LOW)
      {

        if(ls==HIGH)
        {
          roboEyes.setAutoblinker(ON, 3, 2);
          roboEyes.setWidth(46, 36);
          roboEyes.setHeight(46, 36);
          roboEyes.setPosition(W);
        }
        if(rs==HIGH)
        {
          roboEyes.setAutoblinker(ON, 3, 2);
          roboEyes.setWidth(36, 46);
          roboEyes.setHeight(36, 46);
          roboEyes.setPosition(E);
        }

        if(ls==HIGH && rs==HIGH)
        {
          roboEyes.setAutoblinker(ON, 3, 2);
          roboEyes.setWidth(36, 36);
          roboEyes.setHeight(36, 36);
          roboEyes.setPosition(DEFAULT);
          roboEyes.setMood(ANGRY);
        }
        if(ls==LOW && rs==LOW)
        {
          roboEyes.setAutoblinker(ON, 3, 2);
          roboEyes.setWidth(36, 36);
          roboEyes.setHeight(36, 36);
          roboEyes.setPosition(DEFAULT);
          roboEyes.setMood(DEFAULT);
          roboEyes.setIdleMode(ON, 2, 2);
        }
      }
      else
      {
        roboEyes.setMood(HAPPY);
        roboEyes.anim_laugh();
      }
    }
    else
    {
      playFearAnimation();
    }
  }
  roboEyes.update(); // Update eyes drawing every frame

  // Don't use delay() here to ensure smooth animation.
  // For animation sequences, see the AnimationSequences example.
 
 if(sensorValue > 250) {
  digitalWrite(led2,HIGH);
  digitalWrite(led1,LOW);
 }
 else{
  digitalWrite(led1,HIGH);
  digitalWrite(led2,LOW);
 }
}
