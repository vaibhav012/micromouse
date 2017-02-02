#define LeftFarSensor    8
#define LeftNearSensor   9
#define CenterSensor     2
#define RightNearSensor  3
#define RightFarSensor   4

int LeftFarReading;
int LeftNearReading;
int CenterReading;
int RightNearReading;
int RightFarReading;

int i = 0, j = 0, DryRun = 0, k=0;

#define ButtonOn    13
#define ButtonMode  12
#define ButtonSignal A0

#define LeftMotor1  5
#define LeftMotor2  6

#define RightMotor1 10
#define RightMotor2 11

#define ClearTime  200
#define MotorSpeed 250
#define ShortDelay 750

char Path[30];

void setup()
{
  Serial.begin(9600);
  pinMode(LeftNearSensor, INPUT);
  pinMode(LeftFarSensor, INPUT);
  pinMode(CenterSensor, INPUT);
  pinMode(RightNearSensor, INPUT);
  pinMode(RightFarSensor, INPUT);
  pinMode(ButtonMode, INPUT);
  pinMode(ButtonOn, INPUT);

  pinMode(LeftMotor1, OUTPUT);
  pinMode(LeftMotor2, OUTPUT);
  pinMode(RightMotor1, OUTPUT);
  pinMode(RightMotor2, OUTPUT);
  pinMode(ButtonSignal, OUTPUT);
  //delay(1000);
  digitalWrite(ButtonSignal,HIGH);
}

void loop() {

  if (digitalRead(ButtonMode) == 1)
  {
    FinalFollow();
  }
  else if (DryRun == 1)
  {
    Serial.print("DRY RUN ENDS CONNECT PIN 12 TO HIGH FOR FINAL PATH RUN\n");
    printArray();
  }
  else if (DryRun == 0)
  {
    readSensors();
    //******************************DECISION MAKING**********************************
    //RIGHT TURN
    if (RightFarReading == 0)
    {
      StopMotors();
      CheckRaceEnd();
      if (DryRun == 0)
      {
        delay(ShortDelay);
        GoForward();
        readSensors();
        while(RightFarReading != 1)
        {
          readSensors();
        }
        delay(ClearTime);
        Path[i] = 'R';
        i++;
        if (Path[i - 2] == 'B')
        {
          modify();
        }
        printArray();
        Serial.print("\n");
        analogWrite(LeftMotor1, 0);
        analogWrite(LeftMotor2, MotorSpeed);
        analogWrite(RightMotor1, MotorSpeed);
        analogWrite(RightMotor2, 0);
        delay(ShortDelay);
        readSensors();
        while (CenterReading != 0)
        {
          readSensors();
          Serial.print("LOOPING\n");
        }
      }
    }
    //LEFT TURN
    else if (LeftFarReading == 0)
    {
      StopMotors();
      CheckRaceEnd();
      if (DryRun == 0)
      {
        delay(ShortDelay);
        GoForward();
        readSensors();
        while(LeftFarReading != 1)
        {
          readSensors();
        }
        delay(ClearTime);
        readSensors();
        if (CenterReading != 0)
        {
          Path[i] = 'L';
          i++;
          if (Path[i - 2] == 'B')
          {
            modify();
          }
          printArray();
          Serial.print("\n");
          analogWrite(LeftMotor1, MotorSpeed);
          analogWrite(LeftMotor2, 0);
          analogWrite(RightMotor1, 0);
          analogWrite(RightMotor2, MotorSpeed);
          delay(ShortDelay);
          readSensors();
          while (CenterReading != 0)
          {
            readSensors();
            Serial.print("LOOPING\n");
          }
        }
        else
        {
          Path[i] = 'S';
          i++;
          if (Path[i - 2] == 'B')
          {
            modify();
          }
          printArray();
          Serial.print("\n");
          delay(ClearTime);
        }
      }
    }
    //DEAD END
    else if (LeftFarReading == 1 && LeftNearReading == 1 && CenterReading == 1 && RightNearReading == 1 && RightFarReading == 1)
    {
      StopMotors();
      //CheckRaceEnd();
      if (DryRun == 0)
      {
        delay(ShortDelay);
        GoForward();
        delay(ClearTime);
        readSensors();
        if (LeftFarReading == 1 && LeftNearReading == 1 && CenterReading == 1 && RightNearReading == 1 && RightFarReading == 1)
        {
          Path[i] = 'B';
          i++;
          printArray();
          Serial.print("\n");
          analogWrite(LeftMotor1, MotorSpeed);
          analogWrite(LeftMotor2, 0);
          analogWrite(RightMotor1, 0);
          analogWrite(RightMotor2, MotorSpeed);
          delay(ShortDelay);
          readSensors();
          while (CenterReading != 0)
          {
            readSensors();
            Serial.print("DEAD END ROTATION\n");
            /*if(LeftNearReading == 0)
              {
                analogWrite(LeftMotor1,MotorSpeed);
                analogWrite(LeftMotor2,0);
                analogWrite(RightMotor1,0);
                analogWrite(RightMotor2,MotorSpeed);
                while(CenterReading != 0)
                {
                  readSensors();
                }
                break;
              }*/
          }
        }
      }
    }
    //*******************************************************************************

    //******************************LINE FOLLOWER************************************
    if (LeftNearReading == 1 && RightNearReading == 1 && CenterReading == 0)
    {
      analogWrite(LeftMotor1, 0);
      analogWrite(LeftMotor2, MotorSpeed);
      analogWrite(RightMotor1, 0);
      analogWrite(RightMotor2, MotorSpeed);
    }
    else if (LeftNearReading == 0)
    {
      analogWrite(LeftMotor1, MotorSpeed);
      analogWrite(LeftMotor2, 0);
      analogWrite(RightMotor1, 0);
      analogWrite(RightMotor2, MotorSpeed);
    }
    else if (RightNearReading == 0)
    {
      analogWrite(LeftMotor1, 0);
      analogWrite(LeftMotor2, MotorSpeed);
      analogWrite(RightMotor1, MotorSpeed);
      analogWrite(RightMotor2, 0);
    }
    //*******************************************************************************
  }
}


void readSensors()
{
  LeftFarReading     = digitalRead(LeftFarSensor);
  LeftNearReading    = digitalRead(LeftNearSensor);
  CenterReading      = digitalRead(CenterSensor);
  RightNearReading   = digitalRead(RightNearSensor);
  RightFarReading    = digitalRead(RightFarSensor);
  Serial.print(LeftFarReading);
  Serial.print(LeftNearReading);
  Serial.print(CenterReading);
  Serial.print(RightNearReading);
  Serial.print(RightFarReading);
  Serial.print("\n");
}

void printArray()
{
  for (j = 0; j < i; j++)
  {
    Serial.print(Path[j]);
  }
}

void StopMotors()
{
  analogWrite(LeftMotor1, 0);
  analogWrite(LeftMotor2, 0);
  analogWrite(RightMotor1, 0);
  analogWrite(RightMotor2, 0);
}

void GoForward()
{
  analogWrite(LeftMotor1, 0);
  analogWrite(LeftMotor2, MotorSpeed);
  analogWrite(RightMotor1, 0);
  analogWrite(RightMotor2, MotorSpeed);
}

void modify()
{
  printArray();
  Serial.print("MODIFYING");
  if (Path[i - 3] == 'L' && Path[i - 2] == 'B' && Path[i - 1] == 'R')
  {
    Path[i - 3] = 'B';
    i = i - 2;
  }
  if (Path[i - 3] == 'R' && Path[i - 2] == 'B' && Path[i - 1] == 'L')
  {
    Path[i - 3] = 'B';
    i = i - 2;
  }
  if (Path[i - 3] == 'R' && Path[i - 2] == 'B' && Path[i - 1] == 'S')
  {
    Path[i - 3] = 'L';
    i = i - 2;
  }
  if (Path[i - 3] == 'R' && Path[i - 2] == 'B' && Path[i - 1] == 'R')
  {
    Path[i - 3] = 'S';
    i = i - 2;
  }
  if (Path[i - 3] == 'S' && Path[i - 2] == 'B' && Path[i - 1] == 'R')
  {
    Path[i - 3] = 'L';
    i = i - 2;
  }
  if (Path[i - 3] == 'S' && Path[i - 2] == 'B' && Path[i - 1] == 'S')
  {
    Path[i - 3] = 'B';
    i = i - 2;
  }
  printArray();
}

void CheckRaceEnd()
{
  Serial.print("checking race end");
  StopMotors();
  analogWrite(LeftMotor1, 0);
  analogWrite(LeftMotor2, MotorSpeed);
  analogWrite(RightMotor1, 0);
  analogWrite(RightMotor2, MotorSpeed);
  delay(ClearTime);
  StopMotors();
  readSensors();
  if (RightFarReading == 0 && RightNearReading == 0 && CenterReading == 0 && LeftNearReading == 0 && LeftFarReading == 0)
  {
    DryRun = 1;
    Serial.print("RACE ENDED");
  }
  analogWrite(LeftMotor1, MotorSpeed);
  analogWrite(LeftMotor2, 0);
  analogWrite(RightMotor1, MotorSpeed);
  analogWrite(RightMotor2, 0);
  delay(ClearTime);
  StopMotors();
}

void FinalFollow()
{
  printArray();
  Serial.print("\n");
  readSensors();
  /*********************************DECISION**************************************/
  if (RightFarReading == 0 || LeftFarReading == 0)
  {
    if (Path[k] == 'L')
    {
      Serial.print("LEFT");
      StopMotors();
      delay(ShortDelay);
      GoForward();
      delay(ClearTime);
      k++;
      analogWrite(LeftMotor1, MotorSpeed);
      analogWrite(LeftMotor2, 0);
      analogWrite(RightMotor1, 0);
      analogWrite(RightMotor2, MotorSpeed);
      delay(ShortDelay);
      readSensors();
      while (CenterReading != 0)
      {
        readSensors();
        Serial.print("LOOPING\n");
      }
    }
    else if (Path[k] == 'R')
    {
      Serial.print("RIGHT");
      StopMotors();
      delay(ShortDelay);
      GoForward();
      delay(ClearTime);
      k++;
      analogWrite(LeftMotor1, 0);
      analogWrite(LeftMotor2, MotorSpeed);
      analogWrite(RightMotor1, MotorSpeed);
      analogWrite(RightMotor2, 0);
      delay(ShortDelay);
      readSensors();
      while (CenterReading != 0)
      {
        readSensors();
        Serial.print("LOOPING\n");
      }
    }
    else if (Path[k] == 'S')
    {
      analogWrite(LeftMotor1, 0);
      analogWrite(LeftMotor2, MotorSpeed);
      analogWrite(RightMotor1, 0);
      analogWrite(RightMotor2, MotorSpeed);
      Serial.print("STRAIGHT");
      /*readSensors();
      while (LeftFarSensor != 1 && RightFarSensor != 1)
      {
        readSensors();
        Serial.print("LOOPING\n");
      }*/
      delay(ClearTime);
      k++;
    }
  }
  /*******************************************************************************/

  /******************************LINE FOLLOWER************************************/
  if (LeftNearReading == 1 && RightNearReading == 1 && CenterReading == 0)
  {
    analogWrite(LeftMotor1, 0);
    analogWrite(LeftMotor2, MotorSpeed);
    analogWrite(RightMotor1, 0);
    analogWrite(RightMotor2, MotorSpeed);
  }
  else if (LeftNearReading == 0)
  {
    analogWrite(LeftMotor1, MotorSpeed);
    analogWrite(LeftMotor2, 0);
    analogWrite(RightMotor1, 0);
    analogWrite(RightMotor2, MotorSpeed);
  }
  else if (RightNearReading == 0)
  {
    analogWrite(LeftMotor1, 0);
    analogWrite(LeftMotor2, MotorSpeed);
    analogWrite(RightMotor1, MotorSpeed);
    analogWrite(RightMotor2, 0);
  }
  /*******************************************************************************/
}

