# Med-Timer
Proof-of-concept (hereafter abbreviated POC) timer to help the ill remember to take their medicine

## Table of Contents
  * [Goals](#goals)
  * [Compiling and Uploading](#compiling-and-uploading)
  * [Using the Timer](#using-the-timer)
  * [Attaching Peripherals](#attaching-peripherals)
       * [Breadboarding](#breadboarding)
       * [Buzzer](#buzzer)
       * [Switch](#switch)
  * [Customization](#customization)

This repository has been tested on an Arduino Micro.

## Goals
Eventually, for simplicity sake, [the Arduino Timer library](https://github.com/JChristensen/Timer) should be implemented.

At some point down the road, this project will also include the Microchip PIC 10 or 12F family for various reasons, including cost.

## Compiling and Uploading
The easiest route for testing is to procure your favorite [Arduino](https://www.arduino.cc) dev board and download the latest Arduino IDE. The Mini, Micro, and Nano are recommended since they make it easy to plug everything into a breadboard.

Clone this repository by issuing `git clone https://github.com/LightningStalker/Med-Timer.git` from within your sketchbook folder or by downloading the ZIP. Either way, you should end up with a folder named `Med-Timer` inside your sketchbook folder. The sketchbook folder is a directory named `Arduino`, either in your home directory on Linux machines, or inside the `Documents` folder on Windows and Mac OS.

Now start up Arduino IDE and `Med-Timer` should appear in the `Sketchbook` in the `File` menu. If not, just go to `File -> Open` and locate the place where you cloned/unzipped it in the step above. Open up `Med-Timer.ino` or click on `Med-Timer` in the sketchbook. Customize the timers and alerts under where it says `morning dose`. This should be fairly obvious if you have experience with Arduino sketches. If not, [arduino.cc](https://www.arduino.cc) has everything you need to get started. See more about this further down in the `Customization` section.

The rest is obvious if you're familiar with Arduino. Plug in the Arduino USB and it should be automatically detected in Arduino IDE. Select your board under `Tools -> Board` and click the blue arrow ➡ button to upload or `Sketch -> Upload` and your timer should be ready.

## Using the Timer
In the POC, the Arduino is attached to a USB power source. This may be the computer it was programmed on, a USB power supply, or a USB battery power bank. The power bank provides the best option in case of a power outage. Eventually a more permanent battery solution should be devised.

In the morning, after taking the first dose, press the reset button on the Arduino. A short flash/beep confirms reset within 8 seconds and timing begins. After a preset interval (4 hours in the POC), the LED flashes and the buzzer sounds when it is time to take medicine. Now the other button (pin 12 in the POC) is pressed, stopping the alert and the dose is taken immediately. This repeats for the various other doses throughout the day.

## Attaching Peripherals
### Breadboarding
The Arduino Mini, Micro, and Nano all plug easily into a standard breadboard, and this is the recommended configuration for testing. Peripherals, including the buzzer, pushbutton switch, additional LEDs, and others as deemed necessary, simply plug into the breadboard. [A standard 400 hole breadboard](https://www.banggood.com/8_5-x-5_5cm-400-Tie-Points-400-Holes-Solderless-Breadboard-Bread-Board-p-91872.html) may be used for this purpose.

### Buzzer
Between pins D4 and GND there are 4 rows on the breadboard. A [standard 5V buzzer](http://www.soberton.com/wst-1205s/) fits this space perfectly. No jumper wire is necessary. Be sure to observe proper polarity, attaching the + side of the buzzer to D4. Damage to the Arduino and buzzer may occur if these connections are reversed.

If you are concerned with any possible EMF generated by the buzzer, a Schottky diode, ([credit EEVblog](https://www.eevblog.com/forum/beginners/piezo-buzzers-vs-magnetic-buzzers/)) such as the BAT54XV2 may be attached in reverse across the pins of the buzzer. A piezo buzzer eliminates this potential.

### Switch
A suitable normally open switch may be connected between pins D12 and GND. [Bourns](http://www.bourns.com/) manufactures several lines of excellent switches and is highly recommended. For testing the POC, a jumper wire was attached to GND and touched to pin D12 to send button presses. Plugging the wire into the D12 row suppresses all alerts.

## Customization
All input and output pins are interchangeable, of course, by modifying the appropriate lines of code. Pin D13 is used by the Arduino's onboard LED, however and use of other pins is recommended.

The time delay `hours()`, `minutes()`, and `seconds()` wrapper functions for the `delay()` function are provided for convenience. Use them to set intervals between doses.

Sometimes 2 or more different medicines are taken at differing intervals. The `error` variable has been provided for this purpose. The `alert()` function returns an `unsigned long` of how many seconds it has been running. In case the alert is not noticed for several minutes, this accumulating time value may be stored in `error` and subtracted from subsequent calls to time delay functions.

In the example POC, 2 medicines are taken in the morning at the same time before starting the timer. One is to be taken every 4 hours, and another to be taken once more again in 5 hours. If after running the 4 hour timer and calling `alert()`, that alert is then not noticed for 30 minutes, the 5 hour medicine will be delayed by that 30 minutes if a 1 hour timer is simply slated to run after the call to `alert()`. Thus, storing the runTime of `alert()` in `error` and subtracting it from the delay of the next timer ensures that the 5 hour medicine is indeed taken at the 5 hour mark.

[The Timer library](https://github.com/JChristensen/Timer) eliminates this problem and offers increased simplicity, possibly at the price of increased memory usage.
