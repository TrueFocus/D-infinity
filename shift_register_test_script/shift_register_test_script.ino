// ST_CP pin = 10
const int latchPin = 10;
// SH_CP pin = 11
const int clockPin = 11;
// DS pin 12
const int dataPin = 12;

const uint8_t num2segment[10] = {
  0b11111100, //0
  0b01100000,
  0b11011010,
  0b11110010,
  0b01100110,
  0b10110110,
  0b10111110,
  0b11100000,
  0b11111110,
  0b11110110
};


void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

}

void loop() {
  for(int i=0; i<10; i++){
  // set latchpin low so no LED's changes while reading data
  digitalWrite(latchPin, HIGH);
  shiftOut(dataPin, clockPin, LSBFIRST, num2segment[i]); // disp 10110000
  // Update LED's
  digitalWrite(latchPin, LOW);
  delay(1000);
  }
}
