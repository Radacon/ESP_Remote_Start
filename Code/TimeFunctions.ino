void setLocalTime()
{
}

void printTime()
{
  //Serial.print(daysOfTheWeek[timeClient.getDay()]);
  //Serial.print(", ");
  //Serial.print(timeClient.getHours());
  //Serial.print(":");
  //Serial.print(timeClient.getMinutes());
  //Serial.print(":");
  //Serial.println(timeClient.getSeconds());
}

int convert_to_epoch(int d, int mo, int y, int h, int m, int s)
{
  long curr_epoch;
  //Okay so number of years since 1970
  y=y-1970;
  //Number of Months Since January (Jan=0) OK
  mo=mo-1;
  //Number of Days since the 1st (Day 1 of a month = -1???) Not sure why the rest works
  d=d-2;
  //Everything below this was custom trimmed.
  //Can't tell you why. getEpoch returns the offset epoch time (GMT -6 for my region)
  //The numbers for the time are parsing through correctly. HMPH
  h=h-5;
  m=m-23;
  s=s-132;
  curr_epoch = 0;
  curr_epoch = curr_epoch + (y*31556926);
  curr_epoch = curr_epoch + (mo*2629743);
  curr_epoch = curr_epoch + (d*86400);
  curr_epoch = curr_epoch + (h*3600);
  curr_epoch = curr_epoch + (m*60);
  curr_epoch = curr_epoch + (s);
  return curr_epoch;
}

void checkforalarms(void)
{
  
  if(AmandaSchedDate>0)
  {
    if(timeClient.getEpochTime()>=AmandaSchedDate)
    {
      //Serial.println("Starting Amanda's Car");
      startAmanda();
      //Serial.println("Amanda's Car Started");
      AmandaSchedDate=0;
      AmandaSchedDateStr="";
    }
  }

  if(ConradSchedDate>0)
  {
    if(timeClient.getEpochTime()>=ConradSchedDate)
    {
      //Serial.println("Starting Conrad's Truck");
      startConrad();
      //Serial.println("Conrad's Truck Started");
      ConradSchedDate=0;
      ConradSchedDateStr = "";
    }
  }

  if(ESP_Reset_Date>0)
  {
    if(timeClient.getEpochTime()>=ESP_Reset_Date)
    {
      ESP.restart();
    }
  }
}

void update_time(void)
{
  timeClient.forceUpdate();
  handleRoot();
}
