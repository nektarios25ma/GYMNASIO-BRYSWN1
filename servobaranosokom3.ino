//ΤΟ ΠΡΟΓΡΑΜΜΑ ΤΗΣ ΜΠΑΡΑΣ ΤΟΥ ΝΟΣΟΚΟΜΕΙΟΥ , ΤΗΣ ΟΘΟΝΗΣ LCD ΚΑΙ ΤΗΣ ΣΤΑΔΙΑΚΗΣ ΦΩΤΕΙΝΟΤΗΤΑΣ ΤΗΣ ΠΟΛΗΣ(ΟΣΟ ΠΕΡΙΣΣΟΤΕΡΟ ΣΚΟΤΑΔΙ ΤΟΣΟ ΠΙΟ ΠΟΛΥ ΦΩΤΙΖΟΥΝ) 
// I2C LCD screen demo
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16 ,2); 
#define echo 12
#define trigger 13
#include <Servo.h> // Συμπερίλαβε τη βιβλιοθήκη του σέρβο
Servo myservo; // Δημιούργησε ένα αντικείμενο τύπου Servo
int s=1; // Το βήμα (μοίρες) που θα αυξάνουμε τη θέση του σέρβο σε κάθε επανάληψη
//const int LDR=A0; //ΕΔΩ ΣΥΝΔΕΩ ΤΗΝ ΦΩΤΟΑΝΤΙΣΤΑΣΗ LDR ΓΙΑ ΝΑ ΔΙΑΒΑΖΩ ΤΟ ΕΠΙΠΕΔΟ ΦΩΤΕΙΝΟΤΗΤΑΣ
int LDR;
int input=0;
int brightness=0;
float hum;  //Stores humidity value,ΤΙΜΗ ΥΓΡΑΣΙΑΣ
float temp; //Stores temperature value,ΤΙΜΗ ΘΕΡΜΟΚΡΑΣΙΑΣ
const int CityLIGHTSA=5;
const int CityLIGHTSB=3;
unsigned long start_time; // Δημιούργησε μια μεταβλητή τύπου unsigned long integer με όνομα start_time
#include <DHT.h>; //ΒΙΒΛΙΟΘΗΚΗ ΑΙΣΘΗΤΗΡΑ DHT
//Constants
#define DHTPIN 4     // what pin we're connected to ,ΤΟ ΠΙΝ ΠΟΥ ΣΥΝΔΕΟΥΜΕ ΤΟΝ ΑΙΣΘΗΤΗΡΑ ΘΕΡΜΟΚΡΑΣΙΑΣ - ΥΓΡΑΣΙΑΣ
#define DHTTYPE DHT22   // DHT 22  (AM2302),ΤΥΠΟΣ ΑΙΣΘΗΤΗΡΑ,ΔΙΑΛΕΞΑΜΕ DHT22 ΩΣ ΠΙΟ ΑΞΙΟΠΙΣΤΟ
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
//Variables

int red1=2;
int blue1=6;

#include "SoftwareSerial.h"
SoftwareSerial mySerial(10, 11);
# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]

# define ACTIVATED LOW

int buttonPause = 3;

long TP_init()   //ΤΟ CONTROL-ΔΕΚΤΗΣ ΠΟΥ ΘΑ ΕΛΕΓΧΕΙ ΤΗΝ ΠΑΡΑΛΑΒΗ Ή ΟΧΙ ΤΟΥ ΣΗΜΑΤΟΣ-ΠΟΙΟ ΚΑΤΩ ΦΑΙΝΕΤΑΙ ΟΤΑΝ ΧΡΗΣΙΜΟΠΟΙΗΤΑΙ
{
 digitalWrite(trigger,LOW);
  delayMicroseconds(2);
  digitalWrite(trigger,HIGH);
  delayMicroseconds(2);
  digitalWrite(trigger,LOW);
  delayMicroseconds(2);
  long microseconds=pulseIn(echo,HIGH,100000);
  return microseconds ;
   
}

void setup()
{
 pinMode(buttonPause, INPUT);
digitalWrite(buttonPause,HIGH);

mySerial.begin (9600);
delay(1000);
  Serial.begin(9600);
pinMode(CityLIGHTSA,OUTPUT);  //LIGHTSA AND B  ΣΤΑΔΙΑΚΟΣ ΦΩΤΙΣΜΟΣ ΠΟΛΗΣ
pinMode(CityLIGHTSB,OUTPUT);
pinMode(red1, OUTPUT); 
pinMode(blue1, OUTPUT);
pinMode(trigger, OUTPUT);
pinMode(echo, INPUT);

lcd.init(); // initialize the lcd
// Print a message to the LCD.
lcd.backlight();
lcd.setCursor(0,0);
myservo.attach(9); // Σχετίζει το σερβο με το πιν 9
LDR=analogRead(A0);
myservo.write(0); // Γύρισε τον άξονα του servo στην τιμή 0
delay(35); // Περίμενε λίγο για να γυρίσει το servo στην τιμή που θέλουμε
dht.begin(); //ΘΕΡΜΟΜΕΤΡΟ DHT,ΞΕΚΙΝΑ.
digitalWrite(CityLIGHTSA,LOW);
digitalWrite(CityLIGHTSB,LOW);
}
void loop()
{ 
  
//Read data and store it to variables hum and temp,ΔΙΑΒΑΖΩ ΜΕΤΡΗΣΕΙΣ ΚΑΙ ΤΙΣ ΑΠΟΘΗΚΕΥΩ ΣΤΙΣ ΜΕΤΑΒΛΗΤΕΣ hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
lcd.setCursor(0,0); // Η ΟΘΟΝΗ LCD ΘΑ ΔΕΙΞΕΙ ΘΕΡΜΟΚΡΑΣΙΑ ΚΑΙ ΥΓΡΑΣΙΑ % ΣΤΗΝ ΠΡΩΤΗ ΓΡΑΜΜΗ
  lcd.print( temp );
   lcd.print( "C ");
lcd.setCursor(7,0);
lcd.print( hum );
lcd.print(" % ");         
 lcd.setCursor(0,1);
lcd.print("GYMNASIO BRYSWN ");   
  myservo.write(0); // Γύρισε τον άξονα του servo στην τιμή 0
delay(1500); // Περίμενε λίγο για να γυρίσει το servo στην τιμή που θέλουμε
if ( TP_init()>=10) {
start_time=millis(); //ΑΡΧΙΖΩ ΤΟ ΧΡΟΝΟΜΕΤΡΟ 
while (millis() - start_time < 40000) { // Εφόσον δεν πέρασαν 40 δευτ κάνε:
  myservo.write(110); // Γύρισε τον άξονα του servo στην τιμή 110-άνοιξε την μπάρα
delay(20); 
//Read data and store it to variables hum and temp,ΔΙΑΒΑΖΩ ΜΕΤΡΗΣΕΙΣ ΚΑΙ ΤΙΣ ΑΠΟΘΗΚΕΥΩ ΣΤΙΣ ΜΕΤΑΒΛΗΤΕΣ hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();

   input = analogRead(LDR);
 brightness = map(input,520,1023,0,255);  //ΣΤΑΔΙΑΚΟΣ ΦΩΤΙΣΜΟΣ ΠΟΛΗΣ ΑΝΤΙΣΤΡΟΦΩΣ ΑΝΑΛΟΓΗ ΤΗΣ ΕΞΩΤΕΡΙΚΗΣ ΦΩΤΕΙΝΟΤΗΤΑΣ
 analogWrite(CityLIGHTSA,brightness);
 analogWrite(CityLIGHTSB,brightness);
 Serial.println(TP_init()); 


lcd.setCursor(0,0);
lcd.print("AMBULANCE DETECT");
lcd.setCursor(0,1);
lcd.print(" DOORS OPEN!!! ");  
if (temp<14.00)
{
  lcd.setCursor(0,1);
lcd.print(" COLD ,PROTECT!!!");
playSecond();
  digitalWrite(blue1,HIGH);
  delay(50);
playFifth();
  
}
if (temp>28.00)
{
  lcd.setCursor(0,1);
lcd.print("HOT CLIMATE,PROTECT");
playFourth();
digitalWrite(red1,HIGH);
  delay(50);
 playThird();
}
}}
if ( TP_init()<=10) {
start_time=millis(); //ΑΡΧΙΖΩ ΤΟ ΧΡΟΝΟΜΕΤΡΟ 
while (millis() - start_time < 2500) { // Εφόσον δεν πέρασαν 40 δευτ κάνε:
  myservo.write(0); // Γύρισε τον άξονα του servo στην τιμή 0-κλεισε την μπάρα
delay(20); 
//Read data and store it to variables hum and temp,ΔΙΑΒΑΖΩ ΜΕΤΡΗΣΕΙΣ ΚΑΙ ΤΙΣ ΑΠΟΘΗΚΕΥΩ ΣΤΙΣ ΜΕΤΑΒΛΗΤΕΣ hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();

   input = analogRead(LDR);
 brightness = map(input,400,1023,0,255);  //ΣΤΑΔΙΑΚΟΣ ΦΩΤΙΣΜΟΣ ΠΟΛΗΣ ΑΝΤΙΣΤΡΟΦΩΣ ΑΝΑΛΟΓΗ ΤΗΣ ΕΞΩΤΕΡΙΚΗΣ ΦΩΤΕΙΝΟΤΗΤΑΣ
 analogWrite(CityLIGHTSA,brightness);
 analogWrite(CityLIGHTSB,brightness);
 Serial.println(TP_init()); 
lcd.setCursor(0,0);
lcd.print(" NO AMBULANCE!   ");
lcd.setCursor(0,1);
lcd.print(" DOORS CLOSED    ");  

if (temp<14.00)
{
 lcd.setCursor(0,1);
lcd.print(" COLD ,PROTECT!!!");
playSecond();
  digitalWrite(blue1,HIGH);
  delay(50);
playFifth(); 
}
if (temp>28.00)
{
 lcd.setCursor(0,1);
lcd.print("HOT CLIMATE,PROTECT");
playFourth();
digitalWrite(red1,HIGH);
  delay(50);
 playThird();   
}
}
}
// τα παρακατω ειναι για τον έλεγχο παραλαβης υπερηχου,ldr,φωτεινοτητας
Serial.println(TP_init());

Serial.println(input);
 Serial.println(brightness);
 Serial.print(temp); Serial.print(" C ");
  
}

void playFirst()
{
  execute_CMD(0x3F, 0, 0);
  delay(2500);
  setVolume(30);
  delay(500);
 
}
void playFifth() //κατευθυνθητε σε θερμρνομενους χωρους
{
  execute_CMD(0x03, 0, 4);
  delay(5500);
  setVolume(30);
  delay(50);
 
}
void playSecond() //ΠΡΟΣΟΧΗ ΧΑΜΗΛΗ ΘΕΡΜΟΚΡΑΣΙΑ
{
  execute_CMD(0x03, 0, 1);
  delay(4000);
  setVolume(30);
  delay(50);
 
}
void playThird() //ΚΑΤΕΥΘΥΝΘΗΤΕ ΣΕ ΚΛΙΜΑΤΙΖΟΜΕΝΟΥΣ ΧΩΡΟΥΣ
{
  execute_CMD(0x03, 0, 2);
  delay(5500);
  setVolume(30);
  delay(50);
 
}
void playFourth() //ΠΡΟΣΟΧΗ ΥΨΗΛΗ ΘΕΡΜΟΚΡΑΣΙΑ
{
  execute_CMD(0x03, 0, 3);
  delay(4000);
  setVolume(30);
  delay(50);
 
}
void pause()
{
  execute_CMD(0x0E,0,0);
  delay(500);
}

void play()
{
  execute_CMD(0x0D,0,1); 
  delay(500);
}



void setVolume(int volume)
{
  execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
  delay(1000);
}

void execute_CMD(byte CMD, byte Par1, byte Par2)
// Excecute the command and parameters
{
// Calculate the checksum (2 bytes)
word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
// Build the command line
byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
//Send the command line to the module
for (byte k=0; k<10; k++)
{
mySerial.write( Command_line[k]);
}
}

