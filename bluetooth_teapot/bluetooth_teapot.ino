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

void setup() {

  Serial.begin(9600);

  // Start up the library
  sensors.begin();
  sensors2.begin();

  digitalWrite(13, 1);

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


//Функция термоконтроля
void tempControl() {
  //Таймер опроса каждую секунду
  if (millis() - counterPeriodTemp > 1000) {
    counterPeriodTemp = millis();

    // Описание на англиском сохранено как в стандартном примере для подключения данных датчиков
    // call sensors.requestTemperatures() to issue a global temperature
    // request to all devices on the bus
    sensors.requestTemperatures(); // Send the command to get temperatures
    sensors2.requestTemperatures(); // Send the command to get temperatures
    // After we got the temperatures, we can print them here.
    // We use the function ByIndex, and as an example get the temperature from the first sensor only.
    float tempC = sensors.getTempCByIndex(0);
    float tempC2 = sensors2.getTempCByIndex(0);

    // Отправка первого параметра при успешном чтении
    // Check if reading was successful
    if (tempC != DEVICE_DISCONNECTED_C) {
      Serial.print(tempC);
    } else {
      Serial.print(0);
    }

    Serial.print("A");  // Символ, который определяет где кончается первый параметр

    // Отправка второго параметра при успешном чтении
    if (tempC2 != DEVICE_DISCONNECTED_C) {
      Serial.print(tempC2);
    } else {
      Serial.print(0);
    }

    Serial.print("B");  // Символ, который определяет где кончается второй параметр

    // Отправка состояния реле.
    // Перенос строки определяет конец массива байтов и показывает где кончает данных параметр
    Serial.println(digitalRead(13));
    
    //При достижении указанной температуры отключить реле
    if (tempC > tempOff) {
      digitalWrite(13, 1);
    }
  }
}
