c-tds-project-3 example using Co-operative - Time Driven System - Project for LPC1769

Example description
Required_SystemCoreClock => 100MHz
SysTick_RateHz => 1000 ticks per second (1mS)

Four tasks
task-gpio_switch_lpc1769
    Simple switch interface code, without/with software debounce.

task-gpio_led_lpc1769
    Simple led interface code.

task-heartbeat_lpc1769
    If everything is OK, flashes at 0.5 Hz
    If there is a fault code active, this is displayed.
    
task-watchdog_lpc1769
    Simple Watchdog Timer interface.


Special connection requirements
There are no special connection requirements for this example.

Build procedures:
Visit the LPCOpen quickstart guide at "http://www.lpcware.com/content/project/lpcopen-platform-nxp-lpc-microcontrollers/lpcopen-v200-quickstart-guides"
to get started building LPCOpen projects.
