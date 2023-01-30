//Rotation pins
int RVRx = A0; // VRx to analog input 0
int RVRy = A1; // VRy to analog input 1
int RSW = 2; // SW to digital pin 2
int RxPosition = 0; // Initialize xPosition variable with 0
int RyPosition = 0;
int RButtonState = 0; // Initialize yPosition variable with 0

//Movement pins
int MVRx = A2; // VRx to analog input 0
int MVRy = A3; // VRy to analog input 1
int MSW = 3; // SW to digital pin 2
int MxPosition = 0; // Initialize xPosition variable with 0
int MyPosition = 0;
int MButtonState = 0; // Initialize yPosition variable with 0

//Zoom pins
int ZoomOUT = 5;
int ZoomIN = 4;
int ZoomOUT_state = 0;
int ZoomIN_state = 0;

//Planet pins
int sun = 26;
int sunState = 0;
int mercury = 28;
int mercuryState = 0;
int venus = 30;
int venusState = 0;
int earth = 32;
int earthState = 0;
int mars = 34;
int marsState = 0;
int jupiter = 36;
int jupiterState = 0;
int saturn = 38;
int saturnState = 0;
int uranus = 40;
int uranusState = 0;
int neptune = 42;
int neptuneState = 0;


void setup() {
  Serial.begin(9600); 

  pinMode(RVRx, INPUT); 
  pinMode(RVRy, INPUT); 
  pinMode(RSW, INPUT_PULLUP); 
  pinMode(MVRx, INPUT); 
  pinMode(MVRy, INPUT); 
  pinMode(MSW, INPUT_PULLUP); 
  pinMode(ZoomOUT, INPUT_PULLUP);
  pinMode(ZoomIN, INPUT_PULLUP);
  pinMode(sun, INPUT_PULLUP);
  pinMode(mercury, INPUT_PULLUP);
  pinMode(venus, INPUT_PULLUP);
  pinMode(earth, INPUT_PULLUP);
  pinMode(mars, INPUT_PULLUP);
  pinMode(jupiter, INPUT_PULLUP);
  pinMode(saturn, INPUT_PULLUP);
  pinMode(uranus, INPUT_PULLUP);
  pinMode(neptune, INPUT_PULLUP);

}
void loop() {
  RxPosition = analogRead(RVRx); 
  RyPosition = analogRead(RVRy); 
  MxPosition = analogRead(MVRx); 
  MyPosition = analogRead(MVRy); 
  RButtonState = digitalRead(RSW);
  MButtonState = digitalRead(MSW);
  ZoomOUT_state = digitalRead(ZoomOUT);
  ZoomIN_state = digitalRead(ZoomIN);
  sunState = digitalRead(sun);
  mercuryState = digitalRead(mercury);
  venusState = digitalRead(venus);
  earthState = digitalRead(earth);
  marsState = digitalRead(mars);
  jupiterState = digitalRead(jupiter);
  saturnState = digitalRead(saturn);
  uranusState = digitalRead(uranus);
  neptuneState = digitalRead(neptune);
  

  
  RotateTo(RxPosition, RyPosition);
  Zoom(ZoomIN_state, ZoomOUT_state);
  MoveTo(MxPosition, MyPosition);
  Planets(sunState, mercuryState, venusState, earthState, marsState, jupiterState, saturnState, uranusState, neptuneState);
  Reset(MButtonState,RButtonState);
  delay(200);

}

void Planets(int su, int me, int v, int e, int ma, int j, int sa, int u, int n){

  if (su == HIGH){
    Serial.println("P-Sun");
    delay(100);
  }
  if (me == HIGH){
    Serial.println("P-Mercury");
        delay(100);

  }
  if (v == HIGH){
    Serial.println("P-Venus");
      delay(100);

  }
  if (e == HIGH){
    Serial.println("P-Earth");
      delay(100);

  }
  if (ma == HIGH){
    Serial.println("P-Mars");
      delay(100);

  }
  if (j == HIGH){
    Serial.println("P-Jupiter");
      delay(100);

  }
  if (sa == HIGH){
    Serial.println("P-Saturn");
      delay(100);

  }
  if (u == HIGH){
    Serial.println("P-Uranus");
      delay(100);

  }
  if (n == HIGH){
    Serial.println("P-Neptune");
      delay(100);

  }
}
 
void Zoom(int in, int out){
  if (in == HIGH){
    Serial.println("Z-IN");
    delay(100);
  }
  if (out == HIGH){
    Serial.println("Z-OUT");
    delay(100);
  }
}

void Reset(int MR, int RR){
  if (MR == LOW || RR == LOW){
      Serial.println("Reset");
  }
  delay(100);
  }

void RotateTo(int x, int y){
  if (x > 800 && y <= 767 && y >= 249) {
    Serial.println("R-L");
  } else if (x < 300 && y <= 767 && y >= 249) {
    Serial.println("R-R");
  } else if (x <= 767 && x >= 249 && y > 800) {
    Serial.println("R-U");
  } else if (x <= 767 && x >= 249 && y < 300) {
    Serial.println("R-D");
  } else if (x <= 508 && x >= 491 && y <= 508 && y >= 491){
    
  } else if (x > 800 && y > 800) {
    Serial.println("R-UL");
  } else if (x > 800 && y < 300) {
    Serial.println("R-DL");
  } else if (x < 300 && y > 800) {
    Serial.println("R-UR");
  } else if (x < 300 && y < 300) {
    Serial.println("R-DR");

  }
} 
void MoveTo(int x, int y){
  if (x > 800 && y <= 767 && y >= 249) {
    Serial.println("M-L");
  } else if (x < 300 && y <= 767 && y >= 249) {
    Serial.println("M-R");
  } else if (x <= 767 && x >= 249 && y > 800) {
    Serial.println("M-U");
  } else if (x <= 767 && x >= 249 && y < 300) {
    Serial.println("M-D");
  } else if (x <= 508 && x >= 491 && y <= 508 && y >= 491){
    
  } else if (x > 800 && y > 800) {
    Serial.println("M-UL");
  } else if (x > 800 && y < 300) {
    Serial.println("M-DL");
  } else if (x < 300 && y > 800) {
    Serial.println("M-UR"); 
  } else if (x < 300 && y < 300) {
    Serial.println("M-DR");

  }
  

}