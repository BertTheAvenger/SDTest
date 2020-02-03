#include <Arduino.h>
#include <SD.h>

#define timerStart() (init = millis())
#define timerPrint(phrase) fin = millis(); Serial.print(phrase); Serial.println(fin - init);
#define sizePrint() Serial.print("File size increased by "); Serial.println(fSizefin - fSizeinit);

uint32_t writingSize = 0;
const uint32_t maxWriteSize = 100000;
const uint32_t increment = 10000;
uint8_t buf[maxWriteSize];

//B = buffered
//S = single
//Buffered will write buffers, Single will write single bytes...


void setup() {
  Serial.begin(115200);
  while (!Serial)
  {
    ;
  }
  
  if(!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Error initing SD, exiting...");
    return;
  } else {
    SD.remove("b.bin");
    SD.remove("s.bin");
  }

  //Generate a buffer to test writes with.
  uint32_t ctr = 0;
  for(uint32_t i = 0; i < maxWriteSize; i++) {
    buf[i] = ctr;
    ctr++;
  }

  /** DO THE TEST **/

  for(uint32_t size = 0; size < maxWriteSize; size += increment) { //Increase write size tests 100 bytes at a time.
    uint32_t init = 0; //Timer
    uint32_t fin = 0;
    uint32_t fSizeinit = 0;
    uint32_t fSizefin = 0;
    File file;
    Serial.println("\n\n");
    Serial.print("WRITING SIZE... ");
    Serial.println(size);
    Serial.println("===============================================================");



    /*-------------------Single Byte Write Test-----------------------*/
    file = SD.open("s.bin", FILE_WRITE);
    if(!file) {Serial.println("Err opening file"); return;}
    fSizeinit = file.size();

    timerStart();
    for(uint32_t i = 0; i < size; i++) { //Fill with a for loop.
      file.write(2);
      ctr++;
    }
    timerPrint("Wrote SINGLE BYTES in ");


    timerStart();
    file.flush();
    timerPrint("Flushed SINGLE BYTES in ");
    
    fSizefin = file.size();
    sizePrint();
    Serial.println("------------------------------------------\n");

    file.close();


    /*-------------------------Buffer Write Test------------------------------------*/
    file = SD.open("b.bin", FILE_WRITE);
    fSizeinit = file.size();

    timerStart();
    file.write(buf, size);
    timerPrint("Wrote BUFFER in ");

    timerStart();
    file.flush();
    timerPrint("Flushed BUFFER in ");

    fSizefin = file.size();
    sizePrint();

    file.close();
  }



  Serial.println("\n\nFINISHED.");
  
}

void loop() {

}