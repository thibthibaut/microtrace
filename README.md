# Microtrace

```
  /\/\ (_) ___ _ __ ___/__   \_ __ __ _  ___ ___ 
 /    \| |/ __| '__/ _ \ / /\/ '__/ _` |/ __/ _ \
/ /\/\ \ | (__| | | (_) / /  | | | (_| | (_|  __/
\/    \/_|\___|_|  \___/\/   |_|  \__,_|\___\___|

```

Tracing tool for microcontrollers. Inspired from [minitrace](https://github.com/hrydgard/minitrace). 

Microtrace is a header-only library targeting microcontrollers for producing JSON traces suitable for Chrome's built-in trace viewer (chrome://tracing).

> Note: Is in very early developpement and currently has only been tested on STM32 microcontrolers

## Screenshots

Comming soon

## Prerequisites




## How to use 

Copy paste microtrace.h into your project, include the file and use it like this:

```c
#include "microtrace.h"

void main(void) {

  MICROTRACE_INIT();

  /* ... */

  MICROTRACE_BEGIN("Perif", "Camera Init");
  BSP_Camera_Init();
  MICROTRACE_END("Perif", "Camera Init");

  MICROTRACE_BEGIN("Main", "Main loop");

  for (int i = 0; i < 10; i++) {
    MICROTRACE_BEGIN("Image proc", "Equalize histogram")
    /* Processing here */
    MICROTRACE_END("Image proc", "Equalize histogram")
  }
  MICROTRACE_END("Main", "Main loop");

  MICROTRACE_INSTANT("Main", "Instantaneous event");

  MICROTRACE_DUMP_JSON();
}

```

`MICROTRACE_DUMP_JSON()` will dump a json file through `printf()`, if you are using a UART, you can do 

```sh
cat /dev/ttyACMX > trace.json
```

We will then be able to open the trace with Chrome tracing tool by openning chrome://tracing and clicking `Load button`.

Please refer to Chrome documentation on how to use this tool.

> Note: If the maximum number of events is reached (see Configuration). `MICROTRACE_DUMP_JSON()` will be automatically called and the program will go in infinite loop.

## Configuration 

Please refer to microtrace.h 

## Licence 

This work is under MIT Licence. See Licence file for more info
