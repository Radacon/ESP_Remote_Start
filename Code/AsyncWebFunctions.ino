

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
 
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
 
  server.send(404, "text/plain", message);
}
 
bool loadFromSPIFFS(String path) {
  
  String dataType = "text/html";

  if(path.endsWith(".html")) dataType = "text/html";
  else if(path.endsWith(".htm")) dataType = "text/html";
  else if(path.endsWith(".svg")) dataType = "image/svg+xml";
  else if(path.endsWith(".css")) dataType = "text/css";
  else if(path.endsWith(".js")) dataType = "application/javascript";
  else if(path.endsWith(".png")) dataType = "image/png";
  else if(path.endsWith(".gif")) dataType = "image/gif";
  else if(path.endsWith(".jpg")) dataType = "image/jpeg";
  else if(path.endsWith(".ico")) dataType = "image/x-icon";
  else if(path.endsWith(".xml")) dataType = "text/xml";
  else if(path.endsWith(".pdf")) dataType = "application/pdf";
  else if(path.endsWith(".zip")) dataType = "application/zip";
  
  File dataFile = SPIFFS.open(path.c_str(), "r");
  
  if (server.hasArg("download")) dataType = "application/octet-stream";
  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
  }
 
  Serial.print("Requested page -> ");
  Serial.println(path);
  if (SPIFFS.exists(path)){
      File dataFile = SPIFFS.open(path, "r");
      if (!dataFile) {
          handleNotFound();
          return false;
      }
 
      if (server.streamFile(dataFile, dataType) != dataFile.size()) {
        Serial.println("Sent less data than expected!");
      }else{
          Serial.println("Page served!");
      }
 
      dataFile.close();
  }else{
      handleNotFound();
      return false;
  }
  return true;
}

void printDirectory(File dir, int numTabs) {
  while (true) {
 
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
} 
 
void handleRoot(void) {
    loadFromSPIFFS("/index.html");
}


void schedule_Autostart(void){
    String Temp;
    String SplashMSG = "";
    String hstr, mstr, sstr, ystr, mostr, dstr;
    int h,m,s,y,mo,d;
    long epoch_alarm;
    //Yes I had to do this. Apparently arg(0) is ambiguious. 
    int zero = 0;
    //Each POST request (for scheduling) comes with 3 arguments
    if(server.args()==3){
      //Process Amanda or Conrad's times in the same logic bracket.
      if(server.argName(0)=="AmandaDateStart" || server.argName(0)=="ConradDateStart"){
        Temp = server.arg(1);
        h=Temp.substring(0,2).toInt();
        hstr=Temp.substring(0,2);
        
        m=Temp.substring(3,5).toInt();
        mstr=Temp.substring(3,5);
        s=0;
        sstr="00";
        
        Temp = server.arg(zero);
        
        y=Temp.substring(0,4).toInt();
        ystr=Temp.substring(0,4);
        
        mo=Temp.substring(5,7).toInt();
        mostr=Temp.substring(5,7);
        
        d=Temp.substring(8,10).toInt();
        dstr=Temp.substring(8,10);

        epoch_alarm = convert_to_epoch(d, mo, y, h, m, s);
        SplashMSG = SplashMSG + hstr + ":" + mstr + ":" + sstr + " on " + mostr + "-" + dstr + "-" + ystr;
      }
    }
    //If this request had argument name Amanda, schedule her alarm
    if(server.argName(0)=="AmandaDateStart")
    {
      //Since TimeAlarms does not Support day/month/year for alarm variables
      //We'll use the timer function where we can specify when to execute
      //an action in terms of number of seconds from now.

      //Sanity check to make sure we're at least at 3/24/21 (Day of Creation of this code)
      if(timeClient.getEpochTime()>1616602966)
      {
        AmandaSchedDate = epoch_alarm;
        AmandaSchedDateStr = SplashMSG;
        Serial.println("Amanda is scheduled for " + SplashMSG);
      }
      
    }

    //If this request had argument name Conrad, schedule his alarm
    if(server.argName(0)=="ConradDateStart")
    {
      if(timeClient.getEpochTime()>1616602966)
      {
        ConradSchedDate = epoch_alarm;
        ConradSchedDateStr = SplashMSG;
        Serial.println("Conrad is scheduled for " + SplashMSG);
      }
    }
    //In the future write This Time (epoch and readable) to SPI Flash.
    //Re-instantiate the alarms whenever a reboot happens/power outage. 
    //For now we'll store these as strings in RAM
    handleRoot();
    //server.send(200, "text/plain", ("Autostart scheduled for: "+SplashMSG));
    //Kick the user back to the mainpage after scheduling    
    //loadFromSPIFFS("/index.html");
}

void reboot_esp(void)
{
  loadFromSPIFFS("/reboot.html");
  //Can't use a delay since we need to pop back into the main loop 
  //to serve .css and .svg elements of the reboot page.
  //So we'll schedule a reboot for 5 seconds from now.
  ESP_Reset_Date = timeClient.getEpochTime()+5;
  Serial.println("Rebooting ESP32 (command sent via web interface)");
}

//This is dumb. But it's a patch.
void initHandlers(void)
{
  server.on("/", handleRoot);
  //Scheduling
  server.on("/sched_AF.html",serve_sched_AF);
  server.on("/sched_CF.html",serve_sched_CF);
  server.on("/sched_Submit.html", schedule_Autostart);
  server.on("/clear_Conrad", clear_Conrad_sched);
  server.on("/clear_Amanda", clear_Amanda_sched);
  server.on("/clear_all_sched", clear_all_sched);
  //Vehicle Actions
  server.on("/startAmanda",startAmanda);
  server.on("/stopAmanda",stopAmanda);
  server.on("/lockAmanda",lockAmanda);
  server.on("/unlockAmanda",unlockAmanda);
  
  server.on("/startConrad",startConrad);
  server.on("/stopConrad",stopConrad);
  server.on("/lockConrad",lockConrad);
  server.on("/unlockConrad",unlockConrad);
  //Utilities
  server.on("/reboot.html",reboot_esp); 
  server.on("/get_curr_time", serve_curr_time);
  server.on("/get_rssi", serve_rssi);
  server.on("/update_time", update_time);
  server.on("/get_AF_start_time", serve_AF_start_time);
  server.on("/get_CF_start_time", serve_CF_start_time);
  server.on("/get_CF_rem_time", serve_CF_rem_time);
  server.on("/get_AF_rem_time", serve_AF_rem_time);
  server.on("/get_ram_usage", serve_heap_percentage);
  //CSS
  server.on("/mystyle.css", serve_stylesheet);
  //Icons
  server.on("/lock.svg", serve_lock);
  server.on("/unlock.svg", serve_unlock);
  server.on("/stop.svg", serve_stop);
  server.on("/start.svg", serve_start);
  server.on("/schedule.svg", serve_schedule);
  server.on("/dino.svg", serve_dino);
  server.on("/favicon.ico", serve_favicon);
  server.onNotFound(handleNotFound);
}

void serve_sched_AF(void)
{
  loadFromSPIFFS("/sched_AF.html");
}

void serve_sched_CF(void)
{
  loadFromSPIFFS("/sched_CF.html");
}

void clear_Conrad_sched(void)
{
  Serial.println("Conrad's Scheduled Start Cleared");
  ConradSchedDateStr="";
  ConradSchedDate=0;
  handleRoot();
}

void clear_Amanda_sched(void)
{
  Serial.println("Amanda's Scheduled Start Cleared");
  AmandaSchedDateStr="";
  AmandaSchedDate=0;
  handleRoot();
}

void clear_all_sched(void)
{
  clear_Amanda_sched();
  clear_Conrad_sched();
  handleRoot();
}

void serve_curr_time(void)
{
  server.send(200, "text/plain", timeClient.getFormattedTime());
}

void serve_rssi(void)
{
  String rssi;
  rssi=WiFi.RSSI();
  server.send(200, "text/plain", rssi);
}

void serve_AF_start_time(void)
{
  //If No Alarm is set return the value: "No Start Time Set"
  if(AmandaSchedDateStr=="")
  {
    server.send(200, "text/plain", "No Start Time Set");
  }
  else 
    server.send(200, "text/plain", AmandaSchedDateStr);
}

void serve_CF_start_time(void)
{
  //If No Alarm is set return the value: "No Start Time Set"
  if(ConradSchedDateStr=="")
  {
    server.send(200, "text/plain", "No Start Time Set");
  }
  else 
    server.send(200, "text/plain", ConradSchedDateStr);
}

void serve_stylesheet(void)
{
  loadFromSPIFFS("/mystyle.css");
}

void serve_lock(void)
{
  loadFromSPIFFS("/lock.svg");
}

void serve_dino(void)
{
  loadFromSPIFFS("/dino.svg");
}

void serve_unlock(void)
{
  loadFromSPIFFS("/unlock.svg");
}

void serve_start(void)
{
  loadFromSPIFFS("/start.svg");
}

void serve_stop(void)
{
  loadFromSPIFFS("/stop.svg");
}

void serve_schedule(void)
{
  loadFromSPIFFS("/schedule.svg");
}

void serve_favicon(void)
{
  loadFromSPIFFS("/favicon.ico");
}

void serve_CF_rem_time(void)
{
  int rem_time;
  rem_time = ConradSchedDate - timeClient.getEpochTime();
  String rem_string;
  if(ConradSchedDate == 0)
  {
    server.send(200, "text/plain", "N/A");
  }
  else
  {
    if(rem_time<60)
    {
      rem_string = String(rem_time) + " Seconds";
      server.send(200, "text/plain", rem_string);
    }
    if(rem_time<=3600)
    {
      int minutes = rem_time / 60;
      int seconds = rem_time - (minutes * 60);
      rem_string = String(minutes) + " Minutes " + String(seconds) + " Seconds";
      server.send(200, "text/plain", rem_string);
    }
    if(rem_time>3600)
    {
      int hours, minutes, seconds;
      hours = rem_time / 3600;
      minutes = (rem_time - (hours * 3600))/60;
      seconds = rem_time - (hours * 3600) - (minutes * 60);
      rem_string = String(hours)+ " Hours " + String(minutes) + " Minutes " + String(seconds) + " Seconds";
      server.send(200, "text/plain", rem_string);
    }
  }
}

void serve_AF_rem_time(void)
{
  int rem_time;
  rem_time = AmandaSchedDate - timeClient.getEpochTime();
  String rem_string;
  if(AmandaSchedDate == 0)
  {
    server.send(200, "text/plain", "N/A");
  }
  else
  {
    if(rem_time<60)
    {
      rem_string = String(rem_time) + " Seconds";
      server.send(200, "text/plain", rem_string);
    }
    if(rem_time<=3600)
    {
      int minutes = rem_time / 60;
      int seconds = rem_time - (minutes * 60);
      rem_string = String(minutes) + " Minutes " + String(seconds) + " Seconds";
      server.send(200, "text/plain", rem_string);
    }
    if(rem_time>3600)
    {
      int hours, minutes, seconds;
      hours = rem_time / 3600;
      minutes = (rem_time - (hours * 3600))/60;
      seconds = rem_time - (hours * 3600) - (minutes * 60);
      rem_string = String(hours)+ " Hours " + String(minutes) + " Minutes " + String(seconds) + " Seconds";
      server.send(200, "text/plain", rem_string);
    }
  }
}

void serve_heap_percentage(void)
{
  int heap;
  int heap_pct;
  String heap_STR;
  int total = 81920;
  heap = ESP.getFreeHeap();
  heap = heap * 100;
  heap_pct = heap/total;
  heap_STR=String(heap_pct)+"%";
  server.send(200, "text/plain", heap_STR);
}
