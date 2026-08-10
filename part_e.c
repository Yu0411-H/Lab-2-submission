// 摩尔斯电码解码器 - ESP32-C3
// 按钮: GPIO 5 (接 GND, 内部上拉)
// Dot LED: GPIO 1 (绿)   Dash LED: GPIO 2 (蓝)

const int BUTTON_PIN = 5;    // 避开 GPIO 0 启动引脚
const int DOT_LED = 1;       // 点 → 绿灯
const int DASH_LED = 2;      // 划 → 蓝灯

// 时间阈值 (ms) —— 按作业要求，可根据手感调整并在 README 中说明
const unsigned long DOT_MAX = 200;      // < 200ms = 点
const unsigned long CHAR_GAP = 300;     // 松开后 300ms = 字符结束
const unsigned long WORD_GAP = 800;     // 松开后 800ms = 单词空格
const unsigned long LED_ON_TIME = 50;   // LED 亮起时间

unsigned long pressStart = 0;
unsigned long lastRelease = 0;
unsigned long afterCharTime = 0;
bool wasPressed = false;

uint8_t morseBuf = 0;
uint8_t morseLen = 0;

unsigned long dotLedOff = 0;
unsigned long dashLedOff = 0;

enum State { IDLE, IN_CHAR, AFTER_CHAR };
State state = IDLE;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(DOT_LED, OUTPUT);
  pinMode(DASH_LED, OUTPUT);
  digitalWrite(DOT_LED, LOW);
  digitalWrite(DASH_LED, LOW);
}

void loop() {
  bool pressed = (digitalRead(BUTTON_PIN) == LOW);  // 按下为 LOW
  unsigned long now = millis();

  // 检测按下 (下降沿)
  if (pressed && !wasPressed) {
    pressStart = now;
    wasPressed = true;
    state = IN_CHAR;
  }

  // 检测释放 (上升沿)
  if (!pressed && wasPressed) {
    unsigned long duration = now - pressStart;
    wasPressed = false;
    lastRelease = now;

    if (duration < DOT_MAX) {
      digitalWrite(DOT_LED, HIGH);
      dotLedOff = now + LED_ON_TIME;
      addSymbol(0);  // 0 = dot
    } else {
      digitalWrite(DASH_LED, HIGH);
      dashLedOff = now + LED_ON_TIME;
      addSymbol(1);  // 1 = dash
    }
  }

  // LED 50ms 后熄灭
  if (dotLedOff > 0 && now >= dotLedOff) {
    digitalWrite(DOT_LED, LOW);
    dotLedOff = 0;
  }
  if (dashLedOff > 0 && now >= dashLedOff) {
    digitalWrite(DASH_LED, LOW);
    dashLedOff = 0;
  }

  // 字符超时：字符间隔
  if (state == IN_CHAR && !pressed && (now - lastRelease >= CHAR_GAP)) {
    decodeAndPrint();
    state = AFTER_CHAR;
    afterCharTime = now;
  }

  // 单词超时：单词间隔
  if (state == AFTER_CHAR && !pressed && (now - afterCharTime >= WORD_GAP)) {
    Serial.print(' ');
    state = IDLE;
  }
}

void addSymbol(uint8_t isDash) {
  if (morseLen < 8) {
    morseBuf = (morseBuf << 1) | isDash;
    morseLen++;
  }
}

void decodeAndPrint() {
  char c = decode(morseBuf, morseLen);
  Serial.print(c);
  morseBuf = 0;
  morseLen = 0;
}

char decode(uint8_t pattern, uint8_t len) {
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
