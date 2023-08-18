
#include <Servo.h>
#include "inc.h" 


// Used to hold data that comes from the serial port
String readString; 

// holds serial port incoming messages
String inputString = "";

// turn light on
bool lighton = false;

bool bPrintBytes = false;

// Message from the serial port is ready
bool stringComplete = false; 

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 30000;  //the value is a number of milliseconds

Servo myServo;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // put your setup code here, to run once:
  startMillis = millis();  //initial start time
  myServo.attach(6);                
  Serial.begin(19200);
  Serial.println(SECRET_SSID);
}

void loop() {
  // put your main code here, to run repeatedly:
  readstring();
  
  if(lighton == true)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }else if(lighton == false)
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
  currentMillis = millis();
  
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    Serial.println(SECRET_SSID);
    startMillis = millis();
  }

   if(stringComplete)
   {
      handleInput(inputString);
   }
}



void serial_flush(void) {
  while (Serial.available()) Serial.read();
}

void runPro()
{
  for(int i = 0;i < 10;i++)
  {
    setServo(180);
    delay(1000);
    setServo(1);
    delay(1000);
  }
  
}

void setServo(int newpos)
{
  Serial.print("Setting position to ");
  Serial.println(newpos);
  myServo.write( newpos);
  delay(20);
}

void readstring()
{
    while (Serial.available()) {
      // get the new byte:
      char inChar = (char)Serial.read();
      // add it to the inputString:
      inputString += inChar;
        // if the incoming character is a newline, set a flag so the main loop can
        // do something about it:
        if (inChar == '\n')
        {
          stringComplete = true;
          serial_flush();
          return;
        }
    }
}


bool containsStr(String inStr, String substr)
{
  int pos = inStr.indexOf(substr);

  if (pos >= 0) {
    return true;
  } else {
    return false;
  }
  delay(100);
}

int extractNumber(String inStr)
{
  String numStr = "";

  // Extract numbers from the string
  for (int i = 0; i < inStr.length(); i++) {
    if (isdigit(inStr.charAt(i))) {
      numStr += inStr.charAt(i);
    }
  }

  // Convert the extracted string of numbers to an integer
  int num = atoi(numStr.c_str());
  return num;
}

void handleInput(String iput)
{
  
  Serial.println(iput);
  if(bPrintBytes)
  {
    printBytes(iput);
  }
  
  if(iput == "on\n" || iput == "on")
  {
    lighton = true;
    //break;
  }else if(iput == "off\n" || iput == "off")
  {
    lighton = false;
   // break;
  }else if(iput == "byteson" || iput == "byteson\n")
  {
    bPrintBytes = true;
    Serial.println("I will print the bytes of received messages.");
    //break;
  }else if(iput == "bytesoff" || iput == "bytesoff\n")
  {
    bPrintBytes = false;
    Serial.println("I will not print the bytes of received messages.");
    //break;
  }else if(containsStr(iput,"servo"))
  {

    int newVal = extractNumber(iput);

    setServo(newVal);
  }else if(containsStr(iput,"run"))
  {

   runPro();
   
  }else if(iput == "pos\n")
  {
    Serial.println(myServo.read());
  }

  stringComplete = false;
  inputString = "";

}


void printBytes(String bStr){
int strlen = bStr.length();
  for(int i = 0;i< strlen;i++)
  {
    Serial.print(byte(bStr[i]),HEX);
    Serial.print(" ");
  }

  Serial.print('\n');
}




