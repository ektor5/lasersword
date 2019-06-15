#include <VirtualWire.h>
// virtualwire use pin 12 for TX and pin 11 for RX

#define BUTTON_PIN         8
#define LED_PIN            10
#define WIFI_PIN           7

uint8_t buf[8];
uint8_t buflen = 8;

int delayval = 250; 
int ex_speed = 1;
int buttonState = 0; 
int already_sent = false;
int trim_value;


void setup() {
  Serial.begin(9600);

   
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  vw_set_tx_pin(WIFI_PIN);
//  vw_set_ptt_inverted(true);
  vw_setup(6000);      // Bits per sec

  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == HIGH) {
    if(!already_sent) {
      tx_wifi(0);
      already_sent = true;
    }
  }
  else  {
    // check trimmer value.
    trim_value = analogRead(A0) + 1; // 1:1025
    trim_value = int(trim_value / 10);
    if(trim_value>100)
      trim_value = 100;
    if(trim_value==0)
      trim_value = 1;
    if(trim_value != ex_speed) {
      ex_speed = trim_value;
      tx_wifi((char)trim_value);   // trim_value 1-100
    }
    already_sent = false;
  }
  delay(delayval);
}


int tx_wifi(char value) {
  char msg[16];

  Serial.print("Transmitting...  ");
  Serial.println(int(value));
      
  msg[0] = 126;  // control code
  msg[1] = value;  // value range 1:100 (1:100)

  vw_send((uint8_t *)msg, 2);
  vw_wait_tx(); // Wait until the whole message is gone

  // pulse LED
  digitalWrite(LED_PIN, LOW);
  delay(300);
  digitalWrite(LED_PIN, HIGH);
}
