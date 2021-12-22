
// Definimos as portas que estaramos usando durante o desenvolvimento
int LO1 = 35; //A porta com LO+, no nosso caso 35
int LO2 = 32; 	//A porta com LO-, no nosso caso 32
int OUTPUT = A5; //A porta com o OUTPUT, no nosso caso a A5


void setup() 
{
	Serial.begin(9600); // Inicia a comunicação Serial em 9600 bits/segundo
	pinMode(LO1, INPUT); //Definimos que ambos LO+ e LO- são entradas
	pinMode(LO2, INPUT); 
}

void loop() 
{
	if((digitalRead(LO1) == 1)||(digitalRead(LO2) == 1))
	{
		Serial.println('!');
	}
	else
	{
		Serial.println(analogRead(OUTPUT));
	}
	delay(1);
}