bool newTotal = false;  //checks if a new total measurement is desired
bool pumped = false;    //checks if a pump cycle has been completed

int total = 0;      //total volume of fluid pumped
int potPin = 2;   
int ledPin = 13;  

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  total = 0;      //resets total in every newTotal measurement cycle
  Serial.println("------ Measuring new Total ------");
  Serial.println();
  
  while (!newTotal){
    int val = 0;      //resets measured value in every pump cycle
    int prevVal = 0;  //resets the previous measured value in every pump cycle
    int count[16];    //array of draw volumes to keep count of how many times each is measured

    //resets count[] for each pump cycle
    for(int i = 0; i<16; i++)
      count[i]=0;
    
    while (!pumped){
      val = analogRead(potPin);
      
      count[val/64] += 1; //increments count of each draw volume

      if(val/64 == 0 && prevVal != 0){ //if a plunge cycle is completed
        pumped = true;
        int drawVolume = 0;
        int high = 0; //holds what the greatest count was
        
        for(int i = 1; i < 16; i++){  //checks what draw volume had the greatest count
          if(count[i] > high){
             drawVolume = i;
             high = count[i];
          }
        }

        total += drawVolume;

        printDispensedValue(drawVolume, total);
        
      }
      
      prevVal = val/64; //only assigns prevVal after checking if a plunge cycle is completed

      if(Serial.available() > 0){
        if(Serial.read() == 48){  //ascii for '0' so it exits both loops
          newTotal = true;
          pumped = true;
        }     
      }

      delay(100); //measures 10 times per second 
    }

    pumped = false; //resets pumped variable for new pump cycle
  }

  for(int i = 0; i < 50; i++)
    Serial.println();
    
  newTotal = false; //resets newTotal variable for new total measurement cycle  
}


void printDispensedValue(int location, int total){
  Serial.print("Just Dispensed: ");
  Serial.print(location);
  Serial.print("mL");
  Serial.println();
  Serial.print("Total Dispensed: ");
  Serial.print(total);
  Serial.print("mL");
  Serial.println();
  Serial.println();
  Serial.println("----------------------------------");
  Serial.println();
}
