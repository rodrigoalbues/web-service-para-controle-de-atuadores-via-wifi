#include <ESP8266WiFi.h>

const char* ssid     = "REDE"; // Nome da rede WIFI
const char* password = "SENHA"; // Password da Rede WIFI

//int ledPin  = 13; // GPIO13 do ESP8266 e D7 do NodeMCU
//int ledPin1 = 14; // GPIO14 do ESP8266 e D5 do NodeMCU

#define ledPinVerde D5 // Nesse caso vamos usar a constante D5 que já está definida
#define ledPinAmarelo D6 // Nesse caso vamos usar a constante D7 que já está definida
#define ledPinVermelho D7 // Nesse caso vamos usar a constante D7 que já está definida

WiFiServer server(80); // Usamos a porta 80

void setup() {

  Serial.begin(115200); // Configuramos a velocidade como 115200

  delay(10); // Ativamos um delay

  pinMode(ledPinVerde, OUTPUT); // Definimos o D5 como saída
  digitalWrite(ledPinVerde, HIGH); // O LED começará ligado

  pinMode(ledPinAmarelo, OUTPUT); // Definimos o D6 como saída
  digitalWrite(ledPinAmarelo, HIGH); // O LED começará ligado

  pinMode(ledPinVermelho, OUTPUT); // Definimos o D6 como saída
  digitalWrite(ledPinVermelho, HIGH); // O LED começará ligado

  // Comunicação com a Rede WIFI

  Serial.println();
  Serial.println();

  Serial.print("Conectado a "); // Mensagem apresentada no monitor Serial
  Serial.println(ssid); // Exibe o nome da Rede WIFI no Monitor Serial
  WiFi.begin(ssid, password); // Inicia a Conexão com a Rede WiFi

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("*"); // Enquanto a conexão não é efetuada com sucesso
  }                   // será apresentado no Monitor Serial uma sucessão de "*"

  Serial.println("");
  Serial.println("Conectado a Rede WiFi "); // Se a conexão é efetuada com sucesso
                                           // apresenta a Mensagem no Monitor Serial
 // Servidor

 server.begin(); // Comunicação com o Servidor

 Serial.println("Servidor Iniciado"); // Será mostrado no Monitor Serial que o 
                                     // servidor foi iniciado com sucesso
 // Impressão do endereço IP a ser utilizado

 Serial.print("Use a seguinte URL para acessar o Web Service: ");

 Serial.print("http://");

 Serial.print(WiFi.localIP()); // Abrindo o Navegador com este IP acessamos a página HTML que
                              // contém o Web Service de controle dos LED's. É importante ressaltar
 Serial.println("/");        // que este IP só estará disponível na rede á qual o ESP8266 está conectado!
}
 
void loop() {

  //int contator = 1; // Cria uma variável para exibir quantas interações foram feitas com o Web Service

  //Verificação se o cliente está conectado e disponível
  WiFiClient client = server.available();

  if (!client) { // Verifica se o cliente está conectado ao servidor, executa este ciclo
    return;     // até o cliente estar conectado
  }

  // Espera até o cliente enviar dados para os Led's
  Serial.println("Interação com o Web Service: "); // Apresenta esta mensagem quando o cliente
                                                  // se conecta ao servidor
  while (!client.available()){
    delay(1);
  }

  // Ler a primeira linha do pedido
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Operação do pedido
  int valueVerde    = LOW;
  int valueAmarelo  = LOW;
  int valueVermelho = LOW;
  int alternados      = 0;
  if (request.indexOf("/LEDVerde=ON") != -1) {
    digitalWrite(ledPinVerde, HIGH); // Se o pedido no ledPinVerde for LED Verde=on, acende o LED verde
    valueVerde = HIGH;
  }

  if (request.indexOf("/LEDVerde=OFF") != -1) {
    digitalWrite(ledPinVerde, LOW); // Se o pedido no LedPinVerde for LED Verde=OFF, apaga o LED Verde
    valueVerde = LOW;
  }

  if (request.indexOf("/LEDAmarelo=ON") != -1) {
    digitalWrite(ledPinAmarelo, HIGH); // Se o pedido no LedPinAmarelo for LED Amarelo=ON, acende o LED amarelo
    valueAmarelo = HIGH;
  }

  if (request.indexOf("/LEDAmarelo=OFF") != -1) {
   digitalWrite(ledPinAmarelo, LOW); // Se o pedido no LedPinAmarelo for LED Amarelo=OFF, apaga o LED amarelo
    valueAmarelo = LOW;
  }

  if (request.indexOf("/LEDVermelho=ON") != -1) {
    digitalWrite(ledPinVermelho, HIGH); // Se o pedido no LedPinVermelho for LED Vermelho=ON, acende o LED vermelho
    valueVermelho = HIGH;
  }

  if (request.indexOf("/LEDVermelho=OFF") != -1) {
   digitalWrite(ledPinVermelho, LOW); // Se o pedido no LedPinVermelho for LED Vermelho=OFF, apaga o LED vermelho
    valueVermelho = LOW;
  }

  if (request.indexOf("/alternados=ON") != -1) {
   
    alternados = 2; 
  }

  if (request.indexOf("/alternados=OFF") != -1) {
   
    alternados = 3;
  }

  if (alternados == 2){
    digitalWrite(ledPinVermelho, HIGH);
    digitalWrite(ledPinAmarelo, LOW);
    digitalWrite(ledPinVerde, LOW);
    delay(1000);
    digitalWrite(ledPinVermelho, LOW);
    digitalWrite(ledPinAmarelo, LOW);
    digitalWrite(ledPinVerde, HIGH);
    delay(1000);
    digitalWrite(ledPinVermelho, LOW);
    digitalWrite(ledPinAmarelo, HIGH);
    digitalWrite(ledPinVerde, LOW);
    delay(500);
    
  }  
  
  if (alternados == 3){
      digitalWrite(ledPinVermelho, LOW);
      digitalWrite(ledPinAmarelo, LOW);
      digitalWrite(ledPinVerde, LOW);
  }  

  // Inicialização da página HTML

  // Retorno da resposta
  client.println("HTTP/1.1 200 ok");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  

  client.println("<br><br>");

  client.print("Estado do LED: ");
  client.println(valueVerde);
  client.println("<br />");
  client.println("<label>LED Verde :</label>");
  client.println("<a href=\"/LEDVerde=ON\"\"><button>Ligar </button></a>");// Ligar o LED
  client.println("<a href=\"/LEDVerde=OFF\"\"><button>Desligar </button></a><br />"); // Desligar o LED
  client.println("<br />");
  
  client.print("Estado do LED: ");
  client.println(valueAmarelo);
  client.println("<br />");
  client.println("<label>LED Amarelo:</label>");
  client.println("<a href=\"/LEDAmarelo=ON\"\"><button>Ligar</button></a>");
  client.println("<a href=\"/LEDAmarelo=OFF\"\"><button>Desligar</button></a><br />");
  client.println("<br />");
  
  client.print("Estado do LED: ");
  client.println(valueVermelho);
  client.println("<br />");
  client.println("<label>LED Vermelho:</label>");
  client.println("<a href=\"/LEDVermelho=ON\"\"><button>Ligar</button></a>");
  client.println("<a href=\"/LEDVermelho=OFF\"\"><button>Desligar</button></a><br />");

  client.println("<br />");
  client.print("Estado dos Led's Alternados: ");
  if(alternados == 2) { 
    client.print("Ligado");
  } else{
    client.print("Desligado"); // Se está desligado apresenta "Desligado"
  }
  client.println("<br />");
  client.println("<label>Led's Alternados:</label>");
  client.println("<a href=\"/alternados=ON\"\"><button>Ligar</button></a>");
  client.println("<a href=\"/alternados=OFF\"\"><button>Desligar</button></a><br />");
  client.println(alternados);
  client.println("</html");
  
  delay(10);
  Serial.println("Cliente desconectado"); // Depois do cliente efetuar o pedido apresenta esta mensagem no Monitor Serial
  Serial.println("");
}
