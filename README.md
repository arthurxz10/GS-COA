# 🚀 Missão ORION — Simulação IoT de Cápsula Espacial

**Disciplina:** Computer Organization & Architecture  
**Global Solution 2026.1 | FIAP — Turma 1CCPY | Grupo 05**

| Integrante | RM |
|---|---|
| Arthur dos Santos Bezerra | 569721 |
| Carlos Henrique Fratezi | 571792 |
| Felipe Gouveia Braga | 568956 |

---

## 📌 Descrição do Projeto

Este projeto simula o sistema de sensoriamento embarcado da **cápsula espacial experimental ORION**, desenvolvida como hub de dados da Global Solution 2026.1. O circuito IoT, executado em **Arduino Uno** e simulado no **Tinkercad**, monitora em tempo real três variáveis críticas da missão:

| Sensor | Parâmetro monitorado | Representação na missão |
|---|---|---|
| **TMP36** | Temperatura (°C) | Temperatura interna da cápsula |
| **LDR** | Luminosidade (ADC 0–1023) | Exposição solar / orientação dos painéis |
| **Botão (pushbutton)** | Vibração (digital) | Integridade estrutural da cápsula (simula SW-420) |

---

## 🔧 Componentes Utilizados

| Componente | Quantidade | Função |
|---|---|---|
| Arduino Uno | 1 | Microcontrolador principal |
| TMP36 | 1 | Sensor de temperatura |
| LDR (fotoresistor) | 1 | Sensor de luminosidade |
| Resistor 10kΩ (LDR pull-down) | 1 | Divisor de tensão do LDR |
| Botão (pushbutton) | 1 | Simulação do sensor de vibração SW-420 |
| LCD 16x2 | 1 | Display de status da missão |
| Resistores (220Ω para LEDs) | 2 | Proteção dos LEDs |
| LED Verde | 1 | Indicador de status NOMINAL |
| LED Vermelho | 1 | Indicador de ALERTA |
| Buzzer passivo | 1 | Alerta sonoro |
| Protoboard + jumpers | — | Conexões |

---

## 🗺️ Mapeamento de Pinos

```
Arduino Uno
│
├── A0 ──── TMP36 (saída analógica)
├── A1 ──── LDR (divisor de tensão com resistor 10kΩ para GND)
├── D7 ──── Botão vibração (INPUT_PULLUP — LOW quando pressionado)
│
├── D2 ──── LCD D7
├── D3 ──── LCD D6
├── D4 ──── LCD D5
├── D5 ──── LCD D4
├── D11 ─── LCD EN (Enable)
├── D12 ─── LCD RS (Register Select)
│
├── D8 ──── LED Verde (+ resistor 220Ω → GND)
├── D9 ──── LED Vermelho (+ resistor 220Ω → GND)
└── D10 ─── Buzzer passivo (negativo → GND)
```

> **LCD:** Alimentação em 5V, VSS no GND, V0 (contraste) em GND ou potenciômetro 10kΩ, RW no GND, backlight A em 5V via resistor 220Ω, backlight K no GND.

---

## ⚙️ Funcionamento

### Ciclo de Leitura
A cada **2 segundos** o Arduino:
1. Lê a temperatura via TMP36 (conversão ADC → Volts → °C pela fórmula `(V - 0.5) × 100`)
2. Lê a luminosidade via LDR (valor ADC de 0 a 1023)
3. Lê o estado de vibração via botão com INPUT_PULLUP (LOW quando pressionado = vibração detectada)
4. Atualiza o LCD com os valores e o status da missão
5. Aciona LEDs e buzzer conforme o status
6. Envia log completo pela Serial (9600 baud)

### Limiares Operacionais

| Parâmetro | Mínimo | Máximo | Fora do limiar |
|---|---|---|---|
| Temperatura | 18 °C | 35 °C | ⚠️ ALERTA |
| Luminosidade (ADC) | 200 | 900 | ⚠️ ALERTA |
| Vibração | — | — | ⚠️ ALERTA (qualquer detecção) |

### Indicadores de Status

| Condição | LED Verde | LED Vermelho | Buzzer | LCD |
|---|---|---|---|---|
| Todos os parâmetros nominais | ✅ Aceso | ❌ Apagado | Silencioso | `OK` |
| Qualquer parâmetro fora do limiar | ❌ Apagado | ✅ Aceso | Beep 1kHz | `! ALERTA !` |

---

## 🖥️ Saída Serial (exemplo)

```
============================================
  MISSAO ORION — Mission Control IoT
  FIAP GS 2026.1 | Grupo 05 | 1CCPY
============================================
Timestamp(ms) | Temp(C) | Luz(ADC) | Vibr | Status
2000ms | T:24.32C | L:512 | VIB:NAO | STATUS: NOMINAL
4000ms | T:38.10C | L:487 | VIB:NAO | ALERTA: [TEMP ALTA]
6000ms | T:25.00C | L:150 | VIB:SIM | ALERTA: [LUZ BAIXA] [VIBRACAO]
```

---

## 🔗 Simulação no Tinkercad

> 🔗 **[Inserir link do projeto público no Tinkercad aqui]**

Para visualizar:
1. Acesse o link acima
2. Clique em **"Simulate"**
3. Ajuste os potenciômetros/sliders dos sensores para testar diferentes condições
4. Observe o LCD, os LEDs e abra o **Serial Monitor** para ver os logs

---

## 📁 Estrutura do Repositório

```
/
├── orion_iot.ino          # Código-fonte Arduino completo
├── README.md              # Este arquivo
└── evidencias/
    └── (prints do Tinkercad simulando condições nominal e alerta)
```

---

## 🔗 Conexão com a Missão ORION

Este circuito representa a **camada física de sensoriamento** da cápsula ORION. Os dados gerados por estes sensores alimentam:

- A **matriz de ciclos em Python** (Pensamento Computacional)
- O **sistema de monitoramento em C** (Data Structures)
- O **sistema de IA com Llama** (Prompt & IA)
- O **dashboard de energia renovável** (Energias Renováveis)

Todos os módulos da GS utilizam os mesmos parâmetros (temperatura, luz/energia, vibração) como base narrativa comum da Missão ORION.

---

*FIAP — Global Solution 2026.1 | Entrega: 09/06/2026*
