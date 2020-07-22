# Microtrace

```
  /\/\ (_) ___ _ __ ___/__   \_ __ __ _  ___ ___ 
 /    \| |/ __| '__/ _ \ / /\/ '__/ _` |/ __/ _ \
/ /\/\ \ | (__| | | (_) / /  | | | (_| | (_|  __/
\/    \/_|\___|_|  \___/\/   |_|  \__,_|\___\___|

```

Tracing tool for microcontrollers. Inspired from [minitrace](https://github.com/hrydgard/minitrace). 

Microtrace is a header-only library targeting microcontrollers for producing JSON traces suitable for Chrome's built-in trace viewer (chrome://tracing).

> Note: Is in very early developpement and currently has only been tested on STM32 microcontrollers

## Screenshots

Comming soon

## Prerequisites

You need to have `printf()` printing somewhere in order to dump the JSON file. You also need some kind of function to measure the number of microseconds elapsed since start of program. See Configuration for more details.

## How to use 

Copy paste microtrace.h into your project, include the file and use it like this:

```c
#include "microtrace.h"

void main(void) {

  /* ... */

  MICROTRACE_BEGIN("Perif", "Camera Init", 1);
  BSP_Camera_Init();
  MICROTRACE_END("Perif", "Camera Init", 1);

  MICROTRACE_BEGIN("Main", "Main loop", 1);

  for (int i = 0; i < 10; i++) {
    MICROTRACE_BEGIN("Image proc", "Equalize histogram", 1)
    /* Processing here */
    MICROTRACE_END("Image proc", "Equalize histogram", 1)
  }
  MICROTRACE_END("Main", "Main loop", 1);

  MICROTRACE_INSTANT("Main", "Instantaneous event", 1);

  MICROTRACE_BEGIN("Parallel", "DMA task", 2);
  /* Parallel task here */
  MICROTRACE_END("Parallel", "DMA task", 2);

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

Please refer to [microtrace.h](https://github.com/thibthibaut/microtrace/blob/b540a3ef5253aba0d767a99c02b47e2e1fbd1f3b/microtrace.h#L34)

## License 

This work is under MIT License. See License file for more info.
