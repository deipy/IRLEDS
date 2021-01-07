#include <IRremote.h>

int recv_pin = 8;

IRrecv receiver(recv_pin);
decode_results results;

void setup() {
  Serial.begin(9600);
  receiver.enableIRIn();
}

void loop() {
 if(receiver.decode(&results){
    Serial.println(results);
 }
 receiver.resume();
}
