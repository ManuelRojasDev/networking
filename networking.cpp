#include <networking.h>
#include <String.h>


bool networking::connect(String ssid,String password){// connect to netwrok
	SSID = ssid;
	PASSWORD = password;
	
	return connect();
}

bool networking::connect(){// connect to network;
	int tries;
	bool state = true, state2 = true;
	
	if(debug){
		Serial.print("\nAttempting to connect to Network named: "+ String(SSID)+" with password: "+String(PASSWORD));// print action
	}
	
	WiFi.on();
	delay(1);
	WiFi.setCredentials(SSID, PASSWORD);
	WiFi.connect();
	delay(1);
	
	
	while (WiFi.connecting()) {// attempt to connect
    if(debug){Serial.print(String(tries)+".");}
	
    delay(300);
    if(tries > 15){
		if(debug){ Serial.println("\nConnection to SSID: "+SSID+" with password: "+PASSWORD+" failed");}
		state = false;
		break;
    }else{
		tries++;
	}
	
  }
  
  
  
  
  if(state){// get IP address
	tries = 0;
	if(debug){Serial.println("\nConnection to SSID: "+SSID+" with password: "+PASSWORD+" Successful");}
  
	localIP = WiFi.localIP();
	
	while (localIP[0] == 0) {
		localIP = WiFi.localIP();
		
		delay(100);
		if( tries < 15){
			tries++;
		}else{
			
			state2 = false;
			break;
		}
	}
	
	if(debug){
		if(state2){
			// print the received IP address
			Serial.print("\nIP address uptained: ");
			Serial.println(localIP);
			
			// print the received signal strength:
			long rssi = WiFi.RSSI();
			Serial.print("signal strength (RSSI):");
			Serial.print(rssi);
			Serial.println(" dBm");
		}else{
			Serial.println("\nUnable to uptain IP address");
			}
		}
		
		
	}
	  
  
  
  
  return state;
}



bool networking::WEB(String data, String method){
	bool state = true;
	uint8_t tries;

	while (!client.connect(SERVER, PORT)) {
    if(debug){ Serial.print(String(tries)+".");}
	

	if(tries++ > 100){
		if(debug){ 
			Serial.println("\nThe server isn't responding");
		}
		state = false;
		break;
		}
		
    }
	
	if(state){// post or get
		client.println(method+" "+data);
		client.println();
	}
  
  return state;
}

bool networking::GET_time(String &date, String &time){// get data and wait for responce
	String data;
	bool status;

	
	if(WEB("/", "POST")){
		delay(1);// wait for data
		timer = millis();
		

		while(client.available() || millis() - timer < GET_timeout){
			data += char(client.read());
				
			if(data.indexOf(" GMT") > -1){
				status = true;
				client.stop();
				break;
			}
		}
			
			
		
		
		if(status){
			// date comes int his form Date: Thu, 12 Oct 2017 19:35:25 GMT

			
			String p;// temp string
			uint16_t day, month, year;
			
			// extract date and time 
			data = data.substring(data.indexOf("Date:") + 7, data.indexOf("GMT"));
			data = data.substring(data.indexOf(", ") + 1, data.indexOf("GMT"));
			data.trim();
			if(debug){Serial.println("date obtaines: "+data);}
			
			//parse day
			day = (data.substring(0, data.indexOf(" "))).toInt();
			data = data.substring(data.indexOf(" ") + 1);
			data.trim();
			
			// parse month number
			p = ((data.substring(0, data.indexOf(" "))).trim()).toLowerCase();
			data = data.substring(data.indexOf(" ") + 1);

			uint16_t month_number = (uint8_t)p.charAt(0) + (uint8_t)p.charAt(1) + (uint8_t)p.charAt(2);
			uint16_t month_ascii[13] ={(uint16_t)char('j') + (uint16_t)char('a') + (uint16_t)char('n'),
										(uint16_t)char('f') + (uint16_t)char('e') + (uint16_t)char('b'),
										(uint16_t)char('m') + (uint16_t)char('a') + (uint16_t)char('r'),
										(uint16_t)char('a') + (uint16_t)char('p') + (uint16_t)char('r'),
										(uint16_t)char('m') + (uint16_t)char('a') + (uint16_t)char('y'),
										(uint16_t)char('j') + (uint16_t)char('u') + (uint16_t)char('n'),
										(uint16_t)char('j') + (uint16_t)char('u') + (uint16_t)char('l'),
										(uint16_t)char('a') + (uint16_t)char('u') + (uint16_t)char('g'),
										(uint16_t)char('s') + (uint16_t)char('e') + (uint16_t)char('p'),
										(uint16_t)char('o') + (uint16_t)char('c') + (uint16_t)char('t'),
										(uint16_t)char('n') + (uint16_t)char('o') + (uint16_t)char('v'),
										(uint16_t)char('d') + (uint16_t)char('e') + (uint16_t)char('c')
										};
			for(uint8_t i = 0; i < 12; i++){
				if( month_number == month_ascii[i]){
					month_number = i + 1;
				}
			}
			
			//parse year
			year = (data.substring(0, data.indexOf(" "))).toInt();
			data = data.substring(data.indexOf(" "));
			
			// parse time
			time = data.trim();
			
			date = String(year)+"-"+String(month_number)+"-"+String(day);
			
			if(debug){Serial.println("Date parsed: "+date+" "+time);}

			
			
		}
		
		
	}
	
	client.stop();
	return status;
}

bool networking::POST(String data){// post data
	bool status = true;;
	if(debug){Serial.println("HTTP POST "+SERVER+data);}
	if(WEB(data, "POST")){
		if(debug){Serial.println("HTTP POST succeeded");}
	}else{
		if(debug){Serial.println("HTTP POST failed");}
		status = false;
	}
	client.stop();

	return status;
}
