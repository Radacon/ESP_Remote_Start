//Amanda's KeyFob
int Amanda_Lock_GPIO = 13; //Green
int Amanda_Unlock_GPIO = 12; //Blue/White
int Amanda_Start_GPIO = 15; //Blue

//Conrad's KeyFob
//Pinouts courtesy of https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
//Blue
int Conrad_Lock_GPIO = 2;
//Blue/White
int Conrad_Unlock_GPIO = 14;
//Green Wire
int Conrad_Start_GPIO = 0;

//Lots of delays required to emulate button presses
const int button_delay = 400;
const int button_off_delay = 250;
const int start_hold = 4000;

void init_GPIO()
{
  //Initialize the GPIO Pins for Output Only and set to low
  //Amanda
  pinMode(Amanda_Lock_GPIO, OUTPUT);
  digitalWrite(Amanda_Lock_GPIO,HIGH);
  pinMode(Amanda_Unlock_GPIO, OUTPUT);
  digitalWrite(Amanda_Unlock_GPIO,HIGH);
  pinMode(Amanda_Start_GPIO, OUTPUT);
  digitalWrite(Amanda_Start_GPIO,HIGH);
  //Conrad
  pinMode(Conrad_Lock_GPIO, OUTPUT);
  digitalWrite(Conrad_Lock_GPIO, HIGH);
  pinMode(Conrad_Unlock_GPIO, OUTPUT);
  digitalWrite(Conrad_Unlock_GPIO, HIGH);
  pinMode(Conrad_Start_GPIO, OUTPUT);
  digitalWrite(Conrad_Start_GPIO, HIGH);
}

void startAmanda()
{
  Serial.println("Starting Amanda's Car");
  //Press Lock Twice
  lockAmanda();
  //Press and Hold Start for 3 Seconds
  digitalWrite(Amanda_Start_GPIO, LOW);
  delay(start_hold);
  digitalWrite(Amanda_Start_GPIO, HIGH);
  //Serial.println("Amanda's Car Started");
  server.send(200, "text/plain", "ok");
  //Handle-restarts with a seperate scheduler
  //15 minute timeout on the vehicle
}

void stopAmanda()
{
  //Serial.println("Stopping Amanda's Car");
  digitalWrite(Amanda_Start_GPIO, LOW);
  delay(start_hold);
  digitalWrite(Amanda_Start_GPIO, HIGH);
  server.send(200);
}

void lockAmanda(){
  //Serial.println("Locking Amanda's Car");
  //Press Lock Twice to get audio feedback
  digitalWrite(Amanda_Lock_GPIO, LOW);
  delay(button_delay);
  digitalWrite(Amanda_Lock_GPIO, HIGH);
  delay(button_off_delay);
  digitalWrite(Amanda_Lock_GPIO, LOW);
  delay(button_delay);
  digitalWrite(Amanda_Lock_GPIO, HIGH);
  delay(button_off_delay);
  digitalWrite(Amanda_Lock_GPIO, LOW);
  delay(button_delay);
  digitalWrite(Amanda_Lock_GPIO, HIGH);
  delay(button_off_delay);
  server.send(200);
}

void unlockAmanda(){
  //Serial.println("Unlocking Amanda's Car");
  //Press unlock Twice to unlock all doors
  digitalWrite(Amanda_Unlock_GPIO, LOW);
  delay(button_delay);
  digitalWrite(Amanda_Unlock_GPIO, HIGH);
  delay(button_off_delay);
  digitalWrite(Amanda_Unlock_GPIO, LOW);
  delay(button_delay);
  digitalWrite(Amanda_Unlock_GPIO, HIGH);
  delay(button_off_delay);
   digitalWrite(Amanda_Unlock_GPIO, LOW);
  delay(button_delay);
  digitalWrite(Amanda_Unlock_GPIO, HIGH);
  delay(button_off_delay);
  server.send(200);
}

void startConrad(){
  //Serial.println("Starting Conrad's Truck");
  //Press Lock Twice
  lockConrad();
  //Press and Hold Start for 3 Seconds
  digitalWrite(Conrad_Start_GPIO, LOW);
  delay(start_hold);
  digitalWrite(Conrad_Start_GPIO, HIGH);
  server.send(200);
  //Handle-restarts with a seperate scheduler
  //15 minute timeout on the vehicle
}

void stopConrad()
{
  //Serial.println("Stopping Conrad's Truck");
  digitalWrite(Conrad_Start_GPIO, LOW);
  delay(start_hold);
  digitalWrite(Conrad_Start_GPIO, HIGH);
  server.send(200);
}

void lockConrad(){
  //Serial.println("Locking Conrad's Truck");
  //Press Lock Twice to get audio feedback
  digitalWrite(Conrad_Lock_GPIO, LOW);
  delay(button_delay);
  digitalWrite(Conrad_Lock_GPIO, HIGH);
  delay(button_off_delay);
  digitalWrite(Conrad_Lock_GPIO, LOW);
  delay(button_delay);
  digitalWrite(Conrad_Lock_GPIO, HIGH);
  delay(button_off_delay);
  digitalWrite(Conrad_Lock_GPIO, LOW);
  delay(button_delay);
  digitalWrite(Conrad_Lock_GPIO, HIGH);
  delay(button_off_delay);
  server.send(200);
}

void unlockConrad(){
  //Serial.println("Unlocking Conrad's Truck");
  //Press Lock Twice to get audio feedback
  digitalWrite(Conrad_Unlock_GPIO, LOW);
  delay(button_delay);
  digitalWrite(Conrad_Unlock_GPIO, HIGH);
  delay(button_off_delay);
  digitalWrite(Conrad_Unlock_GPIO, LOW);
  delay(button_delay);
  digitalWrite(Conrad_Unlock_GPIO, HIGH);
  delay(button_off_delay);
  digitalWrite(Conrad_Unlock_GPIO, LOW);
  delay(button_delay);
  digitalWrite(Conrad_Unlock_GPIO, HIGH);
  delay(button_off_delay);
  server.send(200);
}
