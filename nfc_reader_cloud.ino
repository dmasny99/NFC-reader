// MQTT and WIFI libs
#include <ESP8266WiFi.h>
#include <PubSubClient.h> 
// SPI libs and PN532 libs
#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>
#include <NfcAdapter.h> 
// wifi settings setup
const char* ssid = "Masn_ural";
const char* password = "xcGXFE2r";
// yandex cloud settings
const char* mqttserver = "130.193.44.244"; //mqtt.cloud.yandex.net
const char* yandexIoTCoredeviceId = "areslq41mged71mcd3be";
const char* mqttpassword = "Passwordpassword123";
const int mqttport=8883;


String warehouse_topic_test = "$devices/areslq41mged71mcd3be/events/test";
String test_topic = "$devices/areslq41mged71mcd3be/events/test";
String topicCommands = String("$devices/")+String(yandexIoTCoredeviceId)+String("/commands");
String topicEvents = String("$devices/")+String(yandexIoTCoredeviceId)+String("/events");

const char* test_root_ca = \
"-----BEGIN CERTIFICATE-----\n \
MIIFGTCCAwGgAwIBAgIQJMM7ZIy2SYxCBgK7WcFwnjANBgkqhkiG9w0BAQ0FADAf\
MR0wGwYDVQQDExRZYW5kZXhJbnRlcm5hbFJvb3RDQTAeFw0xMzAyMTExMzQxNDNa\
Fw0zMzAyMTExMzUxNDJaMB8xHTAbBgNVBAMTFFlhbmRleEludGVybmFsUm9vdENB\
MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAgb4xoQjBQ7oEFk8EHVGy\
1pDEmPWw0Wgw5nX9RM7LL2xQWyUuEq+Lf9Dgh+O725aZ9+SO2oEs47DHHt81/fne\
5N6xOftRrCpy8hGtUR/A3bvjnQgjs+zdXvcO9cTuuzzPTFSts/iZATZsAruiepMx\
SGj9S1fGwvYws/yiXWNoNBz4Tu1Tlp0g+5fp/ADjnxc6DqNk6w01mJRDbx+6rlBO\
aIH2tQmJXDVoFdrhmBK9qOfjxWlIYGy83TnrvdXwi5mKTMtpEREMgyNLX75UjpvO\
NkZgBvEXPQq+g91wBGsWIE2sYlguXiBniQgAJOyRuSdTxcJoG8tZkLDPRi5RouWY\
gxXr13edn1TRDGco2hkdtSUBlajBMSvAq+H0hkslzWD/R+BXkn9dh0/DFnxVt4XU\
5JbFyd/sKV/rF4Vygfw9ssh1ZIWdqkfZ2QXOZ2gH4AEeoN/9vEfUPwqPVzL0XEZK\
r4s2WjU9mE5tHrVsQOZ80wnvYHYi2JHbl0hr5ghs4RIyJwx6LEEnj2tzMFec4f7o\
dQeSsZpgRJmpvpAfRTxhIRjZBrKxnMytedAkUPguBQwjVCn7+EaKiJfpu42JG8Mm\
+/dHi+Q9Tc+0tX5pKOIpQMlMxMHw8MfPmUjC3AAd9lsmCtuybYoeN2IRdbzzchJ8\
l1ZuoI3gH7pcIeElfVSqSBkCAwEAAaNRME8wCwYDVR0PBAQDAgGGMA8GA1UdEwEB\
/wQFMAMBAf8wHQYDVR0OBBYEFKu5xf+h7+ZTHTM5IoTRdtQ3Ti1qMBAGCSsGAQQB\
gjcVAQQDAgEAMA0GCSqGSIb3DQEBDQUAA4ICAQAVpyJ1qLjqRLC34F1UXkC3vxpO\
nV6WgzpzA+DUNog4Y6RhTnh0Bsir+I+FTl0zFCm7JpT/3NP9VjfEitMkHehmHhQK\
c7cIBZSF62K477OTvLz+9ku2O/bGTtYv9fAvR4BmzFfyPDoAKOjJSghD1p/7El+1\
eSjvcUBzLnBUtxO/iYXRNo7B3+1qo4F5Hz7rPRLI0UWW/0UAfVCO2fFtyF6C1iEY\
/q0Ldbf3YIaMkf2WgGhnX9yH/8OiIij2r0LVNHS811apyycjep8y/NkG4q1Z9jEi\
VEX3P6NEL8dWtXQlvlNGMcfDT3lmB+tS32CPEUwce/Ble646rukbERRwFfxXojpf\
C6ium+LtJc7qnK6ygnYF4D6mz4H+3WaxJd1S1hGQxOb/3WVw63tZFnN62F6/nc5g\
6T44Yb7ND6y3nVcygLpbQsws6HsjX65CoSjrrPn0YhKxNBscF7M7tLTW/5LK9uhk\
yjRCkJ0YagpeLxfV1l1ZJZaTPZvY9+ylHnWHhzlq0FzcrooSSsp4i44DB2K7O2ID\
87leymZkKUY6PMDa4GkDJx0dG4UXDhRETMf+NkYgtLJ+UIzMNskwVDcxO4kVL+Hi\
Pj78bnC5yCw8P5YylR45LdxLzLO68unoXOyFz1etGXzszw8lJI9LNubYxk77mK8H\
LpuQKbSbIERsmR+QqQ==\
-----END CERTIFICATE-----\n";

WiFiClientSecure  net;
PubSubClient client(net);
BearSSL::X509List x509(test_root_ca);
PN532_SPI pn532spi(SPI, D2); // creation spi obj
NfcAdapter nfc = NfcAdapter(pn532spi); // creation nfc obj

#define DEBUG_SERIAL Serial
#define DEBUG_SERIAL_BAUDRATE 115200

void callback(char* topic, byte* payload, unsigned int length);

void connect() {
  delay(5000);
  DEBUG_SERIAL.print("Conecting to wifi ...");
  while (WiFi.status() != WL_CONNECTED) {
    DEBUG_SERIAL.print(".");
    delay(1000);
  }
  DEBUG_SERIAL.println(" Connected");
  net.setInsecure();
  DEBUG_SERIAL.print("Connecting to Yandex IoT Core as");
  DEBUG_SERIAL.print(yandexIoTCoredeviceId);
  DEBUG_SERIAL.print(" ...");
  while (!client.connect("Esp8266Client", yandexIoTCoredeviceId, mqttpassword)) {
    DEBUG_SERIAL.print(".");
    delay(1000);
  }
  DEBUG_SERIAL.println(" Connected");
  ///

  DEBUG_SERIAL.println("Subscribe to: ");
  DEBUG_SERIAL.println(topicCommands.c_str());
  if(client.subscribe(topicCommands.c_str())) {
      DEBUG_SERIAL.println("subscribed");
  }
  DEBUG_SERIAL.print("State ");
  DEBUG_SERIAL.println(client.state());
  }


void callback(char* topic, byte* payload, unsigned int length) {
  DEBUG_SERIAL.print("debug in callback block"); //deb
  String topicString = String(topic);
  DEBUG_SERIAL.print("Message received. Topic: ");  // deb
  DEBUG_SERIAL.println(topicString.c_str()); //deb
  String payloadStr = "";
  for (int i=0;i<length;i++) {
    payloadStr += (char)payload[i];
  }
  DEBUG_SERIAL.println(payloadStr);
  if(payloadStr == "Run"){
    delay(5000); // wait 5 sec
    const char* data_from_tag = "";
    if(nfc.tagPresent()){
      DEBUG_SERIAL.println("debug1");
      NfcTag tag = nfc.read();
      if (tag.hasNdefMessage()) // every tag won't have a message
      {
        DEBUG_SERIAL.println("debug2");
        NdefMessage message = tag.getNdefMessage();
        NdefRecord record = message.getRecord(0);
        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);
        for (int c = 0; c < payloadLength; c++) {data_from_tag += (char)payload[c];}
        
        if(client.publish(warehouse_topic_test.c_str(), payload,payloadLength)){
          DEBUG_SERIAL.println("pub success");
        }

      }
    }
   
  }
}

void setup() {
  DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUDRATE);
  delay(10);
  DEBUG_SERIAL.println("Device started");
  WiFi.begin(ssid, password);
  client.setServer(mqttserver, mqttport);
  DEBUG_SERIAL.print("debug in setup block");
  client.setCallback(callback);
  client.setBufferSize(1024); // было 1024
  client.setKeepAlive(15); // было 15
  connect();
  nfc.begin();
}


void loop() {

  client.loop();
  if (!client.connected()) {
    connect();
  }
}
