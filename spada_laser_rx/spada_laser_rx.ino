


// NeoPixel simple sketch per Stefania Granai (c) 2019 Ettore Chimenti
// released under the GPLv3 license 

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include <VirtualWire.h>
// virtualwire use pin 12 for TX and pin 11 for RX

#define NEOPIN            10

//Number of leds
#define NUMPIXEL       46

//Brightness leds
#define BRIGH          150
#define BRIGH_T1       80 //BRIGH/5
#define BRIGH_T2       20 //BRIGH/10

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXEL, NEOPIN, NEO_GRB + NEO_KHZ800);

uint8_t buf[VW_MAX_MESSAGE_LEN];
uint8_t buflen = VW_MAX_MESSAGE_LEN;
    
volatile int delayval = 40; // delay for half a second
volatile bool stop_flag = true;

void setup() {
  Serial.begin(9600);
  pixels.begin();

  vw_set_rx_pin(8);
  // Initialise the IO and ISR
  vw_setup(2000);      // Bits per sec
  vw_rx_start();       // Start the receiver PLL running
}

void loop() {
  for(int i=1; i<NUMPIXEL; i++){
    if (stop_flag)
      break;

    // Tail effect
    if (i>2)
      pixels.setPixelColor(i-3, pixels.Color(0,0,0));   
    if (i>1)
      pixels.setPixelColor(i-2, pixels.Color(0,BRIGH_T2,0));    
    if (i>0)
      pixels.setPixelColor(i-1, pixels.Color(0,BRIGH_T1,0));
      
    pixels.setPixelColor(i, pixels.Color(0,BRIGH,0));
    
    if ( (i < NUMPIXEL-1) || (i>0) ) {
      pixels.setPixelColor(i+1, pixels.Color(0,0,0));
    }

//    Serial.print("Show!\t");
//    Serial.println(i);

    pixels.show(); // This sends the updated pixel color to the hardware.
    
    rx_wifi();
    delay(delayval); // Delay for a period of time (in milliseconds).
     
  }
  for(int i=NUMPIXEL-2;i>=0; --i){
    if (stop_flag)
        break;
    
    if ( i>0 || i<NUMPIXEL-1 )
        pixels.setPixelColor(i-1, pixels.Color(0,0,0));
      
    pixels.setPixelColor(i, pixels.Color(0,BRIGH,0));
    
    if (i<NUMPIXEL-1)
      pixels.setPixelColor(i+1, pixels.Color(0,BRIGH_T1,0));
    if (i<NUMPIXEL-2)
      pixels.setPixelColor(i+2, pixels.Color(0,BRIGH_T2,0));    
    if (i<NUMPIXEL-3)
      pixels.setPixelColor(i+3, pixels.Color(0,0,0));    

//    Serial.print("Show reverse!\t");
//    Serial.println(i);

    pixels.show(); // This sends the updated pixel color to the hardware.

    rx_wifi();
    delay(delayval); // Delay for a period of time (in milliseconds).
    
  }  
  
  if (stop_flag){
    pixels.clear();
    pixels.show(); 
    delay(100);
    pixels.setPixelColor(1, pixels.Color(BRIGH,0,0));
    pixels.show(); 
    delay(1000);
    pixels.setPixelColor(1, pixels.Color(0,0,0));
    pixels.show();
    rx_wifi();
  }
}


void rx_wifi() {
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  int res = -1;
  
  if(vw_have_message()) {
//    Serial.print("Received \t");
    vw_get_message(buf, &buflen);
    if(buflen == 2 && buf[0] == 126) {
      res = (int)buf[1];
      if(res>0) {
        if(res<10)
          res = 10;
        delayval = res*2;
      }
      else
        stop_flag = !stop_flag;
//      Serial.println(res);
    }
  }
}
