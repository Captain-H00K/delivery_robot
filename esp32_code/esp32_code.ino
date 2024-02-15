enum comands {
  Velocity,
  Angle,
  STOP,
  Movement,
  NONE
};

comands currentSTATE = NONE;

long timer = millis();

struct incomingDate {
  uint16_t value;
  uint8_t direct;
  uint8_t second_value;
};

void reciveDatesFormRos() {
  if(mySerial.readBytes((byte*)&incomingCommand, sizeof(incomingCommand)))
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  reciveDatesFormRos();
  
  switch (currentSTATE) {
    case Velocity:

      break;

    case Angle:

      break;

    case STOP:

      break;

    case Movement:

      break;
  }

  if(millis() - timer > 1000) {

    timer = millis();
  }
}
