# Sistema de Irrigação Automática com ESP32 e Adafruit IO

![Status do Projeto](https://img.shields.io/badge/status-em%20desenvolvimento-yellow)

Este projeto implementa um sistema de irrigação automatizado usando **ESP32**, **sensor capacitivo de umidade do solo**, **boia de nível de água** e controle de bomba via **relé**. Os dados são enviados para a plataforma **Adafruit IO** para monitoramento remoto em tempo real.

---

## 🔧 Funcionalidades Principais

* **Monitoramento da Umidade do Solo:** Leitura contínua através do sensor capacitivo.
* **Detecção de Nível de Água:** Utilização de boia para verificar a disponibilidade de água no reservatório.
* **Controle Automatizado da Bomba:** Acionamento automático da bomba de 5V com base nos seguintes critérios:
    * Umidade do solo abaixo do limite definido.
    * Nível de água suficiente no reservatório.
* **Integração com Adafruit IO:** Envio de dados para monitoramento remoto:
    * **Feed `umidade_solo`:** Percentual atual da umidade.
    * **Feed `estado_boia`:** Status do nível da água (ex: OK/Baixo).
    * **Feed `rele`:** Estado atual da bomba (Ligada/Desligada).
* **Reset Diário:** Reinicialização automática da contagem de irrigações realizadas no dia.
* **Proteção Anti-Throttling:** Envio de dados para a nuvem em intervalos seguros (a cada 10-15 segundos) para evitar bloqueios da API do Adafruit IO.

---

## 📦 Componentes Necessários

| Componente | Pino / Observação |
| :--- | :--- |
| **ESP32 DevKit** | Placa microcontroladora principal. |
| **Sensor Capacitivo de Umidade do Solo** | Conectado a um pino analógico (ex: `GPIO 34`). |
| **Boia de Nível de Água** | Conectada a um pino digital com `INPUT_PULLUP` (ex: `GPIO 33`). |
| **Bomba de Água 5V** | Atuador principal, alimentado por fonte externa ou 5V do ESP32 (se a corrente permitir). |
| **Módulo de Relé 5V** | Interface de potência entre o ESP32 (3.3V) e a bomba (5V). Usar pino digital (ex: `GPIO 32`). |
| **Conexão Wi-Fi** | Necessária para comunicação com o Adafruit IO. |

---

## 🛠️ Instalação e Configuração

### 1. Pré-requisitos de Software

* [Arduino IDE](https://www.arduino.cc/en/software) ou [PlatformIO](https://platformio.org/) configurado para o ESP32.
* Bibliotecas necessárias (via Gerenciador de Bibliotecas):
    * `Adafruit MQTT Library`
    * `Adafruit IO Arduino`

### 2. Configuração Adafruit IO

1.  Crie uma conta no [Adafruit IO](https://io.adafruit.com/).
2.  Crie os seguintes Feeds:
    * `umidade_solo`
    * `estado_boia`
    * `rele`
3.  Obtenha suas chaves (AIO Key) e nome de usuário (Username) no painel do Adafruit IO.

### 3. Configuração do Código

No arquivo principal do projeto, insira suas credenciais:

Credenciais Wi-Fi

#define WIFI_SSID "NOME_DA_SUA_REDE_WIFI"

#define WIFI_PASS "SENHA_DA_SUA_REDE_WIFI"

Credenciais Adafruit IO

#define IO_USERNAME "SEU_USUARIO_ADAFRUIT_IO"
#define IO_KEY      "SUA_CHAVE_ADAFRUIT_IO"
