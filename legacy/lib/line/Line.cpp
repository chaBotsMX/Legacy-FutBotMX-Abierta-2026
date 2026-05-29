/**
 * @file Line.cpp
 * @brief Methods for line detection and angle calculation
 * 
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 06/04/26
**/

#include "Line.h"

Line::Line() : pixels(NUM_SENSORS, LED_PIN, NEO_RGB + NEO_KHZ800) {
  pinMode(LED_PIN, OUTPUT);
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(analogs[i], INPUT);
  }
  analogWrite(vref[0], 200);
  analogWrite(vref[1], 200);
  analogWrite(vref[2], 200);
  analogWrite(vref[3], 200);

  delay(STARTUP_DELAY);

  calculateSinAndCos();
  
  pixels.begin();
  pixels.clear();

  for(int i = 0; i < NUM_SENSORS; i++){
    unsigned long start = millis();
    while(millis() - start < 20){}
    pixels.setPixelColor(i, pixels.Color(80, 80, 80));
    pixels.show();
  }
}

int Line::getLineSector(float lineAngle) {
  //Esta funcion obitiene el sector actual de la linea,
  //La informacion se utiliza para evitar que el robot al pasar de la mitad de la linea voltee la normal y
  //acelere hacia la pared en vez de regresar al campo.
  while (lineAngle < LOWER_ANGLE_BOUND) lineAngle += 360;
  while (lineAngle >= UPPER_ANGLE_BOUND) lineAngle -= 360;

  for (int i = 0; i < NUM_SECTORS; i++) {
    int lower = (i * SECTOR_ANGLE_STEP) - SECTOR_OFFSET;
    int upper = (i * SECTOR_ANGLE_STEP) + SECTOR_OFFSET;

    if (lineAngle >= lower && lineAngle < upper) {
      return i;
    }
  }

  return -1;
}

int Line::lineSwitch(int sector, int lastSector) {
    //Funcion que detecta si se paso la mitad de la linea
    //si se paso, regresa el sector inicial en vez del actual.
    int angle = sector * SECTOR_ANGLE_STEP;

    if (lastSector <= 3 && 3 + lastSector <= sector && sector <= 8 + lastSector) {
      if (sector == 3) angle = 90;
      else angle = lastSector * 30;
    }
    else if (4 <= lastSector && lastSector <= 8 && sector <= lastSector - 4 || lastSector + 3 <= sector) {
      angle = lastSector * 30;
    }
    else if (9 <= lastSector && lastSector - 9 <= sector && sector <= lastSector - 4) {
      angle = lastSector * 30;
    }

    return (angle % 360 + 360) % 360;
}


int Line::adjustLineAngle(int angle) {
    //Funcion que retorna el angulo opuesto al que se le da,
    //se utiliza como el angulo objetivo del robot al que debe de ir si detecta linea
    if (angle >= 0 && angle < 180) {
      return angle + 180;
    }
    else if (angle >= 180 && angle <= 360) {
      return angle - 180;
    }
    else {
      return angle;
    }
}

void Line::update() {
  float sumCos = 0;
  float sumSin = 0;
  int numRead = 0;

  for (int i = 0; i < NUM_SENSORS; i++) {
    int val = digitalRead(analogs[i]);
    val = (val == HIGH) ? 1 : 0;
    Serial.print(val);
    Serial.print(" ");

    if (!val) continue;

    numRead++;
    sumCos += cosenos[i];
    sumSin += senos[i];
  }
  Serial.println();

  if (!numRead) {
    angle = BALL_OUT_OF_RANGE;
    sector = -1;
    firstSectorDetected = -1;
    fixedLineAngle = BALL_OUT_OF_RANGE;
    avoidAngle = BALL_OUT_OF_RANGE;
    firstDetected = false;

    digitalWrite(LED_PIN, LOW);
  } else {
    angle = atan2(sumSin, sumCos) * 180.0 / PI;
    if (angle < 0) angle += 360.0;

    sector = getLineSector(angle);

    if (!firstDetected && sector != -1) {
      firstSectorDetected = sector;
      firstDetected = true;
    }

    if (sector != -1 && firstSectorDetected != -1) {
      fixedLineAngle = lineSwitch(sector, firstSectorDetected);
      avoidAngle = adjustLineAngle(fixedLineAngle);
    } else {
      fixedLineAngle = BALL_OUT_OF_RANGE;
      avoidAngle = BALL_OUT_OF_RANGE;
    }

    digitalWrite(LED_PIN, HIGH);
  }
}

void Line::calculateSinAndCos() {
  for(float angle = 0.0, sensorIdx = 0; angle <= 360.0; angle += LINE_SENSOR_ANGLE_STEP, sensorIdx++) {
    cosenos[(int)sensorIdx] = cos(angle * PI / 180.0);
    senos[(int)sensorIdx] = sin(angle * PI / 180.0);
  }
}