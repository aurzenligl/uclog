Printf API logging framework for resource constrained environments.

Inspiration
------------------
Logger idea is based on:
- python logging module
  https://docs.python.org/2/library/logging.html
- linux kernel vsprintf module
  http://lxr.free-electrons.com/source/lib/vsprintf.c

Logger concepts
------------------
- storage

  Implements operation of storing log. Log may be composed of: timestamp, level, logger name or id,
  text or format string whereabouts and serialized printf arguments (see binary printf section).
  Storage may be a ring-buffer keeping only newest logs or a ping-pong buffer flushed on a regular basis.

- handler

  Handler contains reference to storage and level which may filter logs before they get stored.
  Level may be changed at runtime to control logging output.

- logger

  Front-end of logging framework, contains list of handlers, level and optional parent logger.
  If level is not set, it's taken from first parent who has it set.
  If handlers list is empty, it's taken from first parent who has any.
  Level may be changed at runtime to control logging output.
  It's probably good to keep logger objects (or at references to them) globally accessible
  to avoid having to pass them in function signatures and store in application objects.

Usage proposal
------------------

Logging framework should work more or less along these lines.
During startup application:
- establishes storages,
- wraps them into handlers, which also get levels,
- initializes loggers with identification data and initial levels,
- associates handlers with loggers.
This way user has fine-grained control over logging
by setting handler and logger levels in runtime.

Binary printf
------------------
Binary printf should be used by applications which may log extensively
and have limited resources: memory size, stack size, cpu time,
since it consumes less of each compared to sprintf.

Binary printf stores only arguments of printf call in binary buffer.
User can form a text string from format string and those any time later,
if at all.

Additional care needs to be taken to decode binary logs, but logging
interface is the same as for sprintf storages,
so binary and textual printfs can be used interchangeably.

How to store format strings with binary arguments:
- store fmt verbatim
- store fmt pointer
  - get pointer-string pairs from running application or from linked binary
- store fmt hash
  - get hash-string pairs from application source code
  - #define LOG(LEVEL, FMT, ...) { static const hasher hasher_(FMT); log.log(LEVEL, FMT, ##\__VA_ARGS__, hasher_.hash); }
  - modify vsnbprintf function so that it returns the one-after-the-last argument: hash
- store fmt id
  - make sure that each log is assigned unique id
  - get id-string pairs from application source code
  - #define LOG(ID, LEVEL, FMT, ...) log.log(LEVEL, FMT, ##\__VA_ARGS__, ID)
  - modify vsnbprintf function so that it returns the one-after-the-last argument: id
