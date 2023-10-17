const int which = 3;
IPAddress ip2 = IPAddress(192, 168, 1, 103);
IPAddress ip1 = IPAddress(192, 168, 43, 247);
IPAddress ip0 = IPAddress(192, 168, 43, 75);
IPAddress *ip[] = {&ip0, &ip1, &ip2};

const char *ssid[] = {"Oneplus 6", "AndroidAP", "TP-Link_DC90", "KossuthNet"};
const char *password[] = {"Lacika007", "Nagyponty21", "53745309", "InterNyet17"};

void connectWifi()
{
  WiFiClass::mode(WIFI_STA);
  WiFiClass::setHostname("Csonak_ESP");

  WiFi.setAutoConnect(true);
  WiFi.begin(ssid[which], password[which]);
  Serial.println("Connecting...");
  while (WiFiClass::status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  gateWay = WiFi.gatewayIP();
  //long rssi = WiFi.RSSI();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP().toString());
  Serial.print("Csatlakozva: ");
  Serial.println(ssid[which]);
}
