#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

TinyGPSPlus gps;         // The TinyGPS++ object
SoftwareSerial ss(4, 5); // The serial connection to the GPS device

const char *ssid = "MNSLXGUEST";
const char *password = "batataadrkill";

float latitude, longitude;
int year, month, date, hour, minute, second;
String date_str, time_str, lat_str, lng_str, sat_str;
int pm;

WiFiServer server(80);

void setup()
{
    Serial.begin(115200);
    ss.begin(9600);

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    server.begin();
    Serial.println("Server started");
    Serial.println(WiFi.localIP());
}

void loop()
{
    while (ss.available() > 0)
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
    client.print(s);
    delay(100);
}
