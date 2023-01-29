#include <Ramp.h>
#include <L293D.h>

struct Motor
{
    // Motor library
    // https://github.com/bjoernboeckle/L293D
    // Motor tutorial
    // https://lastminuteengineers.com/l293d-dc-motor-arduino-tutorial/

    // TODO: What pins go where?
    int enablePin = 4;
    int in1Pin = 4;
    int in2Pin = 4;
    L293D motor = L293D(9, 8, 7);

    int minSpeed = 0;
    int maxSpeed = 255;
    int rampDuration = 1000;
    ramp speedController;

    void setup()
    {
        motor.begin();
    }

    int getSpeed()
    {
        return speedController.getValue();
    }

    int getTargetSpeed()
    {
        return speedController.getTarget();
    }

    void setTargetSpeed(int speed)
    {
        int targetSpeed = constrain(speed, minSpeed, maxSpeed);
        speedController.go(targetSpeed, rampDuration);
    }

    void update()
    {
        int speed = speedController.update();
        motor.SetMotorSpeed(speed);
    }
};
