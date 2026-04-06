int analogs[15] = {17,18,19,20,24,25,26,27,41,40,39,38,14,15,16};
// 0, 22.5, 45, 67.5, 90...
float cosenos[15] = {1,0.9238,0.7071,0.3826,0,-0.3826,-0.7071,-0.9238,-0.9238,-0.7071,-0.3826,0,0.3826,0.7071,0.9238};
float senos[15]   = {0,0.3826,0.7071,0.9238,1,0.9238,0.7071,0.3826,-0.3826,-0.7071,-0.9238,-1,-0.9238,-0.7071,-0.3826};

int treshold = 300;
float angle = 500;

bool detectedSensors[15];
int depth = 15;
int side = 0;

bool firstDetected = false;
int firstSector = -1;
int sector = -1;
int fixedLineAngle = 500;
int avoidAngle = 500;

void calculateDepth();
void calculateSide();
int getLineSector(float lineAngle);
int line_switch(int sector, int lastSector);
int adjustLineAngle(int angle);

void setup() {
  Serial.begin(115200);
  Serial1.begin(2000000);
  pinMode(13, OUTPUT);
  analogReadResolution(12);
  delay(1000);
}

void loop() {
  float sumCos = 0;
  float sumSin = 0;
  int numRead = 0;

  for (int i = 0; i < 15; i++) {
    int val = analogRead(analogs[i]);
    val = (val > treshold) ? 1 : 0;

    detectedSensors[i] = val;

    Serial.print(val);
    Serial.print(" ");

    if (val == 1) {
      numRead++;
      sumCos += cosenos[i];
      sumSin += senos[i];
    }
  }

  Serial.println();

  if (numRead == 0) {
    angle = 500;
    depth = 15;
    side = 0;

    sector = -1;
    firstSector = -1;
    fixedLineAngle = 500;
    avoidAngle = 500;
    firstDetected = false;

    digitalWrite(13, LOW);
  } else {
    angle = atan2(sumSin, sumCos) * 180.0 / 3.14159265;
    if (angle < 0) angle += 360.0;

    calculateDepth();
    calculateSide();

    sector = getLineSector(angle);

    if (!firstDetected && sector != -1) {
      firstSector = sector;
      firstDetected = true;
    }

    if (sector != -1 && firstSector != -1) {
      fixedLineAngle = line_switch(sector, firstSector);
      avoidAngle = adjustLineAngle(fixedLineAngle);
    } else {
      fixedLineAngle = 500;
      avoidAngle = 500;
    }

    digitalWrite(13, HIGH);
  }

  Serial.print("angle: ");
  Serial.print(angle);

  Serial.print("  sector: ");
  Serial.print(sector);

  Serial.print("  firstSector: ");
  Serial.print(firstSector);

  Serial.print("  fixedLineAngle: ");
  Serial.print(fixedLineAngle);

  Serial.print("  avoidAngle: ");
  Serial.print(avoidAngle);

  Serial.print("  depth: ");
  Serial.print(depth);

  Serial.print("  side: ");
  Serial.println(side);
  Serial1.write(avoidAngle / 2);
}

void calculateDepth() {
  for (int i = 0; i <= 7; i++) {
    int mirror;

    if (i == 0) mirror = 0;
    else mirror = 15 - i;

    bool left = detectedSensors[i];
    bool right = detectedSensors[mirror];

    if (left && right) {
      depth = i + 1;
      return;
    }

    if (left) {
      if ((mirror > 0 && detectedSensors[mirror - 1]) ||
          (mirror < 14 && detectedSensors[mirror + 1])) {
        depth = i + 1;
        return;
      }
    }

    if (right) {
      if ((i > 0 && detectedSensors[i - 1]) ||
          (i < 14 && detectedSensors[i + 1])) {
        depth = i + 1;
        return;
      }
    }
  }

  depth = 15;
}

void calculateSide() {
  side = 0;

  for (int i = 4; i <= 7; i++) {
    int mirror = 15 - i;

    bool left = detectedSensors[i];
    bool right = detectedSensors[mirror];

    if (left) {
      bool cond0 = (mirror > 1) ? !detectedSensors[mirror - 2] : true;
      bool cond1 = (mirror > 0) ? !detectedSensors[mirror - 1] : true;
      bool cond2 = !detectedSensors[mirror];
      bool cond3 = (mirror < 14) ? !detectedSensors[mirror + 1] : true;
      bool cond4 = (mirror < 13) ? !detectedSensors[mirror + 2] : true;

      if ((cond1 && cond2 && cond3) && (cond0 || cond4)) {
        side = 3;
        return;
      }
    }
    else if (right) {
      bool cond0 = (i > 1) ? !detectedSensors[i - 2] : true;
      bool cond1 = (i > 0) ? !detectedSensors[i - 1] : true;
      bool cond2 = !detectedSensors[i];
      bool cond3 = (i < 14) ? !detectedSensors[i + 1] : true;
      bool cond4 = (i < 13) ? !detectedSensors[i + 2] : true;

      if ((cond1 && cond2 && cond3) && (cond0 || cond4)) {
        side = 1;
        return;
      }
    }
  }
}

int getLineSector(float lineAngle) {
  while (lineAngle < -15) lineAngle += 360;
  while (lineAngle >= 345) lineAngle -= 360;

  for (int i = 0; i < 12; i++) {
    int lower = -15 + (i * 30);
    int upper =  15 + (i * 30);

    if (lineAngle >= lower && lineAngle < upper) {
      return i;
    }
  }

  return -1;
}

int line_switch(int sector, int lastSector) {
  int angle = sector * 30;

  if (lastSector <= 3) {
    if (3 + lastSector <= sector && sector <= 8 + lastSector) {
      if (sector == 3) angle = 90;
      else angle = lastSector * 30;
    }
  }
  else if (4 <= lastSector && lastSector <= 8) {
    if (sector <= lastSector - 4 || lastSector + 3 <= sector) {
      angle = lastSector * 30;
    }
  }
  else if (9 <= lastSector) {
    if (lastSector - 9 <= sector && sector <= lastSector - 4) {
      angle = lastSector * 30;
    }
  }

  angle = (angle % 360 + 360) % 360;
  return angle;
}

int adjustLineAngle(int angle) {
  if (angle >= 0 && angle < 180) {
    return angle + 180;
  }
  else if (angle <= 360 && angle >= 180) {
    return angle - 180;
  }
  else {
    return angle;
  }
}