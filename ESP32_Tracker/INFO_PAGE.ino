void handleInfo(){
  byte mac[6];

/*
  SerialOutParams();
  
  for (uint8_t j=0; j<server.args(); j++){
  }
*/
  
  SendHTTPHeader();
  ghks.MyIP =  WiFi.localIP() ;
  server.sendContent(F("<br><center><b>Node Info</b><br>"));
  server.sendContent(F("<table border=1 title='Device Info'>"));
//  server.sendContent("<tr><td>ESP ID</td><td align=center>0x" + String(ESP.getChipId(), HEX) + "</td><td align=center>"+String(ESP.getChipId())+"</td></tr>" ) ; 
  ghks.MyIP =  WiFi.localIP() ;
  snprintf(buff, BUFF_MAX, "%03u.%03u.%03u.%03u", ghks.MyIP[0],ghks.MyIP[1],ghks.MyIP[2],ghks.MyIP[3]);
  server.sendContent("<tr><td>Network Node IP Address</td><td align=center>" + String(buff) + "</td><td>.</td></tr>" ) ; 
  ghks.MyIPC = WiFi.softAPIP();  // get back the address to verify what happened  
  snprintf(buff, BUFF_MAX, "%03u.%03u.%03u.%03u", ghks.MyIPC[0],ghks.MyIPC[1],ghks.MyIPC[2],ghks.MyIPC[3]);
  server.sendContent("<tr><td>Config Node IP Address</td><td align=center>" + String(buff) + "</td><td>.</td></tr>" ) ; 
  server.sendContent("<tr><td>Time Server</td><td align=center>" + String(ghks.timeServer) + "</td><td>.</td></tr>" ) ; 
  server.sendContent("<tr><td>Network SSID</td><td align=center>" + String(ghks.nssid) + "</td><td>.</td></tr>" ) ; 
  server.sendContent("<tr><td>Network Password</td><td align=center>" + String(ghks.npassword) + "</td><td>.</td></tr>" ) ; 
  server.sendContent("<tr><td>Configure SSID</td><td align=center>" + String(ghks.cssid) + "</td><td>.</td></tr>" ) ; 
  server.sendContent("<tr><td>Configure Password</td><td align=center>" + String(ghks.cpassword) + "</td><td>.</td></tr>" ) ; 
  
  server.sendContent("<tr><td>WiFi RSSI</td><td align=center>" + String(WiFi.RSSI()) + "</td><td>(dBm)</td></tr>" ) ; 
  WiFi.macAddress(mac);      
  snprintf(buff, BUFF_MAX, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  server.sendContent("<tr><td>MAC Address</td><td align=center>" + String(buff) + "</td><td align=center>.</td></tr>" ) ; 
 
  server.sendContent("<tr><td>Last Scan Speed</td><td align=center>" + String(lScanLast) + "</td><td>(per second)</td></tr>" ) ;    

//  server.sendContent("<tr><td>ESP Core Version</td><td align=center>" + String(ESP.getCoreVersion()) + "</td><td>.</td></tr>" ) ;    
//  server.sendContent("<tr><td>ESP Full Version</td><td align=center>" + String(ESP.getFullVersion()) + "</td><td>.</td></tr>" ) ;    
  server.sendContent("<tr><td>SDK Version</td><td align=center>" + String(ESP.getSdkVersion()) + "</td><td>.</td></tr>" ) ;    
//  server.sendContent("<tr><td>CPU Volts</td><td align=center>" + String(ESP.getVcc()) + "</td><td>(V)</td></tr>" ) ;    
  server.sendContent("<tr><td>CPU Frequecy</td><td align=center>" + String(ESP.getCpuFreqMHz()) + "</td><td>(MHz)</td></tr>" ) ;    
//  server.sendContent("<tr><td>Get Rest Reason</td><td align=center>" + String(ESP.getResetReason()) + "</td><td></td></tr>" ) ;    
//  server.sendContent("<tr><td>Get Reset Into</td><td align=center>" + String(ESP.getResetInfo()) + "</td><td></td></tr>" ) ;    
//  server.sendContent("<tr><td>Get Sketch Size</td><td align=center>" + String(ESP.getSketchSize()) + "</td><td>(Bytes)</td></tr>" ) ;    
//  server.sendContent("<tr><td>Free Sketch Space</td><td align=center>" + String(ESP.getFreeSketchSpace()) + "</td><td>(Bytes)</td></tr>" ) ;    

  server.sendContent(F("</table><br>"));    
  SendHTTPPageFooter();
}



void handleSetup() {
  boolean currentLineIsBlank = true;
  tmElements_t tm;
  long  i = 0 ;
  int ii  ;
  int iProgNum = 0;
  int j ;
  int k , kk , iTmp ;
  boolean bExtraValve = false ;
  uint8_t iPage = 1 ;
  boolean bDefault = true ;
//  int td[6];
  long lTmp ; 
  String MyCheck , MyColor   ;
  byte mac[6];
  String message = "" ;

//  SerialOutParams();

  for (uint8_t j=0; j<server.args(); j++){
    bSaveReq = 1 ;
    i = String(server.argName(j)).indexOf("command");
    if (i != -1){  // 
      switch (String(server.arg(j)).toInt()){
        case 1:  // load values
          tv.iDoSave = 3 ;
        break;
        case 2: // Save values
          tv.iDoSave = 2 ;
        break;
        case 3: // Save the running max
          bMagCal = false ;
          compass.m_min = running_min ;
          tv.mag_min = running_min ;
          compass.m_max = running_max ;
          tv.mag_max = running_max ;    
          tv.iDoSave = 3 ;              // also que the eeprom save 
        break;
        case 4: // start stop mag cal mode
          bMagCal = !bMagCal ;
        break;
        case 5: // reset the calbiration of magnetometer
          tv.mag_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
          tv.mag_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};
          compass.m_min = tv.mag_min ;
          compass.m_max = tv.mag_max ;    
          running_min = {32767, 32767, 32767} ;
          running_max = {-32768, -32768, -32768};                      
          bMagCal = true ;
        break;
        case 8: //  Cold Reboot
        break;
        case 9: //  Warm Reboot
          ESP.restart() ;
        break;
        case 667: // wipe the memory to factory default
          BackIntheBoxMemory();
        break;
        case 665:
          sendNTPpacket(ghks.timeServer); // send an NTP packet to a time server  once and hour  
        break;
        case 668:
        break;
      }  
    }
    i = String(server.argName(j)).indexOf("reboot");
    if (i != -1){  // 
      if (( lRebootCode == String(server.arg(j)).toInt()) && ( lRebootCode > 0 )){  // stop the phone browser being a dick and retry resetting !!!!
        ESP.restart() ;        
      }
    }
        
    
    i = String(server.argName(j)).indexOf("nssid");    // ssid setup
    if (i != -1){  
      String(server.arg(j)).toCharArray(ghks.nssid,sizeof(ghks.nssid));
    }        
    i = String(server.argName(j)).indexOf("npass");   // password setup
    if (i != -1){  // have a request to set the time zone
      String(server.arg(j)).toCharArray(ghks.npassword,sizeof(ghks.npassword));
    }        
    i = String(server.argName(j)).indexOf("cssid");    // ssid setup
    if (i != -1){  
      String(server.arg(j)).toCharArray(ghks.cssid,sizeof(ghks.cssid));
    }        
    i = String(server.argName(j)).indexOf("cpass");   // password setup
    if (i != -1){  // have a request to set the time zone
      String(server.arg(j)).toCharArray(ghks.cpassword,sizeof(ghks.cpassword));
    }        

    i = String(server.argName(j)).indexOf("naddr");   // ip address setup
    if (i != -1){  // have a request to set the time zone
      ghks.MyIP[0] = String(server.arg(j)).substring(0,3).toInt() ;
      ghks.MyIP[1] =String(server.arg(j)).substring(4,7).toInt() ;
      ghks.MyIP[2] = String(server.arg(j)).substring(8,11).toInt() ;
      ghks.MyIP[3] =String(server.arg(j)).substring(12,15).toInt() ;
    }               

    i = String(server.argName(j)).indexOf("tzone");
    if (i != -1){  // 
      ghks.fTimeZone = String(server.arg(j)).toFloat() ;
      ghks.fTimeZone = constrain(ghks.fTimeZone,-12,12);
      bDoTimeUpdate = true ; // trigger and update to fix the time
    }
    i = String(server.argName(j)).indexOf("timsv");
    if (i != -1){                                    // timesvr
     String(server.arg(j)).toCharArray( ghks.timeServer , sizeof(ghks.timeServer)) ;
    }    
    i = String(server.argName(j)).indexOf("lpntp");
    if (i != -1){  // 
      ghks.localPort = String(server.arg(j)).toInt() ;
      ghks.localPort = constrain(ghks.localPort,1,65535);
    }        
    i = String(server.argName(j)).indexOf("lpctr");
    if (i != -1){  // 
      ghks.localPortCtrl = String(server.arg(j)).toInt() ;
      ghks.localPortCtrl = constrain(ghks.localPortCtrl,1,65535);
    }        
    i = String(server.argName(j)).indexOf("rpctr");
    if (i != -1){  // 
      ghks.RemotePortCtrl = String(server.arg(j)).toInt() ;
      ghks.RemotePortCtrl = constrain(ghks.RemotePortCtrl,1,65535);
    }        

    
  }          

  SendHTTPHeader();
  
  server.sendContent(F("<link rel='icon' href='data:,'></head><body><html><center><h2>"));
  server.sendContent(String(tv.trackername).substring(0,16)+" Solar Tracker</h2>");

  server.sendContent(F("<a href='/'>Refresh</a><br><br>")) ;   
  if ( bSaveReq != 0 ){
    server.sendContent(F("<blink>"));      
  }
  server.sendContent(F("<a href='/?command=2'>Save Parameters to EEPROM</a><br>")) ;         

  snprintf(buff, BUFF_MAX, "%d/%02d/%02d %02d:%02d:%02d", year(), month(), day() , hour(), minute(), second());
  server.sendContent("<b>"+ String(buff)) ; 
  if ( year() < 2018 ) {
    server.sendContent(F("--- CLOCK NOT SET ---")) ;
  }
  server.sendContent(F("</b><br>")) ;
  
  if ( bSaveReq != 0 ){
    server.sendContent(F("</blink><font color='red'><b>Changes Have been made to settings.<br>Make sure you save if you want to keep them</b><br></font>")) ;     
  }
  server.sendContent(F("<br>")) ;


  
  bDefault = false ;
  server.sendContent("<table border=1 title='Node Settings'>");
  server.sendContent(F("<tr><th>Parameter</th><th>Value</th><th></th></tr>"));

  snprintf(buff, BUFF_MAX, "%04d/%02d/%02d %02d:%02d", year(AutoOff_t), month(AutoOff_t), day(AutoOff_t) , hour(AutoOff_t), minute(AutoOff_t));
  if (AutoOff_t > now()){
    MyColor =  F("bgcolor=red") ;
  }else{
    MyColor =  "" ;
  }
  server.sendContent(F("<tr><td>Time Zone</td><td align=center>")) ; 
  server.sendContent("<form method=get action=" + server.uri() + "><input type='text' name='tzone' value='" + String(ghks.fTimeZone) + "' size=16></td><td><input type='submit' value='SET'></form></td></tr>");

  server.sendContent(F("<tr><td>Local UDP Port NTP</td><td align=center>")) ; 
  server.sendContent("<form method=get action=" + server.uri() + "><input type='text' name='lpntp' value='" + String(ghks.localPort) + "' size=16></td><td><input type='submit' value='SET'></form></td></tr>");

  server.sendContent(F("<tr><td>Local UDP Port Control</td><td align=center>")) ; 
  server.sendContent("<form method=get action=" + server.uri() + "><input type='text' name='lpctr' value='" + String(ghks.localPortCtrl) + "' size=16></td><td><input type='submit' value='SET'></form></td></tr>");

  server.sendContent(F("<tr><td>Remote UDP Port Control</td><td align=center>")) ; 
  server.sendContent("<form method=get action=" + server.uri() + "><input type='text' name='rpctr' value='" + String(ghks.RemotePortCtrl) + "' size=16></td><td><input type='submit' value='SET'></form></td></tr>");
  
  server.sendContent(F("<tr><td>Network SSID</td><td align=center>")) ; 
  server.sendContent("<form method=get action=" + server.uri() + "><input type='text' name='nssid' value='" + String(ghks.nssid) + "' maxlength="+String(sizeof(ghks.nssid))+" size=16></td><td><input type='submit' value='SET'></form></td></tr>");
  server.sendContent(F("<tr><td>Network Password</td><td align=center>")) ; 
  server.sendContent("<form method=get action=" + server.uri() + "><input type='text' name='npass' value='" + String(ghks.npassword) + "' maxlength="+String(sizeof(ghks.npassword))+" size=16></td><td><input type='submit' value='SET'></form></td></tr>");

  server.sendContent(F("<tr><td>Config SSID</td><td align=center>")) ; 
  server.sendContent("<form method=get action=" + server.uri() + "><input type='text' name='cssid' value='" + String(ghks.cssid) + "' maxlength="+String(sizeof(ghks.cssid))+" size=16></td><td><input type='submit' value='SET'></form></td></tr>");
  server.sendContent(F("<tr><td>Config Password</td><td align=center>")) ; 
  server.sendContent("<form method=get action=" + server.uri() + "><input type='text' name='cpass' value='" + String(ghks.cpassword) + "' maxlength="+String(sizeof(ghks.cpassword))+" size=16></td><td><input type='submit' value='SET'></form></td></tr>");

  server.sendContent(F("<tr><td>Time Server</td><td align=center>")) ; 
  server.sendContent("<form method=get action=" + server.uri() + "><input type='text' name='timsv' value='" + String(ghks.timeServer) + "' maxlength=23 size=16></td><td><input type='submit' value='SET'></form></td></tr>");

//    server.sendContent("<tr><td>ESP ID</td><td align=center>0x" + String(ESP.getChipId(), HEX) + "</td><td align=center>"+String(ESP.getChipId())+"</td></tr>" ) ; 
  WiFi.macAddress(mac);      
  snprintf(buff, BUFF_MAX, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  server.sendContent("<tr><td>MAC Address</td><td align=center>" + String(buff) + "</td><td align=center>.</td></tr>" ) ; 
  server.sendContent("<tr><td>WiFi RSSI</td><td align=center>" + String(WiFi.RSSI()) + "</td><td>(dBm)</td></tr>" ) ; 
  ghks.MyIP =  WiFi.localIP() ;    
  snprintf(buff, BUFF_MAX, "%03u.%03u.%03u.%03u", ghks.MyIP[0],ghks.MyIP[1],ghks.MyIP[2],ghks.MyIP[3]);
  server.sendContent("<tr><td>Node IP Address</td><td align=center>" + String(buff) + "</td><td>.</td></tr>" ) ; 
  server.sendContent("<tr><td>Last Scan Speed</td><td align=center>" + String(lScanCtr) + "</td><td>(per second)</td></tr>" ) ;    
  if( hasRTC ){
    rtc_status = DS3231_get_sreg();
    if (( rtc_status & 0x80 ) != 0 ){
      server.sendContent(F("<tr><td>RTC Battery</td><td align=center bgcolor='red'>DEPLETED</td>"));
      server.sendContent("<td><form method=get action=" + server.uri() + "><input type='hidden' name='rtcbf' value='1'><input type='submit' value='RESET'></form></td>");
      server.sendContent(F("</tr>")) ;            
    }else{
      server.sendContent(F("<tr><td>RTC Battery</td><td align=center bgcolor='green'>-- OK --</td><td></td></tr>"));
    }
    server.sendContent("<tr><td>RTC Temperature</td><td align=center>"+String(rtc_temp,1)+"</td><td>(C)</td></tr>") ;                    
  }
  server.sendContent(F("</form></table>"));
  
  

  SendHTTPPageFooter();
}

