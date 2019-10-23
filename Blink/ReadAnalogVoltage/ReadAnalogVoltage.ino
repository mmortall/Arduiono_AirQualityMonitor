const int movPin = 2;

void setup() 
{
    Serial.begin(9600);
    pinMode(movPin, INPUT);
}

void loop(){
    int val = digitalRead(movPin);
    Serial.println(val);
    delay(10);
}
