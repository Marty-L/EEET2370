//GLOBAL VARIABLES
unsigned long freq = 10; //Desired LED blink frequency (Hz)
unsigned long HALF_PERIOD_MILLIS = long(abs((0.5)*(1.0/freq))*1000); //Half the period (ms) - cast for safety when using delay()

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);       //Set built-in LED port as output.
  
  Serial.begin(9600);                 //Set up serial port for logging output.
  Serial.println(HALF_PERIOD_MILLIS); //Echo to serial log to check value.
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);    //LED ON
  delay(HALF_PERIOD_MILLIS);
  digitalWrite(LED_BUILTIN, LOW);     //LED OFF
  delay(HALF_PERIOD_MILLIS);
}
