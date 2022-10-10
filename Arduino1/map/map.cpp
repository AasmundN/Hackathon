long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void setup() {
  Serial.begin(9600);
  // 5 in the range between 0 and 10 is 50 in the range between 0 and 100
  Serial.println(map(5, 0, 10, 0, 100));
}


void loop() {

}
