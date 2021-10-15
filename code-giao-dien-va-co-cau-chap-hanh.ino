#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

int in1=D1;
int in2=D2;
int in3=D3;
int in4=D4;
int speedXe=255;
ESP8266WebServer server;//khởi tạo web server
WebSocketsServer webSocket = WebSocketsServer(81); //khởi tạo websocketserver(giao tiếp đt vs esp)

const char* password = "";
const char* ssid = "Xe Wifi";
IPAddress local_IP(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);
                                            // Thiết kế giao diện
                                            char html[] PROGMEM = R"=====(
                                            <!DOCTYPE html>
                                            <html>
                                            <head>
                                                <meta charset="utf-8" />
                                                <link rel="stylesheet" href="StyleSheet1.css" />
                                                <script type="text/javascript" src="JavaScript.js"></script>
                                                <title>Xe Wifi</title>
                                            </head>
                                            <body onload=" socketInit()" > 
                                                <p id="tieuDe">Xe Wifi</p>
                                                <button id="tienLen" ontouchstart="tienLen()" ontouchend="dungLai()">Tiến lên</button>
                                                <button id="quayTrai" ontouchstart="quayTrai()" ontouchend="dungLai()">Quay trái</button>
                                                <button id="quayPhai" ontouchstart="quayPhai()" ontouchend="dungLai()">Quay phai</button>
                                                <button id="diLui" ontouchstart="diLui()" ontouchend="dungLai()">Đi lùi</button>
                                                <button id="dungLai" ontouchstart="dungLai()" ontouchend="dungLai()">Stop</button>
                                                <div id="thayDoiTocDo">
                                                    <input type="range" id="tocDo" value="255" min="150" max="255" step="1" class="customRange" oninput="thayDoiTocDo()"/>
                                                    <p>Tốc Độ: <span  id="giaTriTocDo">255</span></p>
                                                </div>
                                                <body onmousedown= 'return false' onselectstart= 'return false '>
                                            </body>
                                            </html>
                                            )=====";
                                            char styleCss[] PROGMEM = R"=====(
                                            body {
                                                background-color:#92f35f;
                                                
                                            }
                                            #tieuDe{
                                                font-size:50px;
                                                top:20px;
                                                left:100px;
                                                color:#ff0000;
                                                position:absolute;
                                            }
                                            #tienLen{
                                                width:100px;
                                                height:50px;
                                                background-color:#ffd800;
                                                position:absolute;
                                                top:150px;
                                                left:100px;
                                            }
                                            #quayTrai {
                                                width: 100px;
                                                height: 50px;
                                                background-color: #ffd800;
                                                position: absolute;
                                                top: 200px;
                                                left: 0px;
                                            }
                                            #quayPhai {
                                                width: 100px;
                                                height: 50px;
                                                background-color: #ffd800;
                                                position: absolute;
                                                top: 200px;
                                                left: 200px;
                                            }
                                            #diLui {
                                                width: 100px;
                                                height: 50px;
                                                background-color: #ffd800;
                                                position: absolute;
                                                top: 250px;
                                                left: 100px;
                                            }
                                            #dungLai {
                                                width: 100px;
                                                height: 50px;
                                                background-color: #ffd800;
                                                position: absolute;
                                                top: 200px;
                                                left: 100px;
                                            }
                                            #thayDoiTocDo{
                                                position:absolute;
                                                top:350px;
                                                left:50px;
                                            }
                                            #tocDo{
                                                width:200px;
                                            }
                                            .customRange {
                                                -webkit-appearance: none;
                                                height: 10px;
                                                outline: none;
                                                opacity: 0.8;
                                                -webkit-transition: 0.2s;
                                                border-radius: 3px;
                                              
                                            }
                                                .customRange:hover {
                                                    opacity: 1;
                                                }
                                                .customRange::-webkit-slider-thumb {
                                                    -webkit-appearance: none;
                                                    background: rgb(255,0,0);
                                                    width: 15px;
                                                    height: 15px;
                                                    border-radius: 50%;
                                                }
                                            )=====";
                                            char javaScript[] PROGMEM = R"=====(
                                            function socketInit() {
                                                socket = new WebSocket('ws://' + window.location.hostname + ':81/');//khởi tạo webserver
                                               
                                            }
                                            function quayPhai() {
                                                //var quayPhai = document.getElementById("quayPhai");
                                                socket.send("1");
                                            }
                                            function quayTrai() {
                                                //var quayPhai = document.getElementById("quayPhai");
                                                 socket.send("2");
                                            }
                                            function tienLen() {
                                                //var quayPhai = document.getElementById("quayPhai");
                                                 socket.send("3");
                                                
                                            }
                                            function diLui() {
                                                //var quayPhai = document.getElementById("quayPhai");
                                                socket.send("4");
                                            }
                                            function dungLai() {
                                                //var quayPhai = document.getElementById("quayPhai");
                                                socket.send("5");
                                            }
                                            function thayDoiTocDo() {
                                                var rangeTocDo = document.getElementById("tocDo");//khai báo biến 
                                                var valueTocDo = document.getElementById("giaTriTocDo");
                                                valueTocDo.innerHTML = rangeTocDo.value;
                                                
                                                socket.send("tocdo=" + rangeTocDo.value);
                                            }
                                            )=====";
void setup()
{
 
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
  Serial.begin(115200);

  delay(10);
//  pinMode(pin_led, OUTPUT);
  
  
  WiFi.mode(WIFI_AP); // thiết lập chế độ điểm truy cập wifi
  WiFi.softAP(ssid,password);// thiết lập SSID(tên wifi) và mật khẩu
   
  Serial.print("Setting soft-AP configuration ... ");// in ra màn hình: Setting soft-AP configuration ... 
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");//kiểm tra cấu hình wifi, đúng hay sai

  server.on("/",[](){server.send_P(200, "text/html", html);});//gửi giao diện html lên địa chỉ ip mặc định 192.168.4.1
  server.on("/StyleSheet1.css",[](){server.send_P(200,"text/css",styleCss);});//gửi giao diện css lên địa chỉ ip mặc định 192.168.4.1
  server.on("/JavaScript.js",[](){server.send_P(200,"text/js",javaScript);});//gửi giao diện javascript lên địa chỉ ip mặc định 192.168.4.1
  

  Serial.print("Soft-AP IP address = ");//in ra màn hình: Soft-AP IP address =
  Serial.println(WiFi.softAPIP());//hiển thi địa chỉ ip

 
  server.begin();//khởi tạp server
  webSocket.begin();// khởi tạo webSocket
  webSocket.onEvent(webSocketEvent);//khởi tạo hàm kiểm tra sự kiện trên webSocket
}

void loop()
{
  webSocket.loop();//duy trì webSocket
  server.handleClient();//duy trì wifi

}
                  void tienLen(int tocDo){
                    analogWrite(in1,tocDo);
                    digitalWrite(in2,0);
                    digitalWrite(in3,0);
                    analogWrite(in4,tocDo);
                    Serial.print("tien Len");
                    }
                   void quayPhai(int tocDo){
                    analogWrite(in1,tocDo);
                    digitalWrite(in2,0);
                    digitalWrite(in3,0);
                    digitalWrite(in4,0);
                    Serial.print("quay Phai");
                    }
                   void quayTrai(int tocDo){
                    digitalWrite(in1,0);
                    digitalWrite(in2,0);
                    digitalWrite(in3,0);
                    analogWrite(in4,tocDo);
                    Serial.print("quay Trai");
                    }
                  void diLui(int tocDo){
                    digitalWrite(in1,0);
                    analogWrite(in2,tocDo);
                    analogWrite(in3,tocDo);
                    digitalWrite(in4,0);
                    Serial.print("di Lui");
                    } 
                  void dungLai(){
                    digitalWrite(in1,0);
                    digitalWrite(in2,0);
                    digitalWrite(in3,0);
                    digitalWrite(in4,0);
                    Serial.print("Dung Lai");
                    }    
                  // được gọi đến khi có dữ liệu từ server đến arduino
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  if(type == WStype_TEXT){
   
    String payloadString = (const char *)payload;
   Serial.println(payloadString);
   if (payloadString.equals("1")) quayPhai(speedXe);
   if (payloadString.equals("2")) quayTrai(speedXe);
   if (payloadString.equals("3")) tienLen(speedXe);
   if (payloadString.equals("5")) dungLai();
   if (payloadString.equals("4")) diLui(speedXe);
   if(payloadString.substring(0,6).equals("tocdo=")){
      speedXe=payloadString.substring(6,payloadString.length()).toInt();
   }
   }
   if(type==WStype_CONNECTED){
    IPAddress ip = webSocket.remoteIP(num);
    Serial.println("Websocket client connnected !" + ip);   
  }
  if(type==WStype_DISCONNECTED){
    Serial.println("Client disconnnected !"+ num);
  }

}
