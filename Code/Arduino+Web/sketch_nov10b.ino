#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Stepper.h>

//ESP I/O Pin Set
#define IN4 D8
#define IN3 D7
#define IN2 D6
#define IN1 D5

//Stepper Setup

#define RPM 10
const int STEPS = 2048;



//Wifi Credentials
/*
const char* ssid = "AndroidAPfec6";
const char* password = "tls@ehddnjs";
*/

const char* ssid = "2261030";
const char* password = "luinesuki";


int breaker;
Stepper myStepper(STEPS, IN4, IN2, IN3, IN1);



ESP8266WebServer server(80);





void handleRoot() {                                       //200(정상)때 출력할 함수,추 후 코드작성시 여기가 메인
  server.send(200, "text/plain", "hello from esp8266!");  //OK 요청,hello from esp8266 브라우저에 출력
}


void handleNotFound() {  //404(못 찾을시)에러때 출력할 함수
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {  //Arduino 셋업
  Serial.begin(115200);
  WiFi.begin(ssid, password);  //미리 받은 ssid와 password를 이용하여 와이파이에 연결
  Serial.println("");          //한줄 띄우기
  // 연결 기다릴때 출력할 코드(와이파이 연결할때까지 0.5초 기다리고 . 찍기)
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");  //한줄 띄우기
                       //Connected To '미리 입력받은 ssid' 출력
  Serial.print("연결됨 : ");
  Serial.println(ssid);
  //아이피 주소 출력
  Serial.print("아이피 주소: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS 수신기 시작");
  }
  //서버 설정
  server.on("/", handleRoot);  // 루트로 접근 요청 받을 시 handleRoot 함수 내용 실행
  server.on("/openC/1", rotateLeft);
  server.on("/stopC/1", rotateStop);
  server.on("/closeC/1", rotateRight);
  server.onNotFound(handleNotFound);  //404(못찾으면)handleNotFound 실행
  server.begin();                     //서버 실행
  Serial.println("HTTP 서버 시작됨. 아래의 링크로 접속해 주세요.");
  Serial.print(WiFi.localIP());
  Serial.println(":80");
  myStepper.setSpeed(RPM);
}

void loop(void) {
  server.handleClient();  //서버 요청 계속 받기
}



void rotateLeft() {
  server.send(200, "text/plain", "Open-ing");
  myStepper.step(STEPS);
  Serial.println("Curtain Opening...");
}

void rotateRight() {
  server.send(200, "text/plain", "Close-ing");
  myStepper.step(-STEPS);
  ESP.wdtDisable();
  Serial.println("Curtain Closing...");
  delay(5000);
}

void rotateStop() {
  server.send(200, "text/plain", "Stop-ing");
  //breaker=1;
  Serial.println("Curtain Stoping...");
}

void setCRoutine() {
  server.send(200, "text/plain", "hello from esp8266!");
}