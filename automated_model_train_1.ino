/*
 * https://youtu.be/PoOxFS2DNpI
 * https://github.com/DIYandDigitalRR/arduino-automated-railroad
 * 
 * adapted for test by niq_ro (Nicu FLORICA)
 */

int station1;
int station2;
int station3;
int Speed = 150;
int Direction;
int speedval;

#define sens1 5
#define sens2 6
#define comanda 3

void setup() {
Serial.begin(9600);
pinMode(sens1,OUTPUT);
pinMode(sens2,OUTPUT);
}

void loop() {
  station1=analogRead(A0);
  station2=analogRead(A1);
//  station3=analogRead(A2);
//  speedval=analogRead(A3);
//  Speed=map(speedval,0,1023,0,255);
  analogWrite(comanda,Speed);
  Serial.println(Direction);
  Serial.println(Speed);
  delay(200);


if (station1<500){
  digitalWrite(sens1,LOW);
  digitalWrite(sens2,LOW);
  Direction=0;
  delay(5000);
  digitalWrite(sens1,HIGH);
  digitalWrite(sens2,LOW);

  delay(1500);
  }
if (station2<500){
 digitalWrite(sens1,LOW);
 digitalWrite(sens2,LOW);
  Direction=1;
  delay(5000);
  digitalWrite(sens1,LOW);
  digitalWrite(sens2,HIGH);


  delay(1500);
}  
/*
if ((station3<500) && (Direction==1)){
  digitalWrite(sens1,LOW);
 digitalWrite(sens2,LOW);
  delay(5000);
  digitalWrite(sens1,LOW);
  digitalWrite(sens2,HIGH);
  delay(1500);
}
if ((station3<500) && (Direction==0)){
 digitalWrite(sens1,LOW);
 digitalWrite(sens2,LOW);
 delay(5000);
 digitalWrite(sens1,HIGH);
 digitalWrite(sens2,LOW);
 delay(1500);
 }
*/
} // end main loop
