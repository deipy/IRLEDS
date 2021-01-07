//This code controls 3Pins RGB LED Strips using an Arduino Nano and an IR Sensor. 
//It features two animated modes, and 8 premade colors.
//Written by Deipy                                                        https://github.com/deipy
//
//---------------------TODO-----------------------------------------------------------------------
// -Handle IR commands trough SwitchCase
// -Handle primary and secondary color selection trough a setPrimary(r,g,b) and setSecondary(r,g,b)
// -Use remaing seven buttons
//--------------OTHER UNASSIGNED BUTTONS ---------------------------------------------------------
//ONE=16199887
//TWO=16232527
//THREE=16216207
//FOUR=16248847
//FIVE=16189687
//SIX=16222327
//SEVEN=16206007
//EIGHT=16238647
//------------------------------------------------------------------------------------------------

#include <IRremote.h>
#include <FastLED.h>        
#define LED_PIN 7           //LED DATA to pin 7
#define NUM_LEDS 4          //Number of leds or groups of LEDS
#define SCROLL_SPEED 50    
#define SWIPE_SPEED 100

//----------------GLOBAL VARIABLES-----------------------------------------------------------------
int r=100;    //primary color
int g=100;
int b=255; 
    
int rb=0;     //secondary color (used by swipe function)
int gb=0;
int bb=0;

int group=0;      
int prevMillis=0;
int currentMillis=0;
bool secondary_select=false;
bool hue_scroll = false;
bool swipe=false;
int interval=50;
int hue = 0;
String ir_msg="";
int recv_pin = 8;       //IR DATA to pin 8
CRGB leds[NUM_LEDS];
IRrecv receiver(recv_pin);
decode_results results;
//-------------------------------------------------------------------------------------------------

void setLedGroup(int group_index, int red, int green, int blue){  //Sets a led group withing NUM_LEDS 
  leds[group_index] = CRGB (red, blue, green);                    //to a particular color if leds are addressable one by one
  FastLED.show();                                                 //use this to set the color of a single led as well.
}

void setAllLeds(int red, int green, int blue){   //set all leds to a specific color   //sets all LEDS to a specific color
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB (red, blue, green);
        FastLED.show();
    }  
}

void hsv2rgb(float h, float s, float v){   //HSV to RGB converter to make cycling easier (FastLED does it, I know lol, 
   float c = v *s ;                        //i wrote this before knowing that it did it, so used it anyways
   float x = c * ( 1 - abs(fmod((h/60), 2)-1));
   float m = v - c;

   if(h<60 && h>=0){
    r=(c+m)*255;
    g=(x+m)*255;
    b=(0+m)*255;
   }
   else if (h<120 && h>=60){
    r=(x+m)*255;
    g=(c+m)*255;
    b=(0+m)*255;
   }
   else if (h<180 && h>=120){
    r=(0+m)*255;
    g=(c+m)*255;
    b=(x+m)*255;
   }
   else if (h<240 && h>=180){
    r=(0+m)*255;
    g=(x+m)*255;
    b=(c+m)*255;
   }
   else if (h<300 && h>=240){
    r=(x+m)*255;
    g=(0+m)*255;
    b=(c+m)*255;
   }
   else if(h<360 && h>=300){
    r=(c+m)*255;
    g=(0+m)*255;
    b=(x+m)*255;
   }
}
 
void Swipe(){  //handling swiping between two colors, one LED group at a time
  group+=1;
  if(group==NUM_LEDS){
    group=0;
  }
  setAllLeds(r,g,b);
  setLedGroup(group, rb, gb, bb);
}

void ScrollHue(){ //handling cycling through the color spectrum
  hue+=1;
  if(hue > 360){
    hue=0;
  }
  hsv2rgb(hue, 1, 1);
  setAllLeds(r,g,b);
}

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  receiver.enableIRIn();
  Serial.begin(9600);
}

void loop() {
  currentMillis = millis();                      //Handle timed functions
  if(currentMillis - prevMillis >= interval){
    if(hue_scroll){
      ScrollHue();
    }
    if(swipe){
      Swipe();
    }
    prevMillis = currentMillis;  
  }

  if(receiver.decode(&results)){               //Handle the IR commands 
    if(results.value == 16187647){
      Serial.println("CYCLE");
      interval=SCROLL_SPEED;
      hue_scroll = true;
      swipe=false;
      
    }
    else if(results.value == 16220287){
      Serial.println("SWIPE");
      interval=SWIPE_SPEED;
      swipe=true;
      hue_scroll = false;
      
    }
    else if(results.value == 16203967){
      Serial.println("ON");
      hue_scroll = false;
      swipe=false;
      setAllLeds(r,g,b);
    }
    else if(results.value == 16236607){
      Serial.println("OFF");
      hue_scroll = false;
      swipe=false;
      setAllLeds(0,0,0);
    }
    else if(results.value == 16195807){
      if(!secondary_select){
        Serial.println("RED");
        hue_scroll = false;
        swipe=false;
        r=255;
        g=0;
        b=0;
        setAllLeds(r,g,b);
      }
      else{
        rb=255;
        gb=0;
        bb=0;
        secondary_select=false;
      }
    }
    else if(results.value == 16228447){
      if(!secondary_select){
        Serial.println("GREEN");
        hue_scroll = false;
        swipe=false;
        r=0;
        g=255;
        b=0;
        setAllLeds(r,g,b);
      }
      else{
        rb=0;
        gb=255;
        bb=0;
        secondary_select=false;
      }
    }
    else if(results.value == 16212127){
      if(!secondary_select){
        Serial.println("BLUE");
        hue_scroll = false;
        swipe=false;
        r=0;
        g=0;
        b=255;
        setAllLeds(r,g,b);
      }
      else{
        rb=0;
        gb=0;
        bb=255;
        secondary_select=false;
      }
    }
    else if(results.value == 16244767){
      if(!secondary_select){
        Serial.println("WHITE");
        hue_scroll = false;
        swipe=false;
        r=255;
        g=255;
        b=255;
        setAllLeds(r,g,b);
      }
      else{
        rb=255;
        gb=255;
        bb=255;
        secondary_select=false;
      }
    }
    else if(results.value == 16191727){
      if(!secondary_select){
        Serial.println("ORANGE");
        hue_scroll = false;
        swipe=false;
        r=255;
        g=80;
        b=0;
        setAllLeds(r,g,b);
      }
      else{
        rb=255;
        gb=80;
        bb=0;
        secondary_select=false;
      }
    }
    else if(results.value == 16224367){
      if(!secondary_select){
        Serial.println("YELLOW");
        hue_scroll = false;
        swipe=false;
        r=255;
        g=150;
        b=0;
        setAllLeds(r,g,b);
      }
      else{
        rb=255;
        gb=150;
        bb=0;
        secondary_select=false;
      }
    }
    else if(results.value == 16208047){
      if(!secondary_select){
        Serial.println("CYAN");
        hue_scroll = false;
        swipe=false;
        r=0;
        g=123;
        b=255;
        setAllLeds(r,g,b);
      }
      else{
        rb=0;
        gb=123;
        bb=255;
        secondary_select=false;
      }
    }
    else if(results.value == 16240687){
      if(!secondary_select){
        Serial.println("PURPLE");
        hue_scroll = false;
        swipe=false;
        r=255;
        g=0;
        b=255;
        setAllLeds(r,g,b);
      }
      else{
        rb=255;
        gb=0;
        bb=255;
        secondary_select=false;
      }
    }
    else if(results.value == 16238647){   //Handles secondary color selection for Swipe() function
      Serial.println("SECONDARY : ");
      secondary_select=!secondary_select;
      Serial.print(secondary_select);
    }
    Serial.println(results.value);  //prints the IR INT code to Serial for debug.
    receiver.resume();
  }
}
