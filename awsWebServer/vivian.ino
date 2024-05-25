
#include <SimpleModbusSlave.h>
#include <Servo.h>

#define CHUVA_VALUE 0
#define UMIDADE_VALUE 1
#define SERVO_VALUE 2
#define HOLDING_REGS_SIZE 3

unsigned int holdingRegs[HOLDING_REGS_SIZE];
int chuvaValue = 0;
int umidadeValue = 0;

const int umidade_solo_pin = A0;
const int sensor_chuva_pin = 2;
const int servo_motor_pin = 9;

Servo servoMotor;

void girarServo(int angulo);

void setup() {
  Serial.begin(9600);
  modbus_configure(&Serial, 9600, SERIAL_8N1, 1, 2, HOLDING_REGS_SIZE, holdingRegs);
  pinMode(sensor_chuva_pin, INPUT);
  servoMotor.attach(servo_motor_pin);
  servoMotor.write(0);
  Serial.println("Setup completo");
}

void loop() {
  modbus_update();
  
  /*
  if (result != 0) {
    Serial.print("Erro Modbus: ");
    Serial.println(result);
  }
  */
  
  int umidade_solo = analogRead(umidade_solo_pin);
  int chuva_detectada = digitalRead(sensor_chuva_pin);

  holdingRegs[CHUVA_VALUE] = chuva_detectada;
  holdingRegs[UMIDADE_VALUE] = umidade_solo;

  Serial.print("Umidade do solo: ");
  Serial.println(umidade_solo);
  Serial.print("Chuva detectada: ");
  Serial.println(chuva_detectada);
  Serial.print("Valor do Servo: ");
  Serial.println(holdingRegs[SERVO_VALUE]);

if (umidade_solo > 500) {
    if (chuva_detectada == LOW) {
      Serial.println("Solo seco mas com chuva detectada. Acionando servo motor. Telhado aberto");
      holdingRegs[SERVO_VALUE] = 1;
      girarServo(180);
      
    } else if (chuva_detectada == HIGH) {
      Serial.println("Solo seco mas sem chuva detectada. Motor parado. Telhado fechado");
      holdingRegs[SERVO_VALUE] = 0;
      girarServo(0);
      
    }
    
    
  } else {
    if (chuva_detectada == LOW) {
      Serial.println("Solo molhado mas com chuva detectada. Motor parado. Telhado fechado");
      holdingRegs[SERVO_VALUE] = 0;
      girarServo(0);
      
    } else if (chuva_detectada == HIGH) {
      Serial.println("Solo molhado mas sem chuva detectada. Motor parado. Telhado fechado");
      holdingRegs[SERVO_VALUE] = 0;
      girarServo(0);
      
    }
  }

  modbus_update();

  delay(5000);
}

void girarServo(int angulo) {
  int posicao = map(angulo, 0, 180, 0, 180);
  servoMotor.write(posicao);
  delay(1000);
  modbus_update();
}
