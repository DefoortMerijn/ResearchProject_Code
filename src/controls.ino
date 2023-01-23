int VRx = A0; // VRx to analog input 0
int VRy = A1; // VRy to analog input 1
int SW = 2; // SW to digital pin 2
int xPosition = 0; // Initialize xPosition variable with 0
int yPosition = 0; // Initialize yPosition variable with 0
int SW_state = 0;
int prevXPos = 0;
int prevYPos = 0; // Initialize SW_state variable with 0
void setup() {
  Serial.begin(9600); // Initialize Serial connection with a baudrate of 9600 bits/s

  pinMode(VRx, INPUT); // Set VRx pin as INPUT
  pinMode(VRy, INPUT); // Set VRy pin as INPUT
  pinMode(SW, INPUT_PULLUP); // Enable internal pullup resistor on switch pin

}
void loop() {
  xPosition = analogRead(VRx); // Read x position of joystick
  yPosition = analogRead(VRy); // Read y position of joystick
  SW_state = digitalRead(SW); // Read switch state

  // Serial.print("X: ");
  // Serial.println(xPosition);
  // Serial.print("Y: " );
  // Serial.println(yPosition);
  // Serial.print("Button: ");
  // Serial.println(SW_state);
rotateTo(xPosition, yPosition);
  delay(100);

}

void rotateTo(int x, int y){
  if (x > 800 && y <= 508 && y >= 491) {
    Serial.println("R-Left");
  } else if (x < 300 && y <= 508 && y >= 491) {
    Serial.println("R-Right");
  } else if (x <= 507 && x >= 491 && y > 800) {
    Serial.println("R-Up");
  } else if (x <= 507 && x >= 491 && y < 300) {
    Serial.println("R-Down");
  } else if (x <= 508 && x >= 491 && y <= 508 && y >= 491)
  {
 Serial.println("R-Center");
  }

}