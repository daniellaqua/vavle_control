
//char array1[12] = {0xFF, 0xFF, 0x02, 0x80, 0x92, 0x05, 0x01, 0x42, 0x48, 0x00, 0x00, 0x1E};
//FFFF0280920501424800001E
//   char array1[12] = {0xFF,0xFF,0x02,0x80,0x92,0x05,0x01,0x42,0x48,0x48,0x48,0x1E};
//  char array1[12] = {0xFF, 0xFF, 0x02, 0x80, 0x92, 0x05, 0x01, 0x42, 0x48, 0x11, 0x11, 0x1E};
//  char array1[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};


// constants won't change. They're used here to set pin numbers:
const int buttonPin = 12;     // the number of the pushbutton pin
int buttonState = 0;

char receivedChar;
boolean newData = false;

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  // initialize serial:
  Serial1.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    //open50();
    open100();
    delay(500);
    close_valve();
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
//void serialEvent() {
//  while (Serial1.available()) {
//    // get the new byte:
//    char inChar = (char)Serial1.read();
//    // add it to the inputString:
//    inputString += inChar;
//    // if the incoming character is a newline, set a flag so the main loop can
//    // do something about it:
//    if (inChar == 0x30) {
//      stringComplete = true;
//    }
//  }
//      delay(1000);
//}

void close_valve(){
  Serial1.write(0xFF);
  Serial1.write(0xFF);
  Serial1.write(0x02);
  Serial1.write(0x80);
  Serial1.write(0x92);  
  Serial1.write(0x05);  
  Serial1.write(0x01);
  Serial1.write(0x00);
  Serial1.write(0x00);  
  Serial1.write(0x00);
  Serial1.write(0x00);
  Serial1.write(0x14);
}


void open50(){
  Serial1.write(0xFF);
  Serial1.write(0xFF);
  Serial1.write(0x02);
  Serial1.write(0x80);
  Serial1.write(0x92);  
  Serial1.write(0x05);  
  Serial1.write(0x01);
  Serial1.write(0x42);
  Serial1.write(0x48);  
  Serial1.write(0x00);
  Serial1.write(0x00);
  Serial1.write(0x1E);
}

void open100(){
  Serial1.write(0xFF);
  Serial1.write(0xFF);
  Serial1.write(0x02);
  Serial1.write(0x80);
  Serial1.write(0x92);  
  Serial1.write(0x05);  
  Serial1.write(0x01);
  Serial1.write(0x42);
  Serial1.write(0xC8);  
  Serial1.write(0x00);
  Serial1.write(0x00);
  Serial1.write(0x9E);
}
