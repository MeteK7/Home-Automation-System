#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include <Servo.h>

#include <SimpleDHT.h>
int pinDHT11 = 8;
SimpleDHT11 dht11;

#define RESETPIN 9
#define SDAPIN 10
#define GLIGHT 6
#define RLIGHT 7
#define KLIGHT 5
#define LROOMLIGHT 2
#define BROOMLIGHT 4

byte readCard[4];
byte counter;
bool result = false;
int successRead;
int ServoPin = 3;
int ServoPosition;

Servo towerServo;

MFRC522 reader(SDAPIN, RESETPIN);   // Create MFRC522 instance.

MFRC522::MIFARE_Key key;

void setup()
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  reader.PCD_Init();  // Initialize MFRC522 Hardware
  pinMode(GLIGHT, OUTPUT);
  pinMode(RLIGHT, OUTPUT);
  pinMode(KLIGHT, OUTPUT); //set pin as output , yellow kitchen
  pinMode(LROOMLIGHT, OUTPUT); //set pin as output , blue salon
  pinMode(BROOMLIGHT, OUTPUT); //set pin as output , green bathroom
  towerServo.attach(ServoPin); // We have indicated that the data pin of the servo motor is connected to pin 4 of the Arduino.
  towerServo.write(130);


}
void loop()
{
  float temperature = 0;
  float humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read2(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess)
  {
    Serial.print("Read DHT11 failed, err="); Serial.println(err); delay(2000);
    return;
  }

  Serial.print((float)temperature);
  Serial.print(",");
  Serial.println((float)humidity);
  delay(2500);

  do {

    if (Serial.available()) { //id data is available to read
      char val = Serial.read();

      if (val == 'g') { //if g received
        if (digitalRead(BROOMLIGHT) == LOW) {
          digitalWrite(BROOMLIGHT, HIGH);
        }
        else
        {
          digitalWrite(BROOMLIGHT, LOW);
        }
      }
      else if (val == 'b') { //if b received
        if (digitalRead(LROOMLIGHT) == LOW) {
          digitalWrite(LROOMLIGHT, HIGH);
        }
        else
        {
          digitalWrite(LROOMLIGHT, LOW);
        }

      }
      else if (val == 'y') { //if y received
        if (digitalRead(KLIGHT) == LOW) {
          digitalWrite(KLIGHT, HIGH);
        }
        else
        {
          digitalWrite(KLIGHT, LOW);
        }

      }

      else if (val == 'f') { //if all received
        digitalWrite(BROOMLIGHT, LOW);
        digitalWrite(LROOMLIGHT, LOW);
        digitalWrite(KLIGHT, LOW);
      }

    }
    //Call the getID function until the read is successful
    successRead = getID();
  }
  while (!successRead);
  delay(1000);


}

int getID() {
  // Look for new cards, if there is no card, then return 0;
  if ( ! reader.PICC_IsNewCardPresent()) {
    return 0;
  }
  // Select one of the cards
  if ( ! reader.PICC_ReadCardSerial()) {
    return 0;
  }

  else
  {
    check();
    return 1;
  }
}

int check()
{
  int address = 0;

  for (int i = 0; i < reader.uid.size; i++) //This loop is for reading the card's ID.
  {
    readCard[i] = reader.uid.uidByte[i]; //Reading Card's ID as Byte.
  }

  while (EEPROM.read(address) != 0)
  {
    counter = 0;

    for (int i = 0; i < 4; i++)
    {
      if (EEPROM.read(address) == readCard[i])
      {
        counter++;
      }
      address += 1;
    }

    if (counter == 4)
    {
      result = true; 
      break;  //If the result is true, no need to continue to the loop.
    }
  }

  if (result == true)
  {
    Serial.println("Door is opening...");
    for (int i = 0; i < 4; i++)
    {
      Serial.print(readCard[i], HEX); //Printing Card's ID to the screen as Hexedecimal.
      Serial.print(" ");
    }
    Serial.println("");
    digitalWrite(GLIGHT, HIGH);

    result = false;
    for (int i = 130; i >= 0; i--)//This loop is for opening the door by using servo motor. It will decrease the angle of the servo from 130 to 0 one by one to open the door.
    {
      towerServo.write(i);
      delay(30);
    }

    delay(1000);

    for (int i = 0; i <= 130; i++)//This loop is for closing the door by using servo motor. It will increase the angle of servo one by one till 130 angle.
    {
      towerServo.write(i);
      delay(10);
    }

    digitalWrite(GLIGHT, LOW);
    Serial.println("Door is closed.");
  }

  else
  {
    for (int i = 0; i < 4; i++)
    {
      Serial.print(readCard[i], HEX);
      Serial.print(" ");
    }
    Serial.println("");
    digitalWrite(RLIGHT, HIGH);
    delay(200);
    digitalWrite(RLIGHT, LOW);
  }
  address = 0;
}
