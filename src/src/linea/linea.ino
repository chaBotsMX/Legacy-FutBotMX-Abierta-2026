int analogs[15] = {17,18,19,20,24,25,26,27,41,40,39,38,14,15,16};
// 22.5 , 45, 77.5 , 90
float cosenos[15] = {1,0.9238,0.7071,0.3826,0,-0.3826,-0.7071,-0.9238,-0.9238,-0.7071,-0.3826,0,0.3826,0.7071,0.9238};

float senos[15] = {0,0.3826,0.7071,0.9238,1,0.9238,0.7071,0.3826,-0.3826,-0.7071,-0.9238,-1,-0.9238,-0.7071,-0.3826};

int treshold = 200;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(13,OUTPUT); 
  analogReadResolution(12);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0;i<15;i++){
    int val = analogRead(analogs[i]);
    val > 200? val = 1 : val = 0;
    Serial.print(" ");
    Serial.print(val);
  }
  Serial.println(" ");
  digitalWrite(13,HIGH);
}
