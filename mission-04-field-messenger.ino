/* =============================================================================
   OPERATION: SIGNALS & SECRETS  —  MISSION 04
   "Field Messenger" : a two-way LoRa communicator for the Heltec WiFi LoRa 32 V3
   -----------------------------------------------------------------------------
   Flash this SAME sketch onto BOTH boards. Each board listens all the time.
   Type a message in the Arduino Serial Monitor and press Enter to send it to
   the other board. Incoming messages appear on the little OLED screen along
   with how strong the signal was (RSSI) and how clean it was (SNR).

   ---------------------------------------------------------------------------
   ONE-TIME SETUP IN THE ARDUINO IDE
   ---------------------------------------------------------------------------
   1. File > Preferences > "Additional boards manager URLs", add:
        https://resource.heltec.cn/download/package_heltec_esp32_index.json
   2. Tools > Board > Boards Manager: install  "Heltec ESP32 Series Dev-boards".
   3. Tools > Manage Libraries, install:
        - "RadioLib"  by Jan Gromes
        - "U8g2"      by oliver
   4. Tools > Board: choose  "Heltec WiFi LoRa 32(V3)".
   5. Plug in a board, pick its Port, set Serial Monitor to 115200 baud.

   ⚠️  ALWAYS screw the antenna on BEFORE powering the board. Transmitting with
       no antenna can damage the radio.

   ---------------------------------------------------------------------------
   THE EXPERIMENT (this is the science part!)
   ---------------------------------------------------------------------------
   The four numbers in the TUNABLE block below control the radio. The big one
   is SPREADING_FACTOR. Send a message, write down the RSSI shown on the screen,
   walk farther apart, send again, write down the new RSSI. Make a table of
   distance vs RSSI, then GRAPH it. Next, change SPREADING_FACTOR from 7 up to
   12, re-flash both boards, and repeat. Higher SF reaches farther but sends
   slower — that trade-off is Claude Shannon's channel-capacity law, measured
   with your own two hands.
   ============================================================================= */

#include <RadioLib.h>
#include <U8g2lib.h>
#include <Wire.h>

/* ----------------------------- TUNABLE BLOCK -------------------------------
   Change these, re-flash BOTH boards, and watch what happens. */
#define NODE_NAME        "A"      // set one board to "A" and the other to "B"
float   FREQUENCY      = 915.0;   // MHz. 915 = USA. (Europe would be 868.0)
int     SPREADING_FACTOR = 7;     // 7 (fast, short) ... 12 (slow, long range)
float   BANDWIDTH      = 125.0;   // kHz. Smaller = more range, slower.
int     CODING_RATE    = 5;       // 5..8. Higher = more error protection.
int     TX_POWER       = 14;      // dBm, 2..22. Higher = more range, more battery.
/* ------------------------------------------------------------------------- */

/* ---- Heltec WiFi LoRa 32 V3 pin map (verified against the V3 schematic) --- */
#define PIN_LORA_NSS    8
#define PIN_LORA_DIO1   14
#define PIN_LORA_RST    12
#define PIN_LORA_BUSY   13
#define PIN_LORA_SCK    9
#define PIN_LORA_MISO   11
#define PIN_LORA_MOSI   10

#define PIN_OLED_SDA    17
#define PIN_OLED_SCL    18
#define PIN_OLED_RST    21
#define PIN_VEXT        36        // controls power to the OLED (active LOW)

/* ---- Build the radio and screen objects ---- */
SPIClass loraSPI(HSPI);
SX1262 radio = new Module(PIN_LORA_NSS, PIN_LORA_DIO1, PIN_LORA_RST, PIN_LORA_BUSY, loraSPI);

U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled(U8G2_R0, PIN_OLED_RST, PIN_OLED_SCL, PIN_OLED_SDA);

/* ---- A flag the radio sets (in an interrupt) when a packet arrives ---- */
volatile bool packetArrived = false;
void onPacket() { packetArrived = true; }   // keep this tiny — it's an interrupt

/* ---- Draw a few lines on the little screen ---- */
void show(String line1, String line2, String line3, String line4) {
  oled.clearBuffer();
  oled.setFont(u8g2_font_6x12_tr);
  oled.drawStr(0, 12, line1.c_str());
  oled.drawStr(0, 28, line2.c_str());
  oled.drawStr(0, 44, line3.c_str());
  oled.drawStr(0, 60, line4.c_str());
  oled.sendBuffer();
}

void setup() {
  Serial.begin(115200);
  delay(300);

  // Power up the OLED (Vext is active LOW on the V3)
  pinMode(PIN_VEXT, OUTPUT);
  digitalWrite(PIN_VEXT, LOW);
  delay(50);

  oled.begin();
  show("Node " NODE_NAME " starting", "Waking the radio...", "", "");

  // Start the SPI bus on the V3's LoRa pins
  loraSPI.begin(PIN_LORA_SCK, PIN_LORA_MISO, PIN_LORA_MOSI, PIN_LORA_NSS);

  // Start the radio. The 1.8 is the TCXO voltage the V3 board uses.
  int state = radio.begin(FREQUENCY, BANDWIDTH, SPREADING_FACTOR,
                          CODING_RATE, RADIOLIB_SX126X_SYNC_WORD_PRIVATE,
                          TX_POWER, 8, 1.8, false);

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("Radio ready.");
  } else {
    Serial.print("Radio failed, code ");
    Serial.println(state);
    show("Radio FAILED", "error code:", String(state), "check wiring/antenna");
    while (true) { delay(1000); }   // stop here so the error stays on screen
  }

  // Tell the radio to call onPacket() the instant a message lands,
  // then start listening.
  radio.setDio1Action(onPacket);
  radio.startReceive();

  show("Node " NODE_NAME " READY", "SF=" + String(SPREADING_FACTOR) +
       " " + String(FREQUENCY, 1) + "MHz", "Listening...",
       "Type in Serial->send");
  Serial.println("Type a message and press Enter to send it.");
}

void loop() {
  // ---- 1. Did a message arrive? ----
  if (packetArrived) {
    packetArrived = false;

    String incoming;
    int state = radio.readData(incoming);

    if (state == RADIOLIB_ERR_NONE) {
      float rssi = radio.getRSSI();   // signal strength, in dBm (closer to 0 = stronger)
      float snr  = radio.getSNR();    // signal-to-noise ratio, in dB (higher = cleaner)

      Serial.println("--- message received ---");
      Serial.println("text : " + incoming);
      Serial.println("RSSI : " + String(rssi, 1) + " dBm");
      Serial.println("SNR  : " + String(snr, 1) + " dB");

      show("MSG: " + incoming,
           "RSSI " + String(rssi, 0) + " dBm",
           "SNR  " + String(snr, 1) + " dB",
           "(write these down!)");
    }
    // go back to listening for the next one
    radio.startReceive();
  }

  // ---- 2. Did you type something to send? ----
  if (Serial.available()) {
    String outgoing = Serial.readStringUntil('\n');
    outgoing.trim();

    if (outgoing.length() > 0) {
      // tag the message with which node it came from
      String packet = "[" NODE_NAME "] " + outgoing;

      show("Sending...", packet, "", "");
      int state = radio.transmit(packet);   // this pauses briefly to send

      if (state == RADIOLIB_ERR_NONE) {
        Serial.println("Sent: " + packet);
        show("Sent OK", packet, "Listening...", "");
      } else {
        Serial.println("Send failed, code " + String(state));
        show("Send FAILED", "code " + String(state), "", "");
      }

      packetArrived = false;     // ignore the radio's own "done sending" blip
      radio.startReceive();      // start listening again
    }
  }
}
