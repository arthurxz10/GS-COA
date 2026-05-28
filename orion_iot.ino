/*
 * ============================================================
 * MISSAO ORION — Cápsula Espacial Experimental
 * Comp. Organization & Architecture — GS 2026.1
 * FIAP • Turma 1CCPY • Grupo 05
 *
 * Integrantes:
 *   Arthur dos Santos Bezerra   RM 569721
 *   Carlos Henrique Fratezi     RM 571792
 *   Felipe Gouveia Braga        RM 568956
 *
 * Sensores simulados:
 *   - TMP36   → Temperatura interna da cápsula
 *   - LDR     → Luminosidade (painel solar / lado solar vs sombra)
 *   - SW-420  → Vibração estrutural da cápsula
 *
 * Display: LCD 16x2 (I2C simulado via pinos digitais)
 * Alertas: LEDs verde (OK) e vermelho (ALERTA) + buzzer passivo
 * ============================================================
 */

#include <LiquidCrystal.h>

// --- Pinos ---
// LCD: RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int PIN_TEMP      = A0;   // TMP36 — temperatura
const int PIN_LUZ       = A1;   // LDR   — luminosidade
const int PIN_VIBRACAO  = 7;    // SW-420 (digital) — vibração

const int LED_OK        = 8;    // LED verde
const int LED_ALERTA    = 9;    // LED vermelho
const int BUZZER        = 10;   // Buzzer passivo

// --- Limiares operacionais da cápsula ---
const float TEMP_MIN    = 18.0;
const float TEMP_MAX    = 35.0;
const int   LUZ_MIN     = 200;  // valor ADC mínimo (escuridão = possível falha painel)
const int   LUZ_MAX     = 900;  // valor ADC máximo (exposição solar excessiva)

// --- Variáveis ---
float temperatura;
int   luminosidade;
bool  vibracao;
bool  alertaAtivo     = false;
unsigned long ultimaAtualizacao = 0;
const unsigned long INTERVALO   = 2000; // ms entre leituras

// Caractere personalizado: foguete no LCD
byte foguete[8] = {
  0b00100,
  0b01110,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b01010,
  0b00000
};

// ============================================================
void setup() {
  Serial.begin(9600);

  pinMode(PIN_VIBRACAO, INPUT);
  pinMode(LED_OK,       OUTPUT);
  pinMode(LED_ALERTA,   OUTPUT);
  pinMode(BUZZER,       OUTPUT);

  lcd.begin(16, 2);
  lcd.createChar(0, foguete);

  // Splash screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.print(" MISSAO ORION");
  lcd.setCursor(0, 1);
  lcd.print("  INICIANDO...  ");
  delay(2500);
  lcd.clear();

  Serial.println("============================================");
  Serial.println("  MISSAO ORION — Mission Control IoT");
  Serial.println("  FIAP GS 2026.1 | Grupo 05 | 1CCPY");
  Serial.println("============================================");
  Serial.println("Timestamp(ms) | Temp(C) | Luz(ADC) | Vibr | Status");
}

// ============================================================
void loop() {
  unsigned long agora = millis();

  if (agora - ultimaAtualizacao >= INTERVALO) {
    ultimaAtualizacao = agora;

    // --- Leitura dos sensores ---
    temperatura  = lerTemperatura();
    luminosidade = analogRead(PIN_LUZ);
    vibracao     = digitalRead(PIN_VIBRACAO); // HIGH = vibração detectada

    // --- Análise de status ---
    alertaAtivo = verificarAlertas();

    // --- Exibição no LCD ---
    exibirLCD();

    // --- LEDs e buzzer ---
    controlarAlertas();

    // --- Log Serial ---
    logSerial(agora);
  }
}

// ============================================================
// Converte leitura ADC do TMP36 para graus Celsius
float lerTemperatura() {
  int raw = analogRead(PIN_TEMP);
  float tensao = raw * (5.0 / 1023.0);      // Converte ADC → Volts
  return (tensao - 0.5) * 100.0;             // Fórmula TMP36: (V - 0.5) * 100
}

// ============================================================
// Retorna true se qualquer parâmetro estiver fora dos limiares
bool verificarAlertas() {
  if (temperatura < TEMP_MIN || temperatura > TEMP_MAX) return true;
  if (luminosidade < LUZ_MIN || luminosidade > LUZ_MAX) return true;
  if (vibracao == HIGH) return true;
  return false;
}

// ============================================================
void exibirLCD() {
  lcd.clear();

  // Linha 0: temperatura e luminosidade
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperatura, 1);
  lcd.print("C");

  lcd.setCursor(8, 0);
  lcd.print("L:");
  lcd.print(luminosidade);

  // Linha 1: vibração e status
  lcd.setCursor(0, 1);
  lcd.print("VIB:");
  lcd.print(vibracao ? "SIM" : "NAO");

  lcd.setCursor(8, 1);
  if (alertaAtivo) {
    lcd.print("! ALERTA !");
  } else {
    lcd.print("   OK    ");
  }
}

// ============================================================
void controlarAlertas() {
  if (alertaAtivo) {
    digitalWrite(LED_OK,    LOW);
    digitalWrite(LED_ALERTA, HIGH);
    tone(BUZZER, 1000, 300);   // beep curto 1kHz
  } else {
    digitalWrite(LED_OK,    HIGH);
    digitalWrite(LED_ALERTA, LOW);
    noTone(BUZZER);
  }
}

// ============================================================
void logSerial(unsigned long ts) {
  Serial.print(ts);
  Serial.print("ms | T:");
  Serial.print(temperatura, 2);
  Serial.print("C | L:");
  Serial.print(luminosidade);
  Serial.print(" | VIB:");
  Serial.print(vibracao ? "SIM" : "NAO");
  Serial.print(" | ");

  if (!alertaAtivo) {
    Serial.println("STATUS: NOMINAL");
  } else {
    Serial.print("ALERTA: ");
    if (temperatura < TEMP_MIN) Serial.print("[TEMP BAIXA] ");
    if (temperatura > TEMP_MAX) Serial.print("[TEMP ALTA] ");
    if (luminosidade < LUZ_MIN) Serial.print("[LUZ BAIXA] ");
    if (luminosidade > LUZ_MAX) Serial.print("[LUZ ALTA] ");
    if (vibracao)                Serial.print("[VIBRACAO] ");
    Serial.println();
  }
}
