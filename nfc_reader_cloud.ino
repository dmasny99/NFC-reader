// библиотеки wifi и MQTT
#include <string.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h> 
// библиотеки протокола передачи данных и библиотеки модуля
#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>
#include <NfcAdapter.h> 
//параметры подключения wifi
const char* ssid = "Masny";
const char* password = "5sJn8eXw";
// параметры подключения yandex cloud
const char* mqttserver = "130.193.44.244"; //mqtt.cloud.yandex.net
const char* yandexIoTCoredeviceId = "areslq41mged71mcd3be";
const char* mqttpassword = "Passwordpassword123";
const int mqttport=8883;
const char* not_found = "not found";
const char* incorrect = "incorrect";
const char* ok ="ok";


// определение глобальных констант (топики)
String warehouse_topic_test = "$devices/areslq41mged71mcd3be/events/test";
String test_topic = "$devices/areslq41mged71mcd3be/events/test";
String topicCommands = String("$devices/")+String(yandexIoTCoredeviceId)+String("/commands");
String topicEvents = String("$devices/")+String(yandexIoTCoredeviceId)+String("/events");
// корневой сертификат брокера яндексовского облака
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
//создание объектов класса
WiFiClientSecure  net;
PubSubClient client(net);
BearSSL::X509List x509(test_root_ca);
PN532_SPI pn532spi(SPI, D2); // creation spi obj
NfcAdapter nfc = NfcAdapter(pn532spi); // creation nfc obj

#define DEBUG_SERIAL Serial
#define DEBUG_SERIAL_BAUDRATE 115200
// объявление функции 
void callback(char* topic, byte* payload, unsigned int length);

void connect() {
  delay(5000);
  DEBUG_SERIAL.print("Conecting to wifi ..."); // deb inf
  while (WiFi.status() != WL_CONNECTED) {
    DEBUG_SERIAL.print(".");
    delay(1000);
  }
  DEBUG_SERIAL.println(" Connected"); // deb inf
  net.setInsecure(); // отключаю проверку SSL тк авторизуюсь по логиину и паролю
  DEBUG_SERIAL.print("Connecting to Yandex IoT Core as"); // deb inf
  DEBUG_SERIAL.print(yandexIoTCoredeviceId); // deb inf
  DEBUG_SERIAL.print(" ..."); // deb inf
  while (!client.connect("Esp8266Client", yandexIoTCoredeviceId, mqttpassword)) {
    DEBUG_SERIAL.print(".");
    delay(1000);
  }
  DEBUG_SERIAL.println(" Connected"); // deb inf

  DEBUG_SERIAL.println("Subscribe to: "); // deb inf
  DEBUG_SERIAL.println(topicCommands.c_str());
  if(client.subscribe(topicCommands.c_str())) {DEBUG_SERIAL.println("subscribed");}
  }

void callback(char* topic, byte* payload, unsigned int length) {
  String topicString = String(topic); // дебаг
  DEBUG_SERIAL.print("Message received. Topic: ");  // deb inf
  DEBUG_SERIAL.println(topicString.c_str()); //deb inf
  String payloadStr = "";
  for (int i=0;i<length;i++) {
    payloadStr += (char)payload[i];
  }
  String command = payloadStr.substring(0,3); // строка вида run id N
  DEBUG_SERIAL.println(payloadStr);
  String id = payloadStr.substring(7);
  DEBUG_SERIAL.println(command);// дебаг
  DEBUG_SERIAL.println(id);// дебаг
  if(command = "run"){
    delay(5000); // жду 5 сек
    const char* data_from_tag = "";
    if(nfc.tagPresent()){ // если метка рядом то выполняю блок кода
      NfcTag tag = nfc.read(); // считываю метку
      if (tag.hasNdefMessage()) // если метка не пустая то выполняю блок кода
      {
        NdefMessage message = tag.getNdefMessage(); //считываю все записи данных метки
        NdefRecord record = message.getRecord(0); // беру только первую запись с метки
        int payloadLength = record.getPayloadLength(); // нахожу длину сообщения
        byte payload[payloadLength]; // создаю массив байтов длины полученного сообщения
        record.getPayload(payload); // копирую полученное сообщение в этот массив
        String tag_id = "";
        for (int i=3;i<payloadLength;i++) {tag_id += (char)payload[i];} // метки записаны так ID N
        if(id == tag_id.substring(3)){
          client.publish(warehouse_topic_test.c_str(), ok);
          DEBUG_SERIAL.println("pub success");}
        else {client.publish(warehouse_topic_test.c_str(), incorrect);}
      }
      else {client.publish(warehouse_topic_test.c_str(), incorrect);}
    }
    else{client.publish(warehouse_topic_test.c_str(), not_found);}
  }
  else {client.publish(warehouse_topic_test.c_str(), incorrect);}
}

void setup() {
  DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUDRATE);
  delay(10);
  DEBUG_SERIAL.println("Device started"); //deb inf
  WiFi.begin(ssid, password); // инициализирую настройки подключения к WIFI
  client.setServer(mqttserver, mqttport); // инициализирую настройки подключения к облаку
  client.setCallback(callback); // объявляю колбэк функцию (в ней описана реакция на получение сообщения в топик)
  client.setBufferSize(1024); // размер буфера по дефолту
  client.setKeepAlive(15); // время ожидания по дефолту (чем больше время, тем позже обнаружится разрыв соедиенения)
  connect(); // подключаюсь к wifi, yandex cloud
  nfc.begin(); // включаю pn532
}


void loop() {

  client.loop();
  if (!client.connected()) {
    connect();
  }
}
