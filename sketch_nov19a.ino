 // chay arduino go lenh sudo chmod a+rw /dev/ttyUSB0
// De vao giao dien node-red them / ui -- can ghi ten button de tao dasboard
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// bạn thay đổi thông tin bên dưới cho phù hợp
const char* ssid =         "lib";
const char* password =    "";
const char* mqtt_server = "192.168.12.96";   /// lấy ip bằng lệnh ifconfig, ví dụ 192.168.1.105
const uint16_t mqtt_port = 1883;// Ket noi khong ma hoa
const byte ledPin = D0; // led tren esp
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
 /*
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("event", msg);
  }
  */
}

/*
 * Sau khi nắm được nguyên lý của giao thức MQTT, chúng ta sẽ giới thiệu các khái niêm cơ bản của MQTT như sau:

    Client: là thiết bị IoT muốn gửi/nhận dữ liệu trong network
    Broker: là thiết bị trung gian nhận dữ liệu từ các client muốn gửi và gửi dữ liệu đó tới các client muốn nhận
    Topic: tượng trưng cho loại dữ liệu mà các thiết bị Client gửi/nhận thông qua MQTT. Topic là 1 chuỗi UTF-8 text tượng trưng cho tên loại dữ liệu.
    Ví dụ của topic như: “example/topic1”,  “demo_esp/sensor/accel”, “demo_esp/sensor/gyro”, v.v….
    Trong topic, chúng ta có thể phân chia cấp dùng ký từ ‘/’  và từ đó có thể đăng ký nhận nhiều loại dữ liệu dùng các ký tự đặc biệt như ‘#’ hoặc ‘+’ như sau:
    ‘+’  Đăng ký nhiều topic chỉ khác nhau 1 vị trí phân cấp trong text của topic. Ví dụ như “demo/sensor/+” sẽ đăng ký nhận dữ liệu từ các topic như “demo/sensor/accel”, “demo/sensor/gyro”, v.v…
    ‘#’  Đăng ký nhiều topic có khác nhau nhiều vị trí phân cấp trong text của topic. Ví dụ như “demo/*” sẽ đăng ký nhận dữ liệu từ tất cả topic bắt đầu bằng “demo/”
    Publish: là bước gửi dữ liệu từ 1 thiết bị client đến Broker. Trong bước này, thiết bị IoT sẽ xác định topic (là loại dữ liệu muốn gửi) và giá trị của topic đó
    Subscribe: là bước đăng ký nhận dữ liệu từ Broker của 1 thiết bị Client. Trong bước này, thiết bị IoT sẽ xác định loại dữ liệu mà nó muốn nhận. Khi Broker nhận được loại dữ liệu này từ 1 thiết bị client khác, nó sẽ gửi dữ liệu này tới thiết bị client đã đăng ký nhận
    Unsubscribe: là bước thông báo với Broker là thiết bị Client không muốn tiếp tục nhận dữ liệu nữa

Sau khi hiểu được các khái niệm cơ bản nhưng quan trọng này, các bạn có thể sử dụng các thư viện open-source cho MQTT client và MQTT Broker vì flow chương trình sử dụng thư viện MQTT client khá đơn giản chỉ sử dụng vài hàm API như sau:

    Tạo kết nối TCP đến server chạy code MQTT Broker
    Gọi hàm publish() để gửi dữ liệu lên Broker
    Gọi hàm subscribe() để đăng ký nhận dữ liệu từ Broker (hoặc unsubscribe() để ngừng nhận dữ liệu từ Broker)
    Đăng ký hàm callback của ứng dụng để được gọi khi thư viện MQTT client nhận được dữ liệu từ Broker

Với phạm vi và thời gian hạn chế,  bài viết muốn giới thiệu cách thức hoạt động của giao thức MQTT và các kiến thức tổng quát đủ để các bạn hiểu được bản chất của MQTT và sử dụng các thư viện open-source cho MQTT trong các ứng dụng của mình. Nếu các bạn muốn tìm hiều sâu hơn và đầy đủ hơn cách thức hoạt động của giao thức MQTT để hiểu source code của các bộ thư viện hoặc tự viết 1 bộ thư viện cho riêng mình, các bạn có thể tham khảo các trang web giải thích chi tiết hoặc tài liệu specification của MQTT  ở đường link này http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.pdf
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * Mỗi thư viện đều có tài liệu và sample code hướng dẫn cách sử dụng các hàm API cho hành động kết nối server, publish(), subscribe(), unsubscribe() và đăng ký callback. Do đó  chúng ta dễ dàng tích hợp khả năng truyền dữ liệu qua cloud server giữa các thiết bị ESP8266 1 cách nhanh chóng và dễ dàng với MQTT nhờ sự giúp đỡ của cộng đồng phát triển các bộ thư viện open source này. Trong bài viết này, bài viết sẽ giới thiệu cách sử dụng thư viện PubSubClient vì nó khá đơn giản và dễ hiểu với nhiều người.

Sử dụng các hàm API của thư viện PubSubClient hỗ trợ chức năng MQTT Client

Các hàm API cơ bản và thông dụng nhất của thư viện PubSubClient có thể được liệt kê như sau:

     Hàm PubSubClient(Client &client) để khởi động bộ thư viện. Chúng ta sẽ tạo 1 biến kiểu Client và pass địa chỉ biến đó vào hàm này để khởi động bộ thư viện.
    Hàm setServer(<domain name của Broker>, <TCP port>) để thiết lập địa chỉ của Broker server và port TCP sẽ kết nối đến
    Hàm  setCallback(<callback>) để đăng ký hàm callback sẽ được gọi khi thư viện nhận được giá trị mới từ Broker cho dữ liệu đã được subscribe
    Hàm connect(<client_name>) để bắt đầu quá trình kết nối đến Broker server. Hàm này sẽ đợi đến khi kết nối thành công hoặc timeout; giá trị trả về của hàm là kiểu boolean với True là kết nối thành công, False là kết nối không thành công
    Hàm disconnect() để chủ động ngắt kết nối đến Broker server
    Hàm publish(<topic>, <value>) để gửi giá trị <value> cho dữ liệu <topic> lên Broker server
    Hàm subscribe(<topic>) để đăng ký nhận giá trị mới của dữ liệu <topic> từ Broker server
    Hàm connnected() để kiểm tra trạng thái kết nối đến Broker server (True = kết nối OK)
    Hàm loop() là hàm xử lý các tác vụ theo giao thức MQTT. Chúng ta sẽ gọi hàm này liên tục trong hàm loop() của Arduino

Để cài đặt thư viện PubSubClient trên Arduino, chúng ta vào Sketch -> Include Library -> Manage Libraries….  rồi chọn cài đặt PubSubClient của Nick O’Leary. Các file thư viện sau khi cài đặt có thể tìm ở C:\Users\<name>\Arduino\libraries\PubSubClient\ . Các bạn có thể tìm hiểu cách thức hiện thực MQTT client bằng cách tìm hiểu source code trong thư mục PubSubClient này.
 * 
 */
