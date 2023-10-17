struct GpsDataStruct
{
    double lat, lon, alt, speed;
    uint16_t hdop;
    uint16_t year;
    uint8_t month, day, hour, min, sec, csec;
    uint8_t sat;
    bool valid = false;
};

String getJSONgpsData(GpsDataStruct data)
{
    char c[20];
    String s = "{";
    snprintf(c, 20, "\"lat\": %f ,", data.lat);
    s += c;
    snprintf(c, 20, "\"lon\": %f", data.lon);
    s += c;
    s += "}";
    return s;
}