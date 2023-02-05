
struct data_ {
    uint16_t A0;
    uint16_t A1;
    uint16_t A2;
    uint16_t A3;
    uint16_t A4;
    uint16_t A5;
};

data_ data;

void setup() {
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    Serial.begin(1000000);
}

void sendBinary(int counts)
{
    for (int i = 0; i < counts; i++) {
        data.A0 = analogRead(A0);
        data.A1 = analogRead(A1);
        data.A2 = analogRead(A2);
        data.A3 = analogRead(A3);
        data.A4 = analogRead(A4);
        data.A5 = analogRead(A5);
        Serial.write((byte *) & data, sizeof(data));
    }
}

void sendStr() {
    for (int i = 0; i < 100; i++) {
        Serial.print(analogRead(A0));
        Serial.print(" ");
    }
    Serial.println();
}

void loop() {
    if (Serial.available()) {
        // String buf = Serial.readString();
        int counts = Serial.parseInt();
        if (counts) {
            sendBinary(counts);
        }
    }

}
