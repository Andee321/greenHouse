#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h> // https://github.com/maniacbug/RF24

const uint64_t pipe = 0xF0F1F2F3F1LL; // индитификатор передачи, "труба"

RF24 radio(9, 10); // CE, CSN

bool rele1 = 0;
bool rele2 = 0;

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
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

  radio.openReadingPipe(1, pipe); // открываем первую трубу с индитификатором "pipe"
  radio.startListening(); // включаем приемник, начинаем слушать трубу
}

void loop() {
  if (radio.available()) { // проверяем не пришло ли чего в буфер.
    int val;
    radio.read(&val, sizeof(val)); // читаем данные и указываем сколько байт читать
    if (val == 2) { // если пришло 2,
      rele1 = !rele1;
      Serial.println( rele1);
    }
    if (val == 3) { // если пришло 2,
      rele2 = !rele2;
    }
    digitalWrite(2, rele1);
    digitalWrite(3, rele2);
  }

}
