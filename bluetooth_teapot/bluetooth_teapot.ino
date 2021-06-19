// Импорт необходимых библиотек
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 and 3 on the Arduino
#define ONE_WIRE_BUS 2
#define SECOND_WIRE_BUS 3

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
OneWire secondWire(SECOND_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
DallasTemperature sensors2(&secondWire);

// Переменная счетчика. Для отправки данных с интервалом
long counterPeriodTemp = 0;

// Для хранения принятого символа
char val;

//Температура отключения чайника
float tempOff = 98.0;

float firstPointTemp;           // Первое значение температуры для нахождения скорости нагрева
long firstPointTime;            // Значение времени при котором был замер температуры                
float speedOfHeatingOff = 4.0;  // Скорость нагрева при которой чайник отключается

void setup() {

  Serial.begin(9600);

  // Start up the library
  sensors.begin();
  sensors2.begin();

  digitalWrite(13, 1);

  firstPointTemp = getTempFirstSensor();  // Считываем первое значение температуры
  firstPointTime = millis();              // Время при котором было считано первое значение температуры

}

void loop() {

  funChar();

  tempControl();

}

void funChar() {

  if (Serial.available()) {
    val = Serial.read();
    //Serial.println(val);
    // При символе "1" выключаем чайник
    if (val == '1') {
      digitalWrite(13, 1);
    }
    // При символе "0" включаем чайник
    if ( val == '0') {
      digitalWrite(13, 0);
    }
  }

}

// Функция получения температуры с первого датчика
float getTempFirstSensor() {
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  sensors.requestTemperatures(); // Send the command to get temperatures
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  float tempC = sensors.getTempCByIndex(0);
  if (tempC != DEVICE_DISCONNECTED_C) {
    return tempC;
  } else return 0;

}

// Функция получения температуры со второго датчика
float getTempSecondSensor() {
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  sensors2.requestTemperatures(); // Send the command to get temperatures
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  float tempC2 = sensors2.getTempCByIndex(0);
  if (tempC2 != DEVICE_DISCONNECTED_C) {
    return tempC2;
  } else return 0;
}

// Функция отправки температур и состояния реле
void sendingTempAndRelayStatus() {

  Serial.print(getTempFirstSensor());    // Отправка первого параметра при успешном чтении
  Serial.print("A");  // Символ, который определяет где кончается первый параметр
  Serial.print(getTempSecondSensor());    // Отправка второго параметра при успешном чтении
  Serial.print("B");  // Символ, который определяет где кончается второй параметр
  // Отправка состояния реле.
  // Перенос строки определяет конец массива байтов и показывает где кончает данных параметр
  Serial.println(digitalRead(13));

}

// Функция подсчета скорости нагрева
float getSpeedOfHeating() {
  double timeDifference = (double) (millis() - firstPointTime) / 1000;
  float tempDifference = getTempFirstSensor() - firstPointTemp;
  double speedOfHeating = (double) tempDifference / timeDifference;
  firstPointTemp = getTempFirstSensor();
  firstPointTime = millis();
  return (float) speedOfHeating;
  }

//Функция термоконтроля
void tempControl() {
  //Таймер опроса каждую секунду
  if (millis() - counterPeriodTemp > 1000) {
    counterPeriodTemp = millis();

    sendingTempAndRelayStatus();

    // При достижении скорости нагрева больше указанной отключить реле
    if (getSpeedOfHeating() > speedOfHeatingOff) {
      digitalWrite(13, 1);
    }

    //При достижении указанной температуры отключить реле
    if (getTempFirstSensor() > tempOff) {
      digitalWrite(13, 1);
    }
  }
}
