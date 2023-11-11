# DoorLockerSystem_AVR32
#### The project is built on a layered architectural approach with two microcontrollers, "Atmega32," talking over UART.



![](https://github.com/OmarMahmoudSalah/DoorLockerSystem_AVR32/blob/main/Screens/Door.jpg?raw=true)

------------

**HMI_ECU (Human Machine Interface): This component was responsible for user interaction.**
- ·        HAL Layer (Keypad- LCD)
- ·        MCAL Layer (GPIO-TIMER1-UART)

![](https://github.com/OmarMahmoudSalah/DoorLockerSystem_AVR32/blob/main/Screens/HMI.jpg?raw=true)


------------

**CONTROL_ECU :  is responsible for all processing and decisions in the system, such as password checking, opening the door, and activating the system alarm. It is considered to be the brain of the system.**

·        HAL Layer (BUZZER, DCMOTOR, -EEPROM)
·        MCAl Layer (GPIO-TIMER-UART-TWI-PWM)

![](https://github.com/OmarMahmoudSalah/DoorLockerSystem_AVR32/blob/main/Screens/Control.jpg?raw=true)

------------

### System Requirements

- Create a system password by entering a password consisting of 5 numbers. The LCD will display "*" on the screen for each number entered.

- if the two passwords match, the system now has a password and it is saved inside the EEPROM.

- The LCD will always display the main system options, which are to open the door or change the password.

- If the password is matched, the door is opened using the motor function.

- If the password is unmatched, the buzzer will sound for 1 minute, displaying a warning message.


