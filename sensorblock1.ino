#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h> // https://github.com/maniacbug/RF24
#include <microDS18B20.h>
#include <dht11.h>

const uint64_t pipe = 0xF0F1F2F3F3LL; // индитификатор передачи, "труба"

//MicroDS18B20<2> sensor1;
dht11 sensor;
RF24 radio(9, 10); // CE, CSN

void setup() {
  //  Serial.begin(9600);
 Serial.begin(9600);
  radio.begin();
  delay(2);
  radio.setChannel(9); // канал (0-127)

  // скорость, RF24_250KBPS, RF24_1MBPS или RF24_2MBPS
  // RF24_250KBPS на nRF24L01 (без +) неработает.
  // меньше скорость, выше чувствительность приемника.
  radio.setDataRate(RF24_1MBPS);

  // мощьность передатчика, RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM,
  radio.setPALevel(RF24_PA_HIGH);

  radio.openReadingPipe(3, pipe); // открываем первую трубу с индитификатором "pipe"
  radio.startListening(); // включаем приемник, начинаем слушать трубу

//sensor1.requestTemp();
sensor.read(8); 
 Serial.println(sensor.temperature);
 // Serial.println(sensor1.getTemp());
   Serial.println(analogRead(A7));
  //  radio.openWritingPipe(pipe); // открываем трубу на передачу.
Serial.println(1);
}

////// для UNO
void loop()
{
  int data[4];
 // sensor1.requestTemp();
  if (radio.available()) { // проверяем не пришло ли чего в буфер.
    Serial.println(2);
    int val;
    radio.read(&val, sizeof(val)); // читаем данные и указываем сколько байт читать

    if (val == 4) { // если пришло 1,
      Serial.println(4);
      radio.stopListening(); // останавливает приём (нужно перед началом передачи)

      sensor.read(8);  // читаем градусник
      delay(1000);
      data[0] = int(sensor.temperature); // заполняем массив
      data[1] = int(sensor.humidity);
      //data[2] = int(sensor1.getTemp());
      data[3] = int(analogRead(A7));
      //отправляем данные
      radio.openWritingPipe(pipe); // открываем трубу на передачу.
      radio.write(&data, sizeof(data)); // отправляем данные и указываем сколько байт пакет
      radio.startListening(); // включаем приемник, начинаем слушать трубу
    }
  }
}
