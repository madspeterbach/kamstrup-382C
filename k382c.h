#include "esphome.h"

//
//
// ESPhome custom UART componet for Kamstrup 382c energy meter
// Using the IR Eye from https://wiki.hal9k.dk/projects/kamstrup
// Code heavily inspired by https://github.com/Hal9k-dk/kamstrup/tree/master/Software%20eksempler/kamstrup_powermeter
// Arduino code on above link most likely based on the work by Poul-Henning Kamp (https://github.com/bsdphk/PyKamstrup)
//
//

// Log tag
static const char *TAG = "K382c";

const char* kobisstrings[]   = { "1.8.0","1.6.0" };
const char* kdescstrings[]   = { "TotalEnergyIn","CurrentPower" };
#define NUMREGS 2    			// Number of registers above

float fResultSet[NUMREGS];		// Array to hold the results of the queries of the registers above

// Variables used for onlyu looping queries every 60 seconds 
uint64_t queryPeriod = 10000; // Interval in milliseconds
uint64_t queryInterval = 60000; // Interval in milliseconds
uint64_t time_now =0;
uint64_t timeout =0;
uint64_t next_run =0;


class K382c : public PollingComponent, public UARTDevice
{

public:
  K382c(UARTComponent *parent) : PollingComponent(60000), UARTDevice(parent){};

  byte recvmsg[40];  			// buffer of bytes to hold the received data
  byte rxdata[50];  			// buffer to hold received data
  bool bResultValid[NUMREGS];	// Array to tell if we have valid measurements (after reboot)

  unsigned short kRegCnt; 		// Counter to indicate where we are in the list of registers
  unsigned short iRegRetryCnt;  // Retry counter - per register query

  bool bQueryLoopActive = false; // Flag to indicate whether we are in the process of querying the meter for data
 
  // For each of the values we wish to export, we define a sensor
  Sensor *totEnergyIn_sensor = new Sensor();
  Sensor *CurrentPowerIn_sensor = new Sensor();

  void setup() override
  {
    // ESPHome takes care of setting up the UART for us
    ESP_LOGD(TAG, "Setting up...");
	for(unsigned int x=0;x<NUMREGS;x++) {
		bResultValid[x]=false;
	}

	kRegCnt = 0;			// Reset Kamstrup Register Counter
	bQueryLoopActive = false;	// Query loop not active
	//time_now = (esp_timer_get_time()/1000);		// Set current timestamp
  }

  void loop() override
  {
    time_now = (uint64_t)(esp_timer_get_time()/1000);
    
    if(time_now > next_run && bQueryLoopActive == false) {
      last_run = time_now;  
      bQueryLoopActive = true;
      timeout = time_now + queryPeriod;
      // prompt the meter
      Serial.print("/?!\r\n");
    }
    if(time_now > timeout && bQueryLoopActive == true) {
	bQueryLoopActive = false;
	next_run = time_now + queryInterval;
      }
      
    if(bQueryLoopActive == true) {
       // check if data is available
      if (Serial.available() > 0) {
	// read the incoming string:
	String incomingString = Serial.readStringUntil('\r\n');
	ESP_LOGD(TAG,"Received fra meter: %s",incomingString);
	int paren = incomingString.indexOf("(");
	int aster = incomingString.indexOf("*");
	if(paren != -1 && aster != -1) {
	  int datalen = aster - paren;
	  String obid = incomingString.substring(0,paren);
	  ESP_LOGD(TAG,"OBID: %s",obid);
	  String data = incomingString.substring(paren,datalen);
	  ESP_LOGD(TAG,"data: %s",data);
	  int resIdx = -1;
     
	  if(obid == "1.8.0") {
	    resIdx = 0;
	  }
	  if(obid == "1.6.0") {
	    resIdx = 0;
	  }
	  if(resIdx != -1) {
	    fResultSet[resIdx]=data;
	    bResultValid[resIdx]=true;
	  }
	}
      
      }
    }
  }
  
  void update() override
  {
    // This is the actual sensor reading logic.
    ESP_LOGD(TAG, "Update has been called...");
	unsigned short iCnt = 0;
    
	if(bResultValid[iCnt] == true && fResultSet[iCnt] != 0 ) {
		totEnergyIn_sensor->publish_state(fResultSet[iCnt]);
	} else {
		totEnergyIn_sensor->publish_state(NAN);
	}
	iCnt++;
	if(bResultValid[iCnt] == true) {
		CurrentPowerIn_sensor->publish_state(fResultSet[iCnt]);
	} else {
		CurrentPowerIn_sensor->publish_state(NAN);
	}	

  }

  // private:
};
