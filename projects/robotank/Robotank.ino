#include <PsxControllerHwSpi.h>
#include <Servo.h>

Servo srvClaw;  // объявление перемнной типа Servo для управления 
                // клешней (claw)
Servo srvUpDn;
Servo srvFwdBwd;
Servo srvLftRgt;

// для обработки нажатий кнопок
bool btnFlag = false;
uint32_t btnTimer = 0;

// Обработка правого джойстика
#define MRX 128 // середина правого джойстика по оси x (middle right x)
#define MRY 128 // середина левого джойстика по оси x (middle left x)
#define DRX 5   // отклонение по оси x (delta x)
#define DRY 5   // отклонеие по оси y (delta y)

#define MA_FWD 6 // пин: мотор А, направление вперед (motor A forward)
                 // желтый провод, IN2
#define MA_BWD 9 // пин: мотор B, направление назад (motor A backward)
                 // оранжевый провод, IN1 
#define MB_FWD 3 // пин: мотор B, направление вперед (motor B forward)
                 // коричневый провод, IN4
#define MB_BWD 5 // пин: мотор B, направление назад (motor B backward)
                 // черный провод, IN3

#define MA_SPD 255 // мотор A, максимальная скорость
#define MB_SPD 255 // мотор B, максимальная скорость

// Вывод ATT (SS) адаптера
const byte PIN_PS2_ATT = 10;

PsxControllerHwSpi<PIN_PS2_ATT> psx;

boolean haveGamepad = false;

// Типы контроллеров
const char* controllerTypeStrings[PSCTRL_MAX] = {
  "Unknown",
  "Dual Shock",
  "Dual Shock Wireless",
  "Guitar Hero"
};

// Типы протоколов
const char* controllerProtoStrings[PSPROTO_MAX] = {
  "Unknown",
  "Digital",
  "Dual Shock",
  "Dual Shock 2",
  "Flightstick",
  "neGcon",
  "Jogcon"
};

// Обработка геймпада
void handleGamepad() {
  if (!haveGamepad) {
    gamepadSearch();
  }
  else {
    gamepadRead();
  }
}

// Поиск геймпада
void gamepadSearch() {
  if (psx.begin ()) {
    Serial.println ("Найден геймпад!");
    delay (300);
    if (!psx.enterConfigMode ()) {
      Serial.println ("Не получается войти в режим конфигурации");
    }
    else {
      PsxControllerType ctype = psx.getControllerType ();
      const char* cname = controllerTypeStrings[ctype < PSCTRL_MAX ? static_cast<byte> (ctype) : PSCTRL_MAX];
      Serial.print ("Тип геймпада: ");
      Serial.println(cname);
      if (!psx.enableAnalogSticks ()) {
        Serial.println ("Не получилось инициализировать джойстики");
      }
      if (!psx.exitConfigMode ()) {
        Serial.println ("Не получилось выйти из режима конфигурации");
      }
    }
    psx.read ();
    PsxControllerProtocol proto = psx.getProtocol ();
    const char* pname = controllerProtoStrings[proto < PSPROTO_MAX ? static_cast<byte> (proto) : PSPROTO_MAX];
    Serial.print ("Протокол геймпада: ");
    Serial.println(pname);
    haveGamepad = true;
  }
}

// Чтение геймпада
void gamepadRead()
{
  if (!psx.read ()) {
    Serial.println("Геймпад отключён"); 
    haveGamepad = false;
  }
}

void setup () {
  srvClaw.attach(4);  // привязываем сервопривод к выходу D2
  srvUpDn.attach(2);
  srvLftRgt.attach(7);
  srvFwdBwd.attach(8);

  pinMode(MA_FWD, OUTPUT);
  pinMode(MA_BWD, OUTPUT);
  pinMode(MB_FWD, OUTPUT);
  pinMode(MB_BWD, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT); 
  delay (300);
  Serial.begin (115200);
  // Ждём последовательный порт на Arduino Leonardo
  while (!Serial) {
    fastDigitalWrite (LED_BUILTIN, (millis () / 333) % 2);
  }
  Serial.println("Стартуем!");
}

void loop () {
  static bool changed = false;
  static uint8_t last_lx, last_ly, last_rx, last_ry;
  uint8_t lx, ly, rx, ry;
  uint16_t buttons = 0;

  // Проверяем наличие или отсутствие геймпада
  handleGamepad();

  // Читаем положение левого джойстика
  psx.getLeftAnalog(lx, ly);

  // Обновляем данные, если был сдвинут левый джойстик
  if (lx != last_lx || ly != last_ly) {
    last_lx = lx;
    last_ly = ly;
    changed = true;
  }

  // Читаем положение правого джойстика
  psx.getRightAnalog(rx, ry);

  // Обновляем данные, если был сдвинут правый джойстик
  if (rx != last_rx || ry != last_ry) {
    last_rx = rx;
    last_ry = ry;
    changed = true;
  }

    // считываем шестнадцатибитное слово состояния кнопок
    buttons = psx.getButtonWord();
    if (buttons == PSB_PAD_UP) {
      if (srvUpDn.read() >= 5) {
        srvUpDn.write(srvUpDn.read() - 5);
      }
    }

    if (buttons == PSB_PAD_DOWN) {
      if (srvUpDn.read() <= 175) {
        srvUpDn.write(srvUpDn.read() + 5);
      }
    }    

    if (buttons == PSB_PAD_LEFT) {
      if (srvClaw.read() >= 5) {
        srvClaw.write(srvClaw.read() - 5);
      }
    }

    if (buttons == PSB_PAD_RIGHT) {
      if (srvClaw.read() <= 175) {
        srvClaw.write(srvClaw.read() + 5);
      }
    }    




  // Если поступили новые данные о состоянии геймпада
  if (changed) {
    changed = false;
    // Формируем строку и выводим в монитор порта
    Serial.print("Левый джойстик: ");
    Serial.print(lx);
    Serial.print(", ");
    Serial.print(ly);
    Serial.println();
    Serial.print("Правый джойстик: ");
    Serial.print(rx);
    Serial.print(", ");
    Serial.println(ry);

    if ((ry < MRY - DRY) && // ry < 128 - 5
        (rx > MRX - DRX) && (rx < MRX + DRX)) { // rx в мертвой зоне
      // MOVE FORWARD
      analogWrite(MA_FWD, MA_SPD);
      analogWrite(MA_BWD, LOW);
      analogWrite(MB_FWD, MA_SPD);
      analogWrite(MB_BWD, LOW);      
    }

    else if ((ry < MRY - DRY) && // ry < 128 - 5
             (rx > MRX + DRX)) { // rx > 128 + 5
      // MOVE FORWARD RIGHT
      analogWrite(MA_FWD, MA_SPD);
      analogWrite(MA_BWD, LOW);
      analogWrite(MB_FWD, MA_SPD / 2);
      analogWrite(MB_BWD, LOW);      
    }

    else if ((ry < MRY - DRY) && // ry < 128 - 5
             (rx < MRX - DRX)) { // rx < 128 - 5
      // MOVE FORWARD LEFT
      analogWrite(MA_FWD, MA_SPD / 2);
      analogWrite(MA_BWD, LOW);
      analogWrite(MB_FWD, MA_SPD);
      analogWrite(MB_BWD, LOW);      
    }

    if ((ry > MRY + DRY) && // ry > 128 + 5
        (rx > MRX - DRX) && (rx < MRX + DRX)) { // rx в мертвой зоне
      // MOVE BACKWARD
      analogWrite(MA_FWD, LOW);
      analogWrite(MA_BWD, MA_SPD);
      analogWrite(MB_FWD, LOW);
      analogWrite(MB_BWD, MB_SPD); 
    }

    else if ((ry > MRY + DRY) && // ry > 128 + 5
             (rx > MRX + DRX)) { // rx > 128 + 5
      // MOVE BACKWARD RIGHT
      analogWrite(MA_FWD, LOW);
      analogWrite(MA_BWD, MA_SPD);
      analogWrite(MB_FWD, LOW);
      analogWrite(MB_BWD, MB_SPD / 2);      
    }

    else if ((ry > MRY + DRY) && // ry > 128 + 5
             (rx < MRX - DRX)) { // rx < 128 - 5
      // MOVE BACKWARD LEFT
      analogWrite(MA_FWD, LOW);
      analogWrite(MA_BWD, MA_SPD / 2);
      analogWrite(MB_FWD, LOW);
      analogWrite(MB_BWD, MB_SPD);
    }

    else if ((ry > MRY - DRY) && (ry < MRY + DRY) && // ry в мертвой зоне
             (rx > MRX + DRX)) { // rx > 128 + 5
      // TURN RIGHT
      analogWrite(MA_FWD, MA_SPD);
      analogWrite(MA_BWD, LOW);
      analogWrite(MB_FWD, LOW);
      analogWrite(MB_BWD, MB_SPD);       
    }

    else if ((ry > MRY - DRY) && (ry < MRY + DRY) && // ry в мертвой зоне
             (rx < MRX - DRX)) { // rx < 128 - 5
      // TURN LEFT
      analogWrite(MA_FWD, LOW);
      analogWrite(MA_BWD, MA_SPD);
      analogWrite(MB_FWD, MB_SPD);
      analogWrite(MB_BWD, LOW);       
    }

    else if ((ry > MRY - DRY) && (ry < MRY + DRY) && // ry в мертвой зоне
             (rx > MRX - DRX) && (rx < MRX + DRX)) { // rx в мертвой зоне
      // MOVE STOP
      analogWrite(MA_FWD, LOW);
      analogWrite(MA_BWD, LOW);
      analogWrite(MB_FWD, LOW);
      analogWrite(MB_BWD, LOW);      
    }
    Serial.println();
  }

  else {

    // rotate claw right
    if ((lx > 128) && (srvLftRgt.read() < 180)) {
      srvLftRgt.write(srvLftRgt.read() + 5);
    }

    // rotate claw left
    if ((lx < 128) && (srvLftRgt.read() > 0)) {
      srvLftRgt.write(srvLftRgt.read() - 5);
    }

    if ((ly < 128) && (srvFwdBwd.read() > 0)) {
      srvFwdBwd.write(srvFwdBwd.read() - 5);
    }

    if ((ly > 128) && (srvFwdBwd.read() < 180)) {
      srvFwdBwd.write(srvFwdBwd.read() + 5);
    }


  }
  delay (50);
}