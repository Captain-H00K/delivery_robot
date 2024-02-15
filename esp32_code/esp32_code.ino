enum comands {
  Velocity,
  Angle,
  STOP,
  Movement,
  ConCheck,
  NONE
};

comands currentSTATE = NONE;

long timer = millis();

struct incomingDate {
  uint8_t comandCode;
  uint16_t value;
  uint8_t direct;
  uint8_t second_value;
};

void reciveDatesFormRos() {
  if (mySerial.readBytes((byte*)&incomingCommand, sizeof(incomingCommand)))
  }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  reciveDatesFormRos();

  currentSTATE = incomingDate.comandCode;

  switch (currentSTATE) {
    case Velocity:

      break;

    case Angle:

      break;

    case STOP:

      break;

    case Movement:

      break;

    case ConCheck:

      break;

    case NONE:
        Serial.write();
      break;
  }

  if (millis() - timer > 1000) {

    timer = millis();
  }
}
