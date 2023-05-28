int blue = 2;
unsigned long intervalBlue = 1000; // how long to delay in millis
unsigned long previousBlue = 0;
int blueState = LOW;

void fancyled()
{
    for (int i = 0; i < 30; i++)
    {
        analogWrite(blue, (i * 100) % 1001);
        delay(50);
    }
}

void led_indicator()
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousBlue >= intervalBlue)
    {
        // save this reading!
        previousBlue = currentMillis;

        // figure out if you should turn the LED on or off
        if (blueState == LOW)
        {
            blueState = HIGH;
        }
        else
        {
            blueState = LOW;
        }
        digitalWrite(blue, blueState);
    }
}