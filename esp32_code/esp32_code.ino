enum comands {
  Velocity = 128,
  Angle,
  STOP = 131,
  Movement,
  UnknownErr = 200,
  DataErr,
  ConnectErr,
  PING,
  GyroCalibrate,
  NONE
};

comands currentSTATE = NONE;

long timer = millis();

struct incomingDate {           //структура для чтения данных
  uint8_t comandCode;
  uint16_t value;
  uint8_t direct;
  uint16_t second_value;
};

incomingDate incMSG;

void reciveDatesFormRos() {     //читаем пришедшую команду
  if (Serial.readBytes((byte*)&incMSG, sizeof(incMSG)));
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  reciveDatesFormRos();

  switch (incMSG.comandCode) {
    case Velocity:
      //Serial.print(incMSG);
      break;

    case Angle:

      break;

    case STOP:

      break;

    case Movement:

      break;

    case PING:

      break;

    case NONE:
      //Serial.write();
      break;
  }

  if (millis() - timer > 1000) {

    timer = millis();
  }
}
