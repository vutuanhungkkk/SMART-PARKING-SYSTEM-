
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h> 
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9

//////////////////////////
//////config//////////////
MFRC522 RFID(SS_PIN,RST_PIN); /// chân 9,10
LiquidCrystal_I2C lcd(0X27,16,2); //SCL A5 SDA A4
Servo inser;
Servo outser;
int UID[4], i;
int ID[4] = {86, 183, 97, 31}; 
int InIrPin = 2 ; 
int OutIrPin = 3 ; 
int AIrPin = 4 ; 
int BIrPin = 7 ; 


void setup()
{
SPI.begin();
RFID.PCD_Init();
delay(1000);
Serial.begin(128000);
lcd.init();
lcd.backlight();

inser.attach(6);
outser.attach(5);
pinMode(InIrPin,INPUT);
pinMode(OutIrPin,INPUT);
pinMode(AIrPin,INPUT);
pinMode(BIrPin,INPUT);
Serial.println("CLEARDATA");
Serial.println("LABEL,Time,State,Date,UID");
inser.write(0);
outser.write(0);
}


void loop()
{

/////////doc vi tri trong///////
int slotA = digitalRead(AIrPin);
int slotB = digitalRead(BIrPin);
int IN = digitalRead(InIrPin);

if (slotA==0)
{
  lcd.setCursor(0,0);
  lcd.print("A:co xe");
  delay(100);
}
else {
  lcd.setCursor(0,0);
  lcd.print("A:trong");
   delay(100);
}

if (slotB==0)
{
  lcd.setCursor(9,0);
  lcd.print("B:co xe");
   delay(100);
}
else {
  lcd.setCursor(9,0);
  lcd.print("B:trong");
   delay(100);
}
////////////////////////////////////

/////////TH full vi tri///////////////

if( IN==0 && slotA==0 && slotB==0)
{
lcd.setCursor(2,1);
lcd.print("khong con cho");
delay(5000);
lcd.clear();
}
////////////////////////////////////


/////////TH có xe den/////////////////
 if (!digitalRead(InIrPin))
  {
    lcd.setCursor(2, 1);
    lcd.print("Moi nhap the");

    if (!RFID.PICC_IsNewCardPresent())
    {
      return;
    }

    if (!RFID.PICC_ReadCardSerial())
    {
      return;
    }

    for (byte i = 0; i < RFID.uid.size; i++) // đọc ID của thẻ
    {
      UID[i] = RFID.uid.uidByte[i];
    }

    bool correctUID = true; // Biến xác định UID có trùng khớp với UID mong đợi hay không

    for (byte i = 0; i < RFID.uid.size; i++)
    {
      if (UID[i] != ID[i])
      {
        correctUID = false;
        break;
      }
    }

    if (correctUID) //nếu đọc được ID của thẻ mình đang sử dụng thì thực hiện đóng mở relay
    {
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("Welcome");
      String UIDString = String(UID[0]) + String(UID[1]) + String(UID[2]) + String(UID[3]);
      Serial.println((String) "DATA,TIME,VAO,DATE,"+UIDString);
      inser.write(90);
      delay(4000);
      inser.write(0);
      lcd.clear();
    }
    else
    {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Khong hop le"); // In thông báo sai thẻ
      delay(2000);
      lcd.clear();
    }

    RFID.PICC_HaltA();      // ngừng đọc giá trị
   RFID.PCD_StopCrypto1(); // Dừng tính năng mã hóa thẻ
  }
///////////////////TH có xe ra/////////////////////
     if (!digitalRead(OutIrPin))
  {
    lcd.setCursor(2, 1);
    lcd.print("Moi nhap the");

    if (!RFID.PICC_IsNewCardPresent())
    {
      return;
    }

    if (!RFID.PICC_ReadCardSerial())
    {
      return;
    }

    for (byte i = 0; i < RFID.uid.size; i++) // đọc ID của thẻ
    {
      UID[i] = RFID.uid.uidByte[i];
    }

    bool correctUID = true; // Biến xác định UID có trùng khớp với UID mong đợi hay không

    for (byte i = 0; i < RFID.uid.size; i++)
    {
      if (UID[i] != ID[i])
      {
        correctUID = false;
        break;
      }
    }

    if (correctUID) //nếu đọc được ID của thẻ mình đang sử dụng thì thực hiện đóng mở relay
    {
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("Goodbye");
      String UIDString = String(UID[0]) + String(UID[1]) + String(UID[2]) + String(UID[3]);
      Serial.println((String) "DATA,TIME,RA,DATE,"+UIDString);
      outser.write(90);
      delay(4000);
      outser.write(0);
      lcd.clear();
    }
    else
    {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Khong hop le"); // In thông báo sai thẻ
      delay(2000);
      lcd.clear();
    }

    RFID.PICC_HaltA();      // ngừng đọc giá trị
   RFID.PCD_StopCrypto1(); // Dừng tính năng mã hóa thẻ
  }
  ////////////////////////////////////////////////////////////
}





