#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

char ssid[] = "test12";      //  your network SSID (name) between the " "
char pass[] = "aeroporto";   // your network password between the " "
int keyIndex = 0;            // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS; // connection status
WiFiServer server(80);       // server socket

float latitude, longitude;
int year, month, date, hour, minute, second;
String date_str, time_str, lat_str, lng_str, sat_str;
int pm;

WiFiClient client = server.available();

void setup()
{
    WiFiDrv::pinMode(25, OUTPUT);
    WiFiDrv::pinMode(26, OUTPUT);
    WiFiDrv::pinMode(27, OUTPUT);

    Serial.begin(9600);
    while (!Serial)
        ;

    enable_WiFi();
    connect_WiFi();

    server.begin();
    printWifiStatus();
}

void loop()
{
    client = server.available();

    if (client)
    {
        printWEB();
    }
}

void printWifiStatus()
{
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your board's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");

    Serial.print("To see this page in action, open a browser to http://");
    Serial.println(ip);
}

void enable_WiFi()
{
    // check for the WiFi module:
    if (WiFi.status() == WL_NO_MODULE)
    {
        Serial.println("Communication with WiFi module failed!");
        // don't continue
        while (true)
            ;
    }

    String fv = WiFi.firmwareVersion();
    if (fv < "1.0.0")
    {
        Serial.println("Please upgrade the firmware");
    }
}

void connect_WiFi()
{
    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);

        // wait 10 seconds for connection:
        delay(2000);
    }
}

void printWEB()
{

    if (client)
    {                                 // if you get a client,
        Serial.println("new client"); // print a message out the serial port
        String currentLine = "";      // make a String to hold incoming data from the client
        while (client.connected())
        { // loop while the client's connected
            if (client.available())
            {                           // if there's bytes to read from the client,
                char c = client.read(); // read a byte, then
                Serial.write(c);        // print it out the serial monitor
                if (c == '\n')
                { // if the byte is a newline character

                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0)
                    {

                        if (gps.encode(ss.read()))
                        {
                            if (gps.location.isValid())
                            {
                                latitude = gps.location.lat();
                                lat_str = String(latitude, 6);
                                longitude = gps.location.lng();
                                lng_str = String(longitude, 6);
                            }
                            if (gps.satellites.isValid())
                            {
                                sat_str = String(gps.satellites.value());
                            }

                            if (gps.date.isValid())
                            {
                                date_str = "";
                                date = gps.date.day();
                                month = gps.date.month();
                                year = gps.date.year();

                                date_str += (date < 10) ? "0" : "";
                                date_str += String(date) + " / ";
                                date_str += (month < 10) ? "0" : "";
                                date_str += String(month) + " / ";
                                date_str += (year < 10) ? "0" : "";
                                date_str += String(year);
                            }

                            if (gps.time.isValid())
                            {
                                time_str = "";
                                hour = gps.time.hour();
                                minute = gps.time.minute() + 30;
                                second = gps.time.second();

                                if (minute > 59)
                                {
                                    minute -= 60;
                                    hour++;
                                }

                                hour = (hour + 5) % 24;
                                pm = (hour >= 12) ? 1 : 0;
                                hour %= 12;

                                time_str += (hour < 10) ? "0" : "";
                                time_str += String(hour) + " : ";
                                time_str += (minute < 10) ? "0" : "";
                                time_str += String(minute) + " : ";
                                time_str += (second < 10) ? "0" : "";
                                time_str += String(second);
                                time_str += (pm == 1) ? " PM " : " AM ";
                            }
                        }
                    }

                    // Check if a client has connected
                    WiFiClient client = server.available();
                    if (!client)
                    {
                        return;
                    }

                    // Prepare the response
                    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html> <html> <head> <title>GPS Interfacing with NodeMCU</title> <style>";
                    s += "a:link {background-color: YELLOW;text-decoration: none;}";
                    s += "table, th, td {border: 1px solid black;} </style> </head> <body> <h1 style=\"font-size:300%; ALIGN=CENTER\"> GPS Interfacing with NodeMCU</h1>";
                    s += "<p ALIGN=CENTER style=\"font-size:150%;\"> <b>Location Details</b></p> <table ALIGN=CENTER style=\"width:50%\">";
                    s += "<tr> <th>Latitude</th> <td ALIGN=CENTER >" + lat_str + "</td> </tr>";
                    s += "<tr> <th>Longitude</th> <td ALIGN=CENTER >" + lng_str + "</td> </tr>";
                    s += "<tr> <th>Date</th> <td ALIGN=CENTER >" + date_str + "</td></tr>";
                    s += "<tr> <th>Time</th> <td ALIGN=CENTER >" + time_str + "</td> </tr>";
                    s += "<tr> <th>Satellites</th> <td ALIGN=CENTER >" + sat_str + "</td> </tr>";
                    s += "</table>";

                    if (gps.location.isValid())
                    {
                        s += "<p align=center><a style=\"color:RED;font-size:125%;\" href=\"http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=" + lat_str + "+" + lng_str + "\" target=\"_top\">Click here!</a> To check the location in Google maps.</p>";
                    }

                    s += "</body> </html> \n";
                }
                else
                { // if you got a newline, then clear currentLine:
                    currentLine = "";
                }
            }
            else if (c != '\r')
            {                     // if you got anything else but a carriage return character,
                currentLine += c; // add it to the end of the currentLine
            }
        }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
}

void led(int level, int r, int g, int b)
{
    WiFiDrv::analogWrite(26, level * r); // r
    WiFiDrv::analogWrite(25, level * g); // g
    WiFiDrv::analogWrite(27, level * b); // b
}