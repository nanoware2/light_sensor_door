#define inPin0 0
//define pins
int ledblau  = 7; //rotes Kabel
int unten    = 6;
int oben     = 5;
int turning  = 4; //HIGH for up, LOW for down
int motor    = 3; //HIGH for motor on
//
//define variables
float min_brgt     = 11;     // brightness below the motor turns downwards
int dtime          = 10;      //delaytime between intervals for measuring brightness
int mpoints        = 20;      //length of brgtn array -better put a function in to get this number
int brgtn[20];                //here shold be the same number like mpoints
float brightness   = 100;   //helligkeitsvariable für die motorsteuerung
int minutes        = 5;             //time (in minutes) between the mean brightness measuring points.
int schalterstatus = 0;
int n              = 0;
int i              = 0;
int imax           = 10000; //number of cycles between two brightness measuring points
int ledState = LOW;
int updown;
float test_brgt = 0; //akkumulierte helligkeit
float current_brgt = 0 ; //mittlere Helligkeit
int oldbright = false;


unsigned long previousMillis = 0;
unsigned long prevtime = 0;
unsigned long previousmtime = 0;

void setup(void) {

  pinMode(7, OUTPUT);
  pinMode(6, INPUT);
  pinMode(5, INPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(9600);
  Serial.println("starting...");
  digitalWrite(11, HIGH);
  test_brgt = analogRead(inPin0)/ 1024.0 * 100;
  brightness = test_brgt;
}

void loop(void) {
  //reading switches
  int obenstatus = digitalRead(oben);
  int untenstatus = digitalRead(unten);
  //------------------------

  //measuring of the brightness change oldbright true or false for old or new method.
  if (oldbright) {
    i = i + 1;

    if (i == imax) {
      brgtn[n] = analogRead(inPin0);

      n = n + 1;
      if (n == 20) {
        n = 0;

        for (int n = 0; n < mpoints; n++) {

          brightness += brgtn[n];

        }

      }

      i = 0;
    }

    if (i == 0 && n == 0) {


      brightness = brightness / 1024.0 * 100 / mpoints;
      Serial.print(brightness);
      Serial.println();

    }
  }
  //new brightness measuring mode
  else {

    unsigned long currenttime = millis();

    //  Serial.print(currenttime - prevtime);
    // Serial.println();

    if (currenttime - prevtime >= 60000) {
      brgtn[n] = analogRead(inPin0) / 1024.0 * 100;
      test_brgt += brgtn[n];
      n = n + 1;
      current_brgt = test_brgt / (n + 1);
      prevtime = currenttime;
      Serial.println("current_brgt");
      Serial.print(current_brgt);
      Serial.println();

      if (n == minutes) {
        brightness = test_brgt / (n + 1);
        n = 0;
        current_brgt = brightness;
        test_brgt = brightness;
        Serial.println("brightness:");
        Serial.print(brightness);
        Serial.println();

      }
    }





  }
  //------------------------

  


  //LED Status der Klappe
  if (obenstatus == HIGH) {
    digitalWrite(ledblau, LOW);
  }

  if (untenstatus == HIGH) {
    digitalWrite(ledblau, HIGH);  //LED leuchtet wenn die klappe unten ist

  }


//------------motormanagement------------

  if (brightness < min_brgt) {

    digitalWrite(turning, LOW);

    if (untenstatus == HIGH) {
      digitalWrite(motor, LOW);
    }

    if (untenstatus == LOW) {
      digitalWrite(motor, HIGH);

      //motorschutzmanagement
//      if (obenstatus == HIGH) {
//
//
//        unsigned long currentmtime = millis();
//        Serial.println(currentmtime);
//        
//        if (currentmtime - previousmtime >= 5000) {
//          previousmtime = currentmtime;
//          digitalWrite(motor, LOW);
//          Serial.println("ERROR, stopping program");
//          while (true) { //hier bleibt das Programm drin hängen, wenn die Klappe versehentlich nach oben fährt. LED blinkt schnell
//            unsigned long currentMillis = millis();
//
//            if (currentMillis - previousMillis >= 120) {
//              // save the last time you blinked the LED
//              previousMillis = currentMillis;
//
//              if (ledState == LOW) {
//                ledState = HIGH;
//              }
//              else {
//                ledState = LOW;
//              }
//            }
//            digitalWrite(ledblau, ledState);
//          }
//
//        }
//      }


    }




  }

  if (brightness >= min_brgt) {
    digitalWrite(turning, HIGH);


    if (obenstatus == HIGH) {
      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= 350) {
        previousMillis = currentMillis;
        digitalWrite(motor, LOW);
      }
    }

    if (untenstatus == HIGH) {
      digitalWrite(motor, HIGH);
    }
    if (obenstatus == LOW) {

      digitalWrite(motor, HIGH);

    }
  }


}
