/**
 * @file Line.cpp
 * @brief Methods for line detection and angle calculation
 * 
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 06/04/26
 */

#include "Line.h"

Line::Line(int _threshold) {
  pinMode(13, OUTPUT);
  analogReadResolution(12);
  delay(1000);
  threshold = _threshold;
}
int Line::getLineSector(float lineAngle) {
    //Esta funcion obitiene el sector actual de la linea,
    //La informacion se utiliza para evitar que el robot al pasar de la mitad de la linea voltee la normal y
    //acelere hacia la pared en vez de regresar al campo.
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
int Line::lineSwitch(int sector, int lastSector) {
    //Funcion que detecta si se paso la mitad de la linea
    //si se paso, regresa el sector inicial en vez del actual.
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


int Line::adjustLineAngle(int angle) {
    //Funcion que retorna el angulo opuesto al que se le da,
    //se utiliza como el angulo objetivo del robot al que debe de ir si detecta linea
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

void Line::update() {
  float sumCos = 0;
  float sumSin = 0;
  int numRead = 0;
  for (int i = 0; i < 15; i++) {
    int val = analogRead(analogs[i]);
    val = (val > threshold) ? 1 : 0;
    detectedSensors[i] = val;
//    Serial.print(val);
//    Serial.print(" ");
    if (val == 1) {
      numRead++;
      sumCos += cosenos[i];
      sumSin += senos[i];
    }
  }
//  Serial.println();

  if (numRead == 0) {
    angle = 500;
    sector = -1;
    firstSector = -1;
    fixedLineAngle = 500;
    avoidAngle = 500;
    firstDetected = false;

    digitalWrite(13, LOW);
  } else {
    angle = atan2(sumSin, sumCos) * 180.0 / 3.14159265;
    if (angle < 0) angle += 360.0;


    sector = getLineSector(angle);

    if (!firstDetected && sector != -1) {
      firstSector = sector;
      firstDetected = true;
    }

    if (sector != -1 && firstSector != -1) {
      fixedLineAngle = lineSwitch(sector, firstSector);
      avoidAngle = adjustLineAngle(fixedLineAngle);
    } else {
      fixedLineAngle = 500;
      avoidAngle = 500;
    }

    digitalWrite(13, HIGH);
  }
}

int Line::getAvoidAngle() {
  return avoidAngle;
}