const int which = 2;
IPAddress ip2 = IPAddress(192, 168, 1, 103);
IPAddress ip1 = IPAddress(192, 168, 43, 247);
IPAddress ip0 = IPAddress(192, 168, 43, 75);
IPAddress *ip[] = {&ip0, &ip1, &ip2};

const char *ssid[] = {"Oneplus 6", "AndroidAP", "TP-Link_DC90"};
const char *password[] = {"Lacika007", "Nagyponty21", "53745309"};

void connectWifi(int which)
{
  WiFi.mode(WIFI_STA);
  WiFi.hostname("Csonak_ESP");
  WiFi.begin(ssid[which], password[which]);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
  }
  gateWay = WiFi.gatewayIP();
  long rssi = WiFi.RSSI();
  Serial.print("IP: ");
  Serial.println(WiFi.gatewayIP().toString());
}

int32_t signalStrenght()
{
  return WiFi.RSSI();
}