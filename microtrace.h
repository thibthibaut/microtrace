/*
 *
 * This file is part of the microtrace project.
 * Copyright (c) 2020 Thibaut Vercueil
 * This work is licensed under the MIT license, see the file LICENSE for
 * details.
 *
 * =====================================================================================
 *           dGGGGMMb     ,"""""""""""""".
 *          @p~qp~~qMb    | MicroTrace ! |
 *          M|@||@) M|   _;..............'
 *          @,----.JM| -'
 *         JS^\__/  qKL
 *        dZP        qKRb
 *       dZP          qKKb
 *      fZP            SMMb
 *      HZM            MMMM
 *      FqM            MMMM
 *    __| ".        |\dS"qML
 *    |    `.       | `' \Zq
 *   _)      \.___.,|     .'
 *   \____   )MMMMMM|   .'
 *     `-'       `--'
 *
 *	Ultra-light library for tracing on microcontrollers
 * =====================================================================================
 */
#ifndef _MICROTRACE_H_
#define _MICROTRACE_H_
#include <stdint.h>
#include <stdio.h>

/* =============================== */
/* ====== Configuration ========== */
/* =============================== */

/* Uncomment the following to disable Microtrace */
/* #define MICROTRACE_DISABLE */

/* Maximum number of events to be recorded. Microtrace will stop after max
 * events is reached and start to jump the JSON report. A big number of event
 * can take a lot of space in RAM */
#define MICROTRACE_MAX_EVENTS (5000)

/* Maximum number of characters for the `Category` or `Name` string
 * if an string lenght exceeded this number the compilation will fail
 * Note that reducing this value improves memory footprint */
#define MICROTRACE_MAX_STRING_LEN (32)

/* Uncomment the followint if you want to place
 * Microtrace buffers in specific memory sections
 * (see MICROTRACE_BUFFER_SECTION) */
/* #define MICROTRACE_CUSTOM_MEMORY */

/* Memory section to place microtrace buffer.
 * (it can be usefull to use external memory if avaible. e.g ".ext_sdram"
 * Warning MICROTRACE_CUSTOM_MEMORY should be defined */
#define MICROTRACE_BUFFER_SECTION ""

/* Function used to get number of microseconds elapsed since begining of
 * program. e.g HAL_GetTick */
#define MICROTRACE_COUNT_FUNCTION HAL_GetTick

/* If MICROTRACE_COUNT_FUNCTION doesn't return a number of microseconds, a
 * multiplier can be applied to convert to microseconds. e.g HAL_GetTick()
 * returns miliseconds, hence x1000 */
#define MICROTRACE_MULTIPLIER (1000)

/* =============================== */

typedef struct {
  char category[MICROTRACE_MAX_STRING_LEN];
  char name[MICROTRACE_MAX_STRING_LEN];
  uint32_t timestamp;
  char phase;
  uint32_t tid; /* Thread ID */ 
} microtrace_event_t;

extern microtrace_event_t microtrace_event_buffer[MICROTRACE_MAX_EVENTS];
extern uint32_t microtrace_event_counter;
extern char microtrace_print_buffer[512];

#define MICROTRACE_STATIC_ASSERT(COND, MSG)                                    \
  typedef char MICROTRACE_STATIC_ASSERTION_FAILED_##MSG[(COND) ? 1 : -1]

#ifndef MICROTRACE_BUFFER_HAS_BEEN_ALLOCATED
#define MICROTRACE_BUFFER_HAS_BEEN_ALLOCATED
#ifdef MICROTRACE_CUSTOM_MEMORY
__attribute__((section(MICROTRACE_BUFFER_SECTION)))
#endif
microtrace_event_t microtrace_event_buffer[MICROTRACE_MAX_EVENTS];
uint32_t microtrace_event_counter;
#ifdef MICROTRACE_CUSTOM_MEMORY
__attribute__((section(MICROTRACE_BUFFER_SECTION)))
#endif
char microtrace_print_buffer[512];
#endif

#ifndef MICROTRACE_DISABLE
#define _MICROTRACE_ADD_EVENT(_phase, _category, _name, _tid)                  \
  do {                                                                         \
    MICROTRACE_STATIC_ASSERT(sizeof(_category) < MICROTRACE_MAX_STRING_LEN,    \
                             CATEGORY_STRING_LENGHT_TOO_BIG);                  \
    MICROTRACE_STATIC_ASSERT(sizeof(_name) < MICROTRACE_MAX_STRING_LEN,        \
                             NAME_STRING_LENGHT_LENGHT_TOO_BIG);               \
    if (microtrace_event_counter >= MICROTRACE_MAX_EVENTS) {                   \
      MICROTRACE_DUMP_JSON();                                                  \
      while (1) {                                                              \
      };                                                                       \
    }                                                                          \
    uint32_t timestamp = MICROTRACE_COUNT_FUNCTION();                          \
    microtrace_event_t *event =                                                \
        &microtrace_event_buffer[microtrace_event_counter];                    \
    microtrace_event_counter++;                                                \
    strcpy(event->category, _category);                                        \
    strcpy(event->name, _name);                                                \
    event->timestamp = timestamp;                                              \
    event->phase = _phase;                                                     \
    event->tid = _tid;                                                         \
  } while (0);
#else
#define _MICROTRACE_ADD_EVENT(_phase, _category, _name, _tid)                  \
  do {                                                                         \
  } while (0)
#endif

#ifndef MICROTRACE_DISABLE
#define MICROTRACE_DUMP_JSON()                                                 \
  do {                                                                         \
    printf("{\"traceEvents\":[\r\n");                                          \
    for (uint32_t i = 0; i < microtrace_event_counter - 1; i++) {              \
      sprintf(microtrace_print_buffer,                                         \
              "{\"cat\":\"%s\",\"pid\":1,\"tid\":%ld,\"ts\":%ld,"              \
              "\"ph\":\"%c\",\"name\":\"%s\",\"args\":{}},\r\n",               \
              microtrace_event_buffer[i].category,                             \
              microtrace_event_buffer[i].tid,                                  \
              microtrace_event_buffer[i].timestamp *MICROTRACE_MULTIPLIER,     \
              microtrace_event_buffer[i].phase,                                \
              microtrace_event_buffer[i].name);                                \
      printf(microtrace_print_buffer);                                         \
    }                                                                          \
    sprintf(microtrace_print_buffer,                                           \
            "{\"cat\":\"%s\",\"pid\":1,\"tid\":%ld,\"ts\":%ld,"                \
            "\"ph\":\"%c\",\"name\":\"%s\",\"args\":{}}]}\r\n",                \
            microtrace_event_buffer[microtrace_event_counter - 1].category,    \
            microtrace_event_buffer[microtrace_event_counter - 1].tid,         \
            microtrace_event_buffer[microtrace_event_counter - 1]              \
                .timestamp *MICROTRACE_MULTIPLIER,                             \
            microtrace_event_buffer[microtrace_event_counter - 1].phase,       \
            microtrace_event_buffer[microtrace_event_counter - 1].name);       \
    printf(microtrace_print_buffer);                                           \
  } while (0)
#else
#define MICROTRACE_DUMP_JSON()                                                 \
  do {                                                                         \
  } while (0)
#endif

#define MICROTRACE_BEGIN(cat, name, tid) _MICROTRACE_ADD_EVENT('B', cat, name, tid)
#define MICROTRACE_END(cat, name, tid) _MICROTRACE_ADD_EVENT('E', cat, name, tid)
#define MICROTRACE_INSTANT(cat, name, tid) _MICROTRACE_ADD_EVENT('I', cat, name, tid)

#endif // _MICROTRACE_H_
