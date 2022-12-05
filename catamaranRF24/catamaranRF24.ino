//прошивка для набора с использованием nRF24L01
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define enA 3
#define in1 2
#define in2 4
#define in3 6
#define in4 7
#define enB 5

int vY, hX;
int leftMot, rightMot;

int in_data[2] = {512, 512}; // X Y
byte pipeNo;

RF24 radio(9, 10); // "создать" модуль на пинах 9 и 10 Для Уно

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб


void setup() {
  //Serial.begin(9600); //открываем порт для связи с ПК
	
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(6);     //размер пакета, в байтах

  radio.openReadingPipe(1, address[0]);     //хотим слушать трубу 0
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp(); //начать работу
  radio.startListening();  //начинаем слушать эфир, мы приёмный модуль
}

void loop() {
	while ( radio.available(&pipeNo)) {             // слушаем эфир со всех труб
		radio.read( &in_data, sizeof(in_data) );      // читаем входящий сигнал
		// in_data[0] in_data[1];  
		
		if (in_data[1] >= 562){ // если вперед
			forward();
			vY = map(in_data[1], 562,1024, 0, 255);
			if (in_data[0] >= 562) toRight(); 		// вправо
			else if (in_data[0] <= 462) toLeft(); 	// влево
			else {									// только вперед
				analogWrite(enA,vY);
				analogWrite(enB,vY);
			}
		}
		else if (in_data[1] <= 462){ // если назад
			backward();
			vY = map(in_data[1], 462,0, 0, 255);
			if (in_data[0] >= 562) toRight();		// вправо
			else if (in_data[0] <= 462) toLeft();	// влево
			else {									// только назад
				analogWrite(enA,vY);
				analogWrite(enB,vY);
			}
		}
		else {										// на месте
			if (in_data[0] >= 562) {				// разворот вправо
				fullRight();
				hX = map(in_data[0], 562, 1024, 0, 255);
				analogWrite(enA,vY);
				analogWrite(enB,vY);
			}
			else if (in_data[0] <= 462) {			// разворот влево
				fullLeft();
				hX = map(in_data[0], 462, 0, 0, 255);
				analogWrite(enA,vY);
				analogWrite(enB,vY);
			}
			else stop();							// если значения ХУ в мертвой зоне
		}
	}
}




void stop(){
	digitalWrite(in1,LOW);
	digitalWrite(in2,LOW);
	digitalWrite(in3,LOW);
	digitalWrite(in4,LOW);
}
void forward(){
	digitalWrite(in1, HIGH);
	digitalWrite(in3, HIGH);
	   
	digitalWrite(in2, LOW);
	digitalWrite(in4, LOW);
}
void backward(){
	digitalWrite(in1, LOW);
	digitalWrite(in3, LOW);
	   
	digitalWrite(in2, HIGH);
	digitalWrite(in4, HIGH);
}

void toRight(){
	hX = map(in_data[0], 562, 1024, 0, 255);
	leftMot = vY + hX;
	rightMot = vY - hX;
	
	if (leftMot > 255) leftMot = 255;
	if (rightMot < 0) rightMot = 0;
	
	analogWrite(enA, leftMot);
	analogWrite(enB, rightMot);
}

void toLeft(){
	hX = map(in_data[0], 462, 0, 0, 255);
	leftMot = vY - hX;
	rightMot = vY + hX;
	
	if (leftMot < 0) leftMot = 0;
	if (rightMot > 255) rightMot = 255;
	
	analogWrite(enA, leftMot);
	analogWrite(enB, rightMot);
}

void fullRight(){
	digitalWrite(in1, HIGH);
	digitalWrite(in3, LOW);
	   
	digitalWrite(in2, LOW);
	digitalWrite(in4, HIGH);
}
void fullLeft(){
	digitalWrite(in1, LOW);
	digitalWrite(in3, HIGH);
	
	digitalWrite(in2, HIGH);
	digitalWrite(in4, LOW);
}