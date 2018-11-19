//#include <ESP8266WiFi.h>
//#include <PubSubClient.h>
//
//// Chuong trinh con nhan data
//void CallBack(char * tp, byte * nd, unsigned int length)
//{
//  // Nhan data qua ham nay
//  String topic(tp);// nhan topic
//  String noidung = String((char*)nd); // nhan noi dung tra ve kieu char
//  noidung.remove(length);// kich thuoc chon length theo ben tren
//
//  // Hien thi ra man hinh
//  Serial.print("co du lieu gui ve Van Viet \n");
//  Serial.print(topic);                                       // hien thi topic 
//  Serial.print(":\t");
//  Serial.print(noidung);                                     // hien thi noi dung
//  Serial.print("\n\n");
//
//  //dieu khien led
//  if(topic == "broker"){
//    if(noidung == "led_sang"){
//      digitalWrite(16,LOW);
////      Serial.print("Led sang");
//    }
//    if(noidung == "led_tat"){
//      digitalWrite(16,HIGH);
////      Serial.print("Led tat");
//
//    }
//  }
//  
//}
//
//
//// khai bao 1 WiFi Client
//WiFiClient nhien;
//
//// khai bao MQTT   m15.cloudmqtt.com  16615
//PubSubClient MQTT("m15.cloudmqtt.com",16615, CallBack, nhien); // server - port - ham goi ve - 1 client
//
//void setup() {
//  Serial.begin(9600);
//
//  //ket noi wifi cho esp
//  WiFi.begin("DucNam","39551278");
//
//  //kem tra da ket noi chua
//  while(1)
//  {
//    delay(100);
//    if( WiFi.status() == WL_CONNECTED )
//      break;
//  }
//
//  //xuat thong bao qua monitor
//  Serial.print("ESP da ket noi voi internet \n");
//
//  //ket noi ESP vao MQTT
//  while(1)
//  {
//    delay(500);
//    if(MQTT.connect ("VietID","upfnwvhc","2wJsJVh6ZPMW")) // 1 id client - 1 user - 1 password
//      break;
//  }
//  Serial.print("ESP da ket noi voi MQTT \n");
//
//  //Gui du lieu len Broker
//  MQTT.publish("broker","dang gui data len ");
//
//  //Nhan data tu Web ve
//  MQTT.subscribe("broker",1);                            // 1 topic - 1 Quality of Service (2 3 4..)
//
//  // thiet lap GPIO led 16
//  pinMode(16,OUTPUT);
//  digitalWrite(16,HIGH);             // Chan 16 tich cuc muc Low  
//  }
//
//void loop() {
//                                                      // Duy tri su song cho MQTT chay lien tuc
//  MQTT.loop();
//
////  MQTT.publish("nhandulieu",String(!digitalRead(16)).c_str());
////  delay(500);
//}

























#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// bạn thay đổi thông tin bên dưới cho phù hợp
const char* ssid =         "DucNam";
const char* password =    "39551278";
const char* mqtt_server = "192.168.1.107";   /// lấy ip bằng lệnh ifconfig, ví dụ 192.168.1.105
const uint16_t mqtt_port = 16615;
const byte ledPin = D0;
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(ledPin, OUTPUT);     // Khởi tạo LED
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  // Kết nối wifi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Bật LED nếu nhận được lệnh là 1
  if ((char)payload[0] == '1') {
     Serial.println("ON");
    digitalWrite(ledPin, LOW);   // LED có sẵn trên NodeMCU bật ở mức 0(LOW)
  } 
  // Tắt LED nếu nhận được lệnh là 0
 if ((char)payload[0] == '0') {
    Serial.println("OFF");
    digitalWrite(ledPin, HIGH);  // LED tắt ở mức 1(HIGH)
  }

}

void reconnect() {
  // Đợi tới khi kết nối
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Khi kết nối thành công sẽ gửi chuỗi helloworld lên topic event
      client.publish("event", "hello world");
      // ... sau đó sub lại thông tin
      client.subscribe("event");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
 // Thực hiện 2s gửi dữ liệu helloworld lên broker 1 lần
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("event", msg);
  }
}
