// === PIN DEFINITIONS ===
const int pinUmiditate = A0;  // senzor de umiditate
const int pinLuminaAO = A1;   // fotodiodă (citire analogică)

// D2 → PD2 → LED
// D3 → PD3 → Buzzer

void setup() {
  // Setăm D2 și D3 ca OUTPUT prin registre
  DDRD |= (1 << PD2);  // D2 (LED)
  DDRD |= (1 << PD3);  // D3 (Buzzer)

  Serial.begin(9600);
  Serial.println("Proiect cu acces direct la registri - LED și buzzer");
}

void loop() {
  int valoareUmiditate = analogRead(pinUmiditate);
  int umiditateProc = map(valoareUmiditate, 0, 1023, 100, 0);

  int valoareLumina = analogRead(pinLuminaAO); // 950 - 1023
  bool esteZi = valoareLumina < 750;

  Serial.print("Umiditate: ");
  Serial.print(umiditateProc);
  Serial.print("% | Lumina AO: ");
  Serial.print(valoareLumina);
  Serial.print(" → ");
  Serial.println(esteZi ? "ZI" : "NOAPTE");

  if (esteZi) {
    int pwmLED = map(umiditateProc, 0, 100, 255, 0);
    analogWrite(2, pwmLED); // PWM tot prin analogWrite, dar activ doar ziua

    alertaBuzzer(umiditateProc);
  } else {
    // Stingem LED și buzzer folosind registre
    PORTD &= ~(1 << PD2);  // LED OFF
    PORTD &= ~(1 << PD3);  // Buzzer OFF
  }

  delay(1000);
}

void alertaBuzzer(int umiditate) {
  if (umiditate < 20) {
    PORTD |= (1 << PD3);  // Buzzer ON
    delay(500);
    PORTD &= ~(1 << PD3); // Buzzer OFF
  } else if (umiditate < 40) {
    for (int i = 0; i < 2; i++) {
      PORTD |= (1 << PD3);
      delay(100);
      PORTD &= ~(1 << PD3);
      delay(100);
    }
  }
}
