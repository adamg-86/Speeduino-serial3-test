/////////////////////////
// speeduino serial comm test serial 3
// by Adam Gauthier
// 13/06/2021
////////////////////////

#include <SPI.h>
#include <Wire.h>

void GetData();

uint8_t Data[120]; //Data buffer for serial read

int temp[3];
byte x;
byte offset;

void setup()
{

  Serial.begin(115200);
  Serial1.begin(115200); //  console Serial
}

void loop()
{
  temp[0] = 0;
  temp[1] = 0;
  temp[2] = 0;
  x = 0;
  if (Serial.available())
  {
    while (Serial.available())
    {
      temp[x] = (byte)(Serial.read());
      temp[x] -= 48;
      x++;
    }
    if (x == 2)
    {
      offset = temp[1] + (temp[0] * 10);
    }

    else if (x == 3)
    {
      offset = temp[2] + (temp[1] * 10) + (temp[0] * 100);
    }

    else
    {
      offset = temp[0];
    }
  }
  GetData(0, 0x30, offset, 1);
  delay(50);
}

void GetData(uint8_t canID, uint8_t cmd, uint16_t offset, uint16_t length)
{
  byte firstByte = 0;
  byte cmdr = 0;      //cmd recived
  uint16_t cmdLength; //number of byte to recive
  Serial1.write("r");
  Serial1.write(canID);            //canID
  Serial1.write(cmd);              // cmd
  Serial1.write(lowByte(offset));  //offset low byte
  Serial1.write(highByte(offset)); //offset high byte
  Serial1.write(lowByte(length));  //length low byte
  Serial1.write(highByte(length)); //length high byte
  cmdLength = offset + length;
  if (Serial1.available())
  {
    firstByte = Serial1.read();
    cmdr = Serial1.read(); // will be 0x30
  }

  if ((firstByte == 'r') && (cmdr == cmd))
    while (Serial1.available())
    {
      for (offset; offset < cmdLength; offset++)
      {
        Data[offset] = Serial1.read();
        Serial.print(offset);
        Serial.print(" - ");
        Serial.println(Data[offset]);
      }
    }
}
