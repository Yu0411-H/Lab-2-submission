// 摩尔斯电码发送器 - ESP32-C3
// 发送消息: "wljmzc"

const int LED_PIN = 1;  // LED 接 GPIO 1

// 时间参数 (ms)
const int DOT = 200;         // 点
const int DASH = 600;        // 划 = 3 × 点
const int SYMBOL_GAP = 200;  // 符号间隔
const int LETTER_GAP = 600;  // 字母间隔
const int WORD_GAP = 1400;   // 单词间隔

// A-Z 摩尔斯电码表
const char* morse[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
  "..-", "...-", ".--", "-..-", "-.--", "--.."
};

const char* message = "wljmzc";

void setup() {
  pinMode(LED_PIN, OUTPUT);
  delay(1000);  // 启动后等待 1 秒
}

void loop() {
  sendMessage(message);
  delay(3000);  // 重复间隔
}

void sendMessage(const char* msg) {
  for (int i = 0; msg[i] != '\0'; i++) {
    char c = msg[i];
    if (c == ' ') {
      delay(WORD_GAP);
      continue;
    }
    if (c >= 'a' && c <= 'z') c -= 32;  // 转大写
    if (c >= 'A' && c <= 'Z') {
      sendLetter(morse[c - 'A']);
      if (msg[i+1] != '\0' && msg[i+1] != ' ') delay(LETTER_GAP);
    }
  }
}

void sendLetter(const char* code) {
  for (int i = 0; code[i] != '\0'; i++) {
    if (code[i] == '.') flash(DOT);
    else if (code[i] == '-') flash(DASH);
    if (code[i+1] != '\0') delay(SYMBOL_GAP);
  }
}

void flash(int t) {
  digitalWrite(LED_PIN, HIGH);
  delay(t);
  digitalWrite(LED_PIN, LOW);
}
