#include <SoftwareSerial.h>
SoftwareSerial bt(12, 11); //RX,TX

#define redPin 5
#define greenPin 3
#define bluePin 6
#define Loop_Delay 20

String incomingData;
String redTempValue;
String greenTempValue;
String blueTempValue;
char currentColor;

int Brightness_R;
int Brightness_G;
int Brightness_B;

int FadeStep_R;
int FadeStep_G;
int FadeStep_B;

int counter = 0;
int numColors = 255;
int animationDelay = 25; // number milliseconds before RGB LED changes to next color

void setup()
{
  Serial.begin(115200);
  Serial.println("-= HC-05 Bluetooth RGB LED =-");
  bt.begin(9600);
  bt.println("-= HC-05 Bluetooth RGB LED =-");

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Brightness_R = 0;
  Brightness_G = 0;
  Brightness_B = 0;

  FadeStep_R = 0;      
  FadeStep_G = 325;    
  FadeStep_B = 650;    

  setColor(255, 255, 255);
}

void rainbowPattern() {
  float colorNumber = counter > numColors ? counter - numColors : counter;

  float saturation = 1; // Between 0 and 1 (0 = gray, 1 = full color)
  float brightness = 1; // Between 0 and 1 (0 = dark, 1 is full brightness)
  float hue = (colorNumber / float(numColors)) * 360; // Number between 0 and 360
  long color = HSBtoRGB(hue, saturation, brightness);

  int red = color >> 16 & 255;
  int green = color >> 8 & 255;
  int blue = color & 255;

  setColor(red, green, blue);

  counter = (counter + 1) % (numColors * 2);

  delay(animationDelay);
}

void setColor (unsigned char red, unsigned char green, unsigned char blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

long HSBtoRGB(float _hue, float _sat, float _brightness) {
  float red = 0.0;
  float green = 0.0;
  float blue = 0.0;

  if (_sat == 0.0) {
    red = _brightness;
    green = _brightness;
    blue = _brightness;
  } else {
    if (_hue == 360.0) {
      _hue = 0;
    }

    int slice = _hue / 60.0;
    float hue_frac = (_hue / 60.0) - slice;

    float aa = _brightness * (1.0 - _sat);
    float bb = _brightness * (1.0 - _sat * hue_frac);
    float cc = _brightness * (1.0 - _sat * (1.0 - hue_frac));

    switch (slice) {
      case 0:
        red = _brightness;
        green = cc;
        blue = aa;
        break;
      case 1:
        red = bb;
        green = _brightness;
        blue = aa;
        break;
      case 2:
        red = aa;
        green = _brightness;
        blue = cc;
        break;
      case 3:
        red = aa;
        green = bb;
        blue = _brightness;
        break;
      case 4:
        red = cc;
        green = aa;
        blue = _brightness;
        break;
      case 5:
        red = _brightness;
        green = aa;
        blue = bb;
        break;
      default:
        red = 0.0;
        green = 0.0;
        blue = 0.0;
        break;
    }
  }

  long ired = red * 255.0;
  long igreen = green * 255.0;
  long iblue = blue * 255.0;

  return long((ired << 16) | (igreen << 8) | (iblue));
}

void fadePattern() {

  FadeStep_R = FadeStep_R - 1;
  FadeStep_G = FadeStep_G - 1;
  FadeStep_B = FadeStep_B - 1;

  //fade red LED according to it's fade step counter.
  if (FadeStep_R == -764) {
    FadeStep_R = 765;
  }
  if (FadeStep_R < 0) {
    Brightness_R = FadeStep_R * -1;
  }
  if (FadeStep_R >= 510) {
    Brightness_R = FadeStep_R - 510;
  }

  //fade green LED according to it's fade step counter.
  if (FadeStep_G == -764) {
    FadeStep_G = 765;
  }
  if (FadeStep_G < 0) {
    Brightness_G = FadeStep_G * -1;
  }
  if (FadeStep_G >= 510) {
    Brightness_G = FadeStep_G - 510;
  }

  //fade blue LED according to it's fade step counter.
  if (FadeStep_B == -764) {
    FadeStep_B = 765;
  }
  if (FadeStep_B < 0) {
    Brightness_B = FadeStep_B * -1;
  }
  if (FadeStep_B >= 510) {
    Brightness_B = FadeStep_B - 510;
  }

  Brightness_B = constrain(Brightness_B, 0, 255);
  Brightness_G = constrain(Brightness_G, 0, 255);
  Brightness_R = constrain(Brightness_R, 0, 255);

  setColor(Brightness_R, Brightness_G, Brightness_B);
  delay (Loop_Delay);
}

void loop()
{
  //  while (bt.available() > 0)
  //  {
  //    int redInt = bt.parseInt();
  //    int greenInt = bt.parseInt();
  //    int blueInt = bt.parseInt();
  //
  //    redInt = constrain(redInt, 0, 255);
  //    greenInt = constrain(greenInt, 0, 255);
  //    blueInt = constrain(blueInt, 0, 255);
  //
  //    if (bt.available() > 0)
  //    {
  //      setColor(redInt, greenInt, blueInt);
  //      Serial.print("Red: ");
  //      Serial.print(redInt);
  //      Serial.print(" Green: ");
  //      Serial.print(greenInt);
  //      Serial.print(" Blue: ");
  //      Serial.print(blueInt);
  //      Serial.println();
  //
  //      bt.flush();
  //    }
  //  }

  while (bt.available() > 0) {
    incomingData = bt.readString();
    Serial.println(incomingData);

    if (incomingData == "f") { 
      Serial.println("fadePattern");
      setColor(0, 0, 0);
      redTempValue = "";
      greenTempValue = "";
      blueTempValue = "";
      while (1) {
        fadePattern();
        if (bt.available() > 0) break;
      }
    }

    else if (incomingData == "r") {
      Serial.println("rainbowPattern");
      while (1) {
        setColor(0, 0, 0);
        redTempValue = "";
        greenTempValue = "";
        blueTempValue = "";
        rainbowPattern();
        if (bt.available() > 0) break;
      }
    }

    else if (incomingData == "off") {
      Serial.println("offLED");
      setColor(0, 0, 0);
    }

    //    else if (incomingData = "on") {
    //      Serial.println("onLED");
    //      setColor(255, 255, 255);
    //    }

    else {
      Serial.println("RGB");
      for (int i = 0; i < incomingData.length(); i++) {
        char c = incomingData.charAt(i);

        if (c == 'R' )     {
          currentColor = 'R';
          redTempValue = "";
        }
        else if (c == 'G') {
          currentColor = 'G';
          greenTempValue = "";
        }
        else if (c == 'B') {
          currentColor = 'B';
          blueTempValue = "";
        }

        if      (currentColor == 'R' && c != 'R') redTempValue += c;
        else if (currentColor == 'G' && c != 'G') greenTempValue += c;
        else if (currentColor == 'B' && c != 'B') blueTempValue += c;

      }

      Serial.println(redTempValue.toInt());
      Serial.println(greenTempValue.toInt());
      Serial.println(blueTempValue.toInt());
      setColor(redTempValue.toInt(), greenTempValue.toInt(), blueTempValue.toInt());
    }
  }

}
