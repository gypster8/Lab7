int ledPins[4] = { 1, 2, 3, 4 };

int channelButtonPin = 33;
int currentChannel = 0;

int buttonPins[4] =  { 29, 30, 31, 32 };
int channelLedPins[3] = { 41, 40, 39,  };


// int buttonOn[4] = { LOW, LOW, LOW, LOW };
int buttonOn[3][4] = {
  { LOW, LOW, LOW, LOW },
  { LOW, LOW, LOW, LOW },
  { LOW, LOW, LOW, LOW },
};

bool buttonState[4] = { LOW, LOW, LOW, LOW };
bool lastButtonState[4] = { LOW, LOW, LOW, LOW };

int midiNotes[3] = { 36, 38, 42 };

bool lastChannelButtonState = LOW;
bool channelButtonState = LOW;

// int checkDisplayed = 2; 

unsigned long lastStepTime = 0;
int currentStep = 0;
int tempo = 500;


int potVal = A14;

//int brightness = 0;

void setup() {
  // put your setup code here, to run once:
 

  pinMode(buttonPins[0], INPUT);
  pinMode(buttonPins[1], INPUT);
  pinMode(buttonPins[2], INPUT);
  pinMode(buttonPins[3], INPUT);

  pinMode(channelLedPins[0], OUTPUT);
  pinMode(channelLedPins[1], OUTPUT);
  pinMode(channelLedPins[2], OUTPUT);

  pinMode(channelButtonPin, INPUT);

  pinMode(ledPins[0], OUTPUT);
  pinMode(ledPins[1], OUTPUT);
  pinMode(ledPins[2], OUTPUT);
  pinMode(ledPins[3], OUTPUT);

  Serial.begin(9600);

  

}

void checkButton() {

  for (int i = 0; i < 4; i++) {
    lastButtonState[i] = buttonState[i];
    buttonState[i] = digitalRead(buttonPins[i]);
    if (buttonState[i] == LOW and lastButtonState[i] == HIGH) {
      buttonOn[currentChannel][i] = !buttonOn[currentChannel][i];
      delay(5);
    } else if (buttonState[i] == HIGH and lastButtonState[i] == LOW) {
      delay(5);
    }
  }
}

void checkLed() {
  // brightness = analogRead(potVal);
  // brightness = map(brightness, 0, 1023, 100, 255);
  for (int i = 0; i < 4; i++) {
    if (currentStep == i) {
      analogWrite(ledPins[i], 255);
    } else if (buttonOn[currentChannel][i] == true) {
      analogWrite(ledPins[i], 50);
    } else {
      analogWrite(ledPins[i], 0);
    }
  }
}

void checkChannelButton() {
  
  lastChannelButtonState = channelButtonState;
  channelButtonState = digitalRead(channelButtonPin);
  if (channelButtonState == LOW and lastChannelButtonState == HIGH) {
    //Button is pressed
    //Increase channel by 1
    currentChannel = currentChannel + 1;
    //channelDisplayed += 1;

    //if channel is greater than 2, go back to 0

    if(currentChannel > 2) {
      currentChannel = 0;
    }

    Serial.println(currentChannel);


    delay(5);
  } else if (channelButtonState == HIGH and lastChannelButtonState == LOW) {
    delay(5);
  }
}

void checkChannelLeds() {
  Serial.println(channelLedPins[currentChannel]);
  for (int i = 0; i < 3; i++) {
    if (currentChannel == i) {
      digitalWrite(channelLedPins[i], HIGH);
    } else {
      digitalWrite(channelLedPins[i], LOW);
    }
  }
}


void sequence() {
  if (millis() >= lastStepTime + tempo) {
    //Do something
    lastStepTime = millis();

    if (buttonOn[0][currentStep] == HIGH) {
      usbMIDI.sendNoteOn(midiNotes[0], 127, 1);
      usbMIDI.sendNoteOff(midiNotes[0], 127, 1);
    }
    if (buttonOn[1][currentStep] == HIGH) {
      usbMIDI.sendNoteOn(midiNotes[1], 127, 1);
      usbMIDI.sendNoteOff(midiNotes[1], 127, 1);
    }
    if (buttonOn[2][currentStep] == HIGH) {
      usbMIDI.sendNoteOn(midiNotes[2], 127, 1);
      usbMIDI.sendNoteOff(midiNotes[2], 127, 1);
    }
    
    countUp();
  } 
}

void countUp() {
  currentStep++;
  if (currentStep >= 4) {
    currentStep = 0;
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  checkChannelButton();
  checkChannelLeds();
  checkButton();
  checkLed();
  sequence();
}
