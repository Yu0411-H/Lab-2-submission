// 摩尔斯电码解码器 - ESP32-C3
// 串口波特率: 115200

const int BTN = 5;      // 按钮接 GPIO 5 → GND（内部上拉）
const int DOT_LED = 1;  // 点 (绿)
const int DASH_LED = 2; // 划 (蓝)

// 时间阈值 (ms) —— 在 README 里说明这些值
const unsigned long DOT_MAX = 200;     // < 200ms = 点
const unsigned long DASH_MIN = 200;    // ≥ 200ms = 划
const unsigned long CHAR_GAP = 400;    // 松开后 400ms 无输入 = 字符结束
const unsigned long WORD_GAP = 1000;   // 字符后 1000ms 无输入 = 空格

unsigned long pressStart = 0;
unsigned long lastRelease = 0;
bool btnPressed = false;
bool inChar = false;

uint8_t morse = 0;     // 存储点和划的位模式
uint8_t morseLen = 0;  // 当前字符有几个符号

void setup() {
  Serial.begin(115200);
  pinMode(BTN, INPUT_PULLUP);
  pinMode(DOT_LED, OUTPUT);
  pinMode(DASH_LED, OUTPUT);
  Serial.println("Ready. Start tapping Morse code.");
}

void loop() {
  unsigned long now = millis();
  bool pressed = (digitalRead(BTN) == LOW);

  // ========== 检测按下 ==========
  if (pressed && !btnPressed) {
    btnPressed = true;
    pressStart = now;
    inChar = true;
  }

  // ========== 检测释放 ==========
  if (!pressed && btnPressed) {
    btnPressed = false;
    unsigned long dur = now - pressStart;
    lastRelease = now;

    if (dur < DOT_MAX) {
      digitalWrite(DOT_LED, HIGH);
      addSymbol(0);  // 0 = 点
    } else {
      digitalWrite(DASH_LED, HIGH);
      addSymbol(1);  // 1 = 划
    }

    delay(50);  // LED 亮 50ms
    digitalWrite(DOT_LED, LOW);
    digitalWrite(DASH_LED, LOW);
  }

  // ========== 字符超时：解码并打印到串口 ==========
  if (inChar && !pressed && (now - lastRelease >= CHAR_GAP)) {
    char c = decodeMorse(morse, morseLen);
    Serial.print(c);        // ← 关键：把字母显示在串口监视器上
    morse = 0;
    morseLen = 0;
    inChar = false;
  }

  // ========== 单词超时：输出空格 ==========
  if (!inChar && !pressed && (now - lastRelease >= WORD_GAP)) {
    Serial.print(' ');
    lastRelease = now;  // 防止连续输出空格
  }
}

// 添加一个符号到位模式 (0=点, 1=划)
void addSymbol(uint8_t bit) {
  if (morseLen < 8) {
    morse = (morse << 1) | bit;
    morseLen++;
  }
}

// 解码摩尔斯电码
char decodeMorse(uint8_t pattern, uint8_t len) {
  if (len == 1) {
    if (pattern == 0b0) return 'E';
    if (pattern == 0b1) return 'T';
  }
  if (len == 2) {
    if (pattern == 0b00) return 'I';
    if (pattern == 0b01) return 'A';
    if (pattern == 0b10) return 'N';
    if (pattern == 0b11) return 'M';
  }
  if (len == 3) {
    if (pattern == 0b000) return 'S';
    if (pattern == 0b001) return 'U';
    if (pattern == 0b010) return 'R';
    if (pattern == 0b011) return 'W';
    if (pattern == 0b100) return 'D';
    if (pattern == 0b101) return 'K';
    if (pattern == 0b110) return 'G';
    if (pattern == 0b111) return 'O';
  }
  if (len == 4) {
    if (pattern == 0b0000) return 'H';
    if (pattern == 0b0001) return 'V';
    if (pattern == 0b0010) return 'F';
    if (pattern == 0b0100) return 'L';
    if (pattern == 0b0110) return 'P';
    if (pattern == 0b0111) return 'J';
    if (pattern == 0b1000) return 'B';
    if (pattern == 0b1001) return 'X';
    if (pattern == 0b1010) return 'C';
    if (pattern == 0b1011) return 'Y';
    if (pattern == 0b1100) return 'Z';
    if (pattern == 0b1101) return 'Q';
  }
  return '?';
}
