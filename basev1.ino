#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h> // https://github.com/maniacbug/RF24

const uint64_t pipe = 0xF0F1F2F3F4LL; // индитификатор передачи, "труба"
const uint64_t pipe02 = 0xF0F1F2F3F1LL;
const uint64_t pipe03 = 0xF0F1F2F3F3LL;
RF24 radio(9, 10); // CE, CSN

void setup() {
  Serial.begin(9600);

  radio.begin();
  delay(2);
  radio.setChannel(9); // канал (0-127)

  // скорость, RF24_250KBPS, RF24_1MBPS или RF24_2MBPS
  // RF24_250KBPS на nRF24L01 (без +) неработает.
  // меньше скорость, выше чувствительность приемника.
  radio.setDataRate(RF24_1MBPS);

  // мощьность передатчика RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM,
  radio.setPALevel(RF24_PA_HIGH);
  radio.openReadingPipe(3, pipe03);
  radio.openReadingPipe(2, pipe02);
  radio.openReadingPipe(1, pipe); // открываем первую трубу с индитификатором "pipe"
  radio.startListening(); // включаем приемник, начинаем слушать трубу
}

//
// для ПроМини

void loop()
{
  int data[4];

  if (Serial.available() > 0) { // если чтото пришло в сериал
    int val = Serial.parseInt(); // читаем в переменную
    if (val == 1) { // если это 1
      Serial.println(1);
      radio.stopListening(); // останавливает приём (нужно перед началом передачи)
      radio.openWritingPipe(pipe); // открываем трубу на передачу.
      radio.write(&val, sizeof(val)); // отправляем val и указываем сколько байт пакет
      radio.startListening(); // включаем приемник, начинаем слушать трубу
    }
    if (val == 4) { // если это 1
      Serial.println(4);
      radio.stopListening(); // останавливает приём (нужно перед началом передачи)
      radio.openWritingPipe(pipe03); // открываем трубу на передачу.
      radio.write(&val, sizeof(val)); // отправляем val и указываем сколько байт пакет
      radio.startListening(); // включаем приемник, начинаем слушать трубу
    }
    if (val == 2) { // если это 1
      Serial.println(2);
      radio.stopListening(); // останавливает приём (нужно перед началом передачи)
      radio.openWritingPipe(pipe02); // открываем трубу на передачу.
      radio.write(&val, sizeof(val)); // отправляем val и указываем сколько байт пакет
      radio.startListening(); // включаем приемник, начинаем слушать трубу
    }
    if (val == 3) { // если это 1
      Serial.println(3);
      radio.stopListening(); // останавливает приём (нужно перед началом передачи)
      radio.openWritingPipe(pipe02); // открываем трубу на передачу.
      radio.write(&val, sizeof(val)); // отправляем val и указываем сколько байт пакет
      radio.startListening(); // включаем приемник, начинаем слушать трубу
    }
    Serial.flush(); // очищаем буфер
  }

  if (radio.available()) { // проверяем не пришло ли чего в буфер.
    radio.read(&data, sizeof(data)); // читаем данные и указываем сколько байт читать

    Serial.print("Humidity (%): ");
    Serial.println(data[1]);
    Serial.print("Temperature (oC): ");
    Serial.println(data[0]);
    Serial.print("TemperatureDS (oC): ");
    Serial.println(data[2]);
    Serial.print("zemla: ");
    Serial.println(data[3]);
  }

}
