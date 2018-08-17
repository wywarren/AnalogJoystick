# Analog Joystick with Calibration

Latest Version: v1.0

This is an example of setting up an analog joystick to interface with arduino and to calibrate the input values between -1 and 1 with the bottom left of the joystick being the origin. After calibration, the values are saved to EEPROM so that the next time the  controller is turned on, the values are retained.

Keep in mind even at rest position, the raw input value is noisy. When using, make sure vector magnitude is beyond an arbitrary threshold before firing events. Most raw input value deviations are ~=1 (on a scale of 0-1023).

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

You will need:

```
- Arduino MEGA/UNO/Nano/Mini Pro
- Analog Joystick with x,y,switch inputs
```

### Installing

Build the sketch to your Arduino of choice.

## Deployment

In this example, the switch press is the toggle for calibration. Remember to remap the calibration mode to your own use case so that you can reuse the switch for other purposes.

## Versioning

v1.0 Original

## Authors

* **Warren Wang** - *Initial work* - [AnalogJoystick](https://github.com/wywarren/AnalogJoystick)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* TBD