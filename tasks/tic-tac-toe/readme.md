# Tic Tac Toe

**This is a separate cmake project, so create a separate build in this directory.**

## Problem

### Strategy

In this task you should make a dynamic library, which contains implementation of a strategy for tic-tac-toe game. And also a small utility, which
allows to run such 2 strategies against each other.

The base class for the strategy is given in `strategy.h`:
- `player_num` is 0 for the first player (for `X`) and 1 for `O`
- `MakeMove` should return the next move your strategy makes. The numeration goes like this

| <!-- --> | <!-- --> | <!-- --> |
|----------|----------|----------|
| (0, 0)   | (0, 1)   | (0, 2)   |
| (1, 0)   | (1, 1)   | (1, 2)   |
| (2, 0)   | (2, 1)   | (2, 2)   |

Its guaranteed, that you'll still be able to make a move.
- `OpponentMove` tells the move opponent made.

Pack your class with a strategy into a dynamic library. Additionally implement function `Strategy* CreateStrategy()`, which creates
a new instance of your class and returns a pointer to it. 

### Runner

Apart from a strategy, implement an utility, which takes paths to 2 dynamic libraries with strategies and runs them against each other. The first
strategy given plays `X` and the second plays `O`.

Your runner should exit with code:
- 0 if `X` wins
- 1 if `O` wins
- 2 in case of draw
- -1 in case of invalid move by any strategy

Use `dlopen` and `dlsym` to load strategies.

## Testing

There are 3 already implemented strategies in the repository (in test folder):
- libopt.so plays perfectly. That means your strategy should always play draw with it.
- liblose.so always tries to lose. So your strategy should always win against it.
- libbad.so makes random (including invalid) moves. Your strategy should win here whenever its possible.

Your strategy and runner are also tested against author's runner (also provided in test folder) and strategy in the testing system.

Note, that your runner should use plain symbol name (i.e. `CreateStrategy`) during loading. So you should also force C-style mangling for this
function.

### Submit

Write your own CMakeLists.txt for this task. You want to include `include(../cmake/BuildFlags.cmake)` to load some settings and ASAN build type.
Your build should provide 2 targets:
- `runner` executable
- `opt` shared library (which produces libopt.so)
