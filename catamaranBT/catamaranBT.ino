//прошивка для набора с использованием HC-06


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


void setup() {
  Serial.begin(9600);
	
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  delay(1000);
}

void loop() {
	while (Serial.available())) {            
		
		
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