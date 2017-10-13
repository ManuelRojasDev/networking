#include <arduino.h>
#define _NETWORKING_H


class networking{
	public:
	bool debug = true; // print out what is happening
	
	// SSID 
	String SSID = "AFADA";// ssid
	String PASSWORD = "internet2016";// your network password
	bool connect(String ssid,String password);// connect to netwrok
	bool connect();// connected to network
	
	
	//SERVER
	String SERVER = "dweet.io";
	uint16_t PORT = 80;// server port
	TCPClient client;
	bool GET_time(String &date, String &time);//GET http method, wait and save response
	bool POST(String data);// POST http method, just post, no response expected
	bool WEB(String data, String method);// main post / get function
	unsigned long GET_timeout = 300;// get timeout in millis

	
	
	
	
	
	
	IPAddress localIP;
	private:
		unsigned long timer;// get timer
	
	
	
	
	
	
	
};




