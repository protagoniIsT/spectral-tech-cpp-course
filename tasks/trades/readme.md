# Trades

## Problem

In this task you should implement a small library for reading and indexing a binary file with trades. You can find the definition of trade
structure in `trade.h`. The binary files you'll work with consist of serialized trades, written in little-endian consecutively **without padding**.

### Reader

Implement class `TradeReader`, which receives binary stream `std::istream&` in the constructor. Method `ReadNext` should read next trade (if any) and return
`std::optional<Trade>`. Method `ReadAll` should return `vector<Trade>`, basically reading the entire file. You can find some examples in tests.

### Index

Implement class `TradeIndex`, which receives `std::vector<Trade>` in the constructor and provides method
```c++
int64_t TotalVolume(int64_t from, int64_t to);
```

This method calculates sum of `price * volume` for all trades with `from <= timestamp < to`. This method should have at least `O(log(len(trades)))`
complexity.

## Implementation

You can organize your code as you wish, however you should put your build into `trades.cmake` file (don't modify CMakeLists.txt). Your cmake
should provide 3 targets:
- object trades_reader
- object trades_index
- static library trades with these 2 objects

Check `CMakeLists.txt` for further clarifications.

Note, that building `test_trades_reader` shouldn't trigger build of `trades_index` and `trades`.
