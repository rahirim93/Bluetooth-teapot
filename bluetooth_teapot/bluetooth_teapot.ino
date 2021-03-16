long delayPeriod = 35000;
long counterPeriod = 0;
long counterFlash = 0;
boolean flagOn;
long counterPeriodTemp = 0;

boolean teapotIsOn;

char val;
String val2 = "";
long periodWake = 5000;
long counter = 0;

float tempC;
int reading;
void setup() {
  analogReference(INTERNAL);// включаем внутрений источник опорного 1,1 вольт

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  digitalWrite(2, 0);
  digitalWrite(3, 0);
  digitalWrite(4, 0);
  digitalWrite(5, 0);
  digitalWrite(6, 0);
  digitalWrite(7, 0);
  digitalWrite(8, 0);


  Serial.begin(9600);
  // put your setup code here, to run once:
  digitalWrite(13, 1);
}

void loop() {
  funChar();

  flash();

  tempControl();
}

void funChar() {

  if (Serial.available()) {
    val = Serial.read();
    //Serial.println(val);
    // При символе "1" включаем светодиод
    if (val == '1') {
      digitalWrite(13, 1);
    }
    // При символе "0" выключаем светодиод
    if ( val == '0') {
      digitalWrite(13, 0);
    }
  }
}
//Функция для пробуждения посредтвом включения светодидов через определенные промежутки времени
void flash() {
  if (millis() - counterPeriod > delayPeriod) {
    counterPeriod = millis();
    digitalWrite(2, 1);
    digitalWrite(3, 1);
    digitalWrite(4, 1);
    digitalWrite(5, 1);
    digitalWrite(6, 1);
    digitalWrite(7, 1);
    digitalWrite(8, 1);

    flagOn = true;
  }
  if (flagOn) {
    if (millis() - counterPeriod > 1000) {
      digitalWrite(2, 0);
      digitalWrite(3, 0);
      digitalWrite(4, 0);
      digitalWrite(5, 0);
      digitalWrite(6, 0);
      digitalWrite(7, 0);
      digitalWrite(8, 0);

      flagOn = false;
    }
  }
}

//Функция термоконтроля
void tempControl() {
  //Таймер опроса каждую секунду
  if (millis() - counterPeriodTemp > 1000) {
    counterPeriodTemp = millis();
    reading = analogRead(A0);        // получаем значение с аналогового входа A0
    tempC = reading / 9.31;          // переводим в цельсии
    Serial.println(tempC);           // Отправка температуры на телефон
    //При достижении указанной температуры отключить реле
    if (tempC > 85) {
      digitalWrite(13, 1);
    }
  }
}
