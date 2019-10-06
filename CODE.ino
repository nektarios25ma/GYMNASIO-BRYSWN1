// I2C LCD screen demo
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,20,4); //ΟΘΟΝΗ 20Χ4
char screen[80]; // screen buffer
int col = 0;     // 0 to 19 column number for screen 1
unsigned long start_time; // Δημιούργησε μια μεταβλητή τύπου unsigned long integer με όνομα start_time
//Libraries
#include <DHT.h>; //ΒΙΒΛΙΟΘΗΚΗ ΑΙΣΘΗΤΗΡΑ DHT

//Constants
#define DHTPIN 7     // what pin we're connected to ,ΤΟ ΠΙΝ ΠΟΥ ΣΥΝΔΕΟΥΜΕ ΤΟΝ ΑΙΣΘΗΤΗΡΑ ΘΕΡΜΟΚΡΑΣΙΑΣ - ΥΓΡΑΣΙΑΣ
#define DHTTYPE DHT22   // DHT 22  (AM2302),ΤΥΠΟΣ ΑΙΣΘΗΤΗΡΑ,ΔΙΑΛΕΞΑΜΕ DHT22 ΩΣ ΠΙΟ ΑΞΙΟΠΙΣΤΟ
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
//Variables

float hum;  //Stores humidity value,ΤΙΜΗ ΥΓΡΑΣΙΑΣ
float temp; //Stores temperature value,ΤΙΜΗ ΘΕΡΜΟΚΡΑΣΙΑΣ
int lightPinA = 13; // choose the pin for the LightA -RELAY POSITION A,ΘΕΣΗ Α -ΠΙΝ ΠΟΥ ΣΥΝΔΕΕΤΑΙ ΜΕ ΤΟ ΡΕΛΕ ΓΙΑ ΤΗΝ ΕΝΕΡΓΟΠΟΙΗΣΗ ΤΟΥ ΑΝΤΙΣΤΟΙΧΟΥ ΦΩΤΟΣ(Α)
int inputPinpirA = 2; // choose the input pin (for PIR sensorA),ΕΔΩ Ο ΑΙΣΘΗΤΗΡΑΣ Α ΔΙΝΕΙ ΣΗΜΑ ΟΤΑΝ ΑΝΙΧΝΕΥΕΙ ΚΙΝΗΣΗ
int inputPinpirLAB = 6; // choose the input pin (for PIR LAB),ΕΔΩ Ο ΑΙΣΘΗΤΗΡΑΣ LAB ΔΙΝΕΙ ΣΗΜΑ ΟΤΑΝ ΑΝΙΧΝΕΥΕΙ ΚΙΝΗΣΗ
int pirStateA = LOW; // we start, assuming no motionAdetected to A SENSOR,ΥΠΟΘΕΤΩ ΑΡΧΙΚΑ ΟΤΙ ΔΕΝ ΕΧΩ ΚΙΝΗΣΗ ΣΤΟΝ Α ΑΣΘΗΤΗΡΑ
int valA = 0; // variable for reading the pinA status,ΜΕΤΑΒΛΗΤΗ ΠΟΥ ΔΙΑΒΑΖΕΙ ΤΟΝ Α ΑΙΣΘΗΤΗΡΑ
int valLAB = 0; // variable for reading the pinLAB status,ΜΕΤΑΒΛΗΤΗ ΠΟΥ ΔΙΑΒΑΖΕΙ ΤΟΝ LAB ΑΙΣΘΗΤΗΡΑ
int lightPinB = 12; // choose the pin for the LightB -RELAY POSITION B,ΘΕΣΗ Β -ΠΙΝ ΠΟΥ ΣΥΝΔΕΕΤΑΙ ΜΕ ΤΟ ΡΕΛΕ ΓΙΑ ΤΗΝ ΕΝΕΡΓΟΠΟΙΗΣΗ ΤΟΥ ΑΝΤΙΣΤΟΙΧΟΥ ΦΩΤΟΣ(Β)
int inputPinpirB = 3; // choose the input pin (for PIR sensorB),ΕΔΩ Ο ΑΙΣΘΗΤΗΡΑΣ Β ΔΙΝΕΙ ΣΗΜΑ ΟΤΑΝ ΑΝΙΧΝΕΥΕΙ ΚΙΝΗΣΗ
int pirStateB = LOW; // we start, assuming no motionBdetected,ΥΠΟΘΕΤΩ ΑΡΧΙΚΑ ΟΤΙ ΔΕΝ ΕΧΩ ΚΙΝΗΣΗ ΣΤΟΝ Β ΑΣΘΗΤΗΡΑ
int valB = 0; // variable for reading the pinB status,ΜΕΤΑΒΛΗΤΗ ΠΟΥ ΔΙΑΒΑΖΕΙ ΤΟΝ B ΑΙΣΘΗΤΗΡΑ
const int LDR=A0; //ΕΔΩ ΣΥΝΔΕΩ ΤΗΝ ΦΩΤΟΑΝΤΙΣΤΑΣΗ LDR ΓΙΑ ΝΑ ΔΙΑΒΑΖΩ ΤΟ ΕΠΙΠΕΔΟ ΦΩΤΕΙΝΟΤΗΤΑΣ
int input=0;
int brightness=0;
const int ControlLIGHTSA=5; //ΕΔΩ ΣΥΝΔΕΩ ΤΑ LED ΥΨΗΛΗΣ ΦΩΤΕΙΝΟΤΗΤΑΣ ΠΟΥ ΕΝΕΡΓΟΠΟΙΟΥΝΤΑΙ ΑΝΑΛΟΓΑ ΜΕ ΤΟ ΕΠΙΠΕΔΟ ΣΚΟΤΑΔΙΟΥ,PWM PIN
const int ControlLIGHTSB=9; //ΕΔΩ ΣΥΝΔΕΩ ΤΑ LED ΥΨΗΛΗΣ ΦΩΤΕΙΝΟΤΗΤΑΣ ΠΟΥ ΕΝΕΡΓΟΠΟΙΟΥΝΤΑΙ ΑΝΑΛΟΓΑ ΜΕ ΤΟ ΕΠΙΠΕΔΟ ΣΚΟΤΑΔΙΟΥ,PIN PWM

void setup() {
pinMode(lightPinA, OUTPUT); // declare LightA as output,ΕΔΩ ΟΡΙΖΕΤΑΙ ΩΣ ΕΞΟΔΟΣ ΤΟ ΠΙΝ 13
pinMode(inputPinpirA, INPUT); // declare sensorA as input,ΕΔΩ ΟΡΙΖΕΤΑΙ ΩΣ ΕΙΣΟΔΟΣ ΤΟ ΣΗΜΑ ΤΟΥ ΑΙΣΘΗΤΗΡΑ PIR A
pinMode(lightPinB, OUTPUT); // declare LightB as output,ΕΔΩ ΟΡΙΖΕΤΑΙ ΩΣ ΕΞΟΔΟΣ ΤΟ ΠΙΝ 12
pinMode(inputPinpirLAB, INPUT); //ΕΔΩ ΟΡΙΖΕΤΑΙ ΩΣ ΕΙΣΟΔΟΣ ΤΟ ΣΗΜΑ ΤΟΥ ΑΙΣΘΗΤΗΡΑ PIR LAB
pinMode(inputPinpirB, INPUT); // declare sensorB as input,ΕΔΩ ΟΡΙΖΕΤΑΙ ΩΣ ΕΙΣΟΔΟΣ ΤΟ ΣΗΜΑ ΤΟΥ ΑΙΣΘΗΤΗΡΑ PIR B
pinMode(ControlLIGHTSA,OUTPUT); //ΕΔΩ ΟΡΙΖΕΤΑΙ ΩΣ ΕΞΟΔΟΣ ΤΟ ΠΙΝ 5,PWM PIN
pinMode(ControlLIGHTSB,OUTPUT); //ΕΔΩ ΟΡΙΖΕΤΑΙ ΩΣ ΕΞΟΔΟΣ ΤΟ ΠΙΝ 9,PWM PIN
 lcd.init(); // ΑΡΧΙΚΟΠΟΙΗΣΗ ΟΘΟΝΗΣ
  lcd.backlight();
  lcd.clear(); // clear screen ,ΚΑΘΑΡΙΣΕ ΟΘΟΝΗ
Serial.begin(9600); //ΣΕΙΡΙΑΚΗ ΕΠΙΚΟΙΝΩΝΙΑ..
dht.begin(); //ΘΕΡΜΟΜΕΤΡΟ DHT,ΞΕΚΙΝΑ..
}
void loop(){
//Read data and store it to variables hum and temp,ΔΙΑΒΑΖΩ ΜΕΤΡΗΣΕΙΣ ΚΑΙ ΤΙΣ ΑΠΟΘΗΚΕΥΩ ΣΤΙΣ ΜΕΤΑΒΛΗΤΕΣ hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
     
lcd.setCursor(0,0); // Η ΟΘΟΝΗ LCD ΘΑ ΔΕΙΞΕΙ ΘΕΡΜΟΚΡΑΣΙΑ ΚΑΙ ΥΓΡΑΣΙΑ % ΣΤΗΝ ΠΡΩΤΗ ΓΡΑΜΜΗ
  lcd.print( temp );
   lcd.print( "C ");
lcd.setCursor(7,0);
lcd.print( hum );
lcd.print(" % ");    
  start_time=millis(); //ΑΡΧΙΖΩ ΤΟ ΧΡΟΝΟΜΕΤΡΟ 
while (millis() - start_time < 100000) { // Εφόσον δεν πέρασαν 100 δευτ κάνε:   (100 ΔΕΥΤΕΡΟΛΕΠΤΑ ΕΙΝΑΙ Η ΧΡΟΝΟΚΑΘΥΣΤΕΡΗΣΗ ΠΟΥ ΔΙΝΟΥΜΕ ΑΠΟ ΟΤΑΝ ΘΑ ΑΝΙΧΝΕΥΤΕΙ ΚΙΝΗΣΗ ΜΕΧΡΙ ΝΑ ΔΩΣΕΙ ΕΝΤΟΛΗ ΝΑ ΣΒΗΣΕΙ ΤΟ ΦΩΣ)
 
valLAB = digitalRead(inputPinpirLAB); // read inputA val,ΔΙΑΒΑΣΕ ΤΗΝ ΕΝΔΕΙΞΗ ΤΟΥ ΑΙΣΘΗΤΗΡΑ-PIR LAB
if (valLAB == HIGH) {   // ΣΤΑΔΙΑΚΟΣ ΦΩΤΙΜΟΣ ΕΡΓΑΣΤΗΡΙΑΚΟΥ ΠΙΝΑΚΑ ΕΛΕΓΧΟΥ ΕΑΝ ΑΝΙΧΝΕΥΤΕΙ ΚΙΝΗΣΗ ΑΠΟ ΤΟΝ ΑΙΣΘΗΤΗΡΑ PIR ΤΟΥ ΕΡΓΑΣΤΗΡΙΟΥ
    input = analogRead(LDR); //ΔΙΑΒΑΖΩ ΤΗΝ ΦΩΤΟΑΝΤΙΣΤΑΣΗ LDR 0-1023 ΤΙΜΕΣ , ΜΕ ΜΕΓΑΛΕΣ ΤΙΜΕΣ ΝΑ ΔΙΝΕΙ ΣΤΟ ΣΚΟΤΑΔΙ 
 brightness = map(input,480,1023,0,255); //ΔΗΜΙΟΥΡΓΩ ΕΝΑ ΧΑΡΤΗ ΠΟΥ ΑΝΤΙΣΤΟΙΧΕΙ ΤΗΝ ΣΚΟΤΕΙΝΟΤΗΤΑ ΠΟΥ ΔΙΑΒΑΖΕΙ ΑΠΟ ΤΗΝ LDR ΜΕ ΤΗΝ ΦΩΤΕΙΝΟΤΗΤΑ-brightness ΠΟΥ ΧΕΙ ΜΕΓΙΣΤΗ ΤΙΜΗ 255
 analogWrite(ControlLIGHTSA,brightness); // ΕΔΩ ΣΤΑ LED ΥΨΗΛΗΣ ΦΩΤΕΙΝΟΤΗΤΑΣ ΔΙΝΩ ΕΝΤΟΛΗ ΝΑ ΑΝΑΨΟΥΝ ΑΝΑΛΟΓΑ ΜΕ ΤΗΝ brightness
 analogWrite(ControlLIGHTSB,brightness); // ΕΔΩ ΣΤΑ LED ΥΨΗΛΗΣ ΦΩΤΕΙΝΟΤΗΤΑΣ ΔΙΝΩ ΕΝΤΟΛΗ ΝΑ ΑΝΑΨΟΥΝ ΑΝΑΛΟΓΑ ΜΕ ΤΗΝ brightness
 } else {digitalWrite(ControlLIGHTSA,LOW); //ΑΛΛΙΩΣ ΣΒΗΣΕ ΤΑ ΦΩΤΑΚΙΑ ΤΟΥ CONTROL PANEL(ΜΕΣΑ ΣΤΟ ΕΡΓΑΣΤΗΡΙΟ)
 digitalWrite(ControlLIGHTSB,LOW);} 
 
valA = digitalRead(inputPinpirA); // read inputA value,ΔΙΑΒΑΣΕ ΤΗΝ ΕΝΔΕΙΞΗ ΤΟΥ ΑΙΣΘΗΤΗΡΑ-PIR Α
valB = digitalRead(inputPinpirB); // read inputB value,ΔΙΑΒΑΣΕ ΤΗΝ ΕΝΔΕΙΞΗ ΤΟΥ ΑΙΣΘΗΤΗΡΑ-PIR B
  
if (valA == HIGH) { // check if the inputA is HIGH,ΕΑΝ ΑΝΙΧΝΕΥΤΕΙ ΚΙΝΗΣΗ ΑΠΟ ΤΟΝ ΑΙΣΘΗΤΗΡΑ Α ...
digitalWrite(lightPinA, HIGH);// turn LightA ON,ΑΝΑΨΕ ΤΑ ΦΩΤΑ Α ΜΕΣΩ ΤΟΥ ΡΕΛΕ
if (pirStateA == LOW) {
// we have just turned on A

lcd.setCursor(0,1); //ΓΡΑΨΕ ΣΤΗΝ 2η ΓΡΑΜΜΗ ΚΙΝΗΣΗ ΣΤΟΝ Α ΔΙΑΔΡΟΜΟ
lcd.print(" MotionA detected   ");

pirStateA = HIGH;  //ΔΗΛΩΝΩ ΤΟΤΕ ΤΗΝ ΚΑΤΑΣΤΑΣΗ ΤΟΥ Α ΩΣ HIGH
}   //ΑΛΛΙΩΣ ΕΑΝ ΔΕΝ ΑΝΙΧΝΕΥΤΕΙ ΚΙΝΗΣΗ.....
} else {                   ////ΑΛΛΙΩΣ ΕΑΝ ΔΕΝ ΑΝΙΧΝΕΥΤΕΙ ΚΙΝΗΣΗ.....

if (pirStateA == HIGH){


lcd.setCursor(0,1);  //ΓΡΑΨΕ ΣΤΗΝ 2η ΓΡΑΜΜΗ Η ΚΙΝΗΣΗ ΣΤΟΝ Α ΔΙΑΔΡΟΜΟ ΤΕΡΜΑΤΙΣΤΗΚΕ ( Η ΟΘΟΝΗ ΔΕΙΧΝΕΙ REAL TIME ΤΗΝ ΚΙΝΗΣΗ Ή ΟΧΙ ΣΤΗΝ ΕΜΒΕΛΕΙΑ ΤΩΝ ΑΙΣΘΗΤΗΡΩΝ)
lcd.println(" MotionA ended      ");

pirStateA = LOW;  //ΤΟΤΕ ΔΗΛΩΣΕ ΟΤΙ Η ΚΑΤΑΣΤΑΣΗ ΤΟΥ Α ΕΙΝΑΙ LOW 
}
}  //ΟΜΟΙΑ ΔΙΑΔΙΚΑΣΙΑ ΓΙΑ ΤΟΝ ΑΙΣΘΗΤΗΡΑ Β
if (valB == HIGH) { 
digitalWrite(lightPinB, HIGH);
if (pirStateB == LOW) {


lcd.setCursor(0,2);
lcd.print(" MotionB detected   ");

pirStateB = HIGH;
}
} else {

if (pirStateB == HIGH){


lcd.setCursor(0,2);
lcd.print(" MotionB ended     ");

pirStateB = LOW;
}
}
}

 
 lcd.setCursor(0,3);
lcd.print("   GYMNASIO BRYSWN");   
//ΕΑΝ Η ΚΑΤΑΣΤΑΣΗ ΤΩΝ ΑΙΣΘΗΤΗΡΩΝ Α ΚΑΙ Β ΕΙΝΑΙ LOW-ΔΕΝ ΑΝΙΧΝΕΥΕΤΑΙ ΚΙΝΗΣΗ ΣΒΗΣΕ ΤΑ ΦΩΤΑ(ΔΩΕΣΕ ΣΤΟ ΡΕΛΕ ΕΝΤΟΛΗ ΝΑ ΣΒΗΣΟΥΝ ΤΑ ΦΩΤΑ)ΕΑΝ ΕΧΕΙ ΠΡΩΤΥΣΤΕΡΑ ΑΝΙΧΝΕΥΤΕΙ ΚΙΝΗΣΗ ΜΕΤΡΑΕΙ ΤΟ ΧΡΟΝΟΜΕΤΡΟ 100sec ΜΕΤΑ ΤΟΝ ΤΕΡΜΑΤΙΣΜΟ ΤΗΣ ΓΙΑ ΝΑ ΣΒΗΣΕΙ ΤΑ ΦΩΤΑ  
if (valA == LOW) {digitalWrite(lightPinA, LOW);} 
 if (valB == LOW) {digitalWrite(lightPinB, LOW);} 
 
}
