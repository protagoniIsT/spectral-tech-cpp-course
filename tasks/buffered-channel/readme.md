# Buffered channel

## Problem

In this task you should implement one of the standard components of golang language --- buffered channel. This channel is basically a
MPMC (multiple producer multiple consumer) channel of fixed size and supports following operations:
- `send(value)` to send `value` over the channel
- `recv()` to take the next value from the channel
- `close()` to close the channel, i.e. disallow any further send or recv calls

Since this channel allows multiple producers and multiple consumers write and read simultaneously (and maintains FIFO order), its a perfect synchronization tool, which
can be used to build multithreaded pipelines of any complexity. It can also serve as a basic block for some other primitives, e.g. in a thread pool.

The channel has a fixed capacity $n$ and is built around a channel of size $n$ in a way, that:
- `send(value)` blocks until there is a free space in the channel, and after that `value` is pushed at the end of the channel
- `recv()` blocks until there is any element in the channel, and after that recv extracts a value from the channel's head. Note, that each element can be extracted exactly once, but its not specified, which thread will do it.
- `close` closes the channel. All the elements stored in the channel before `close` call should be sent successfully. However, any `send` after `close`
should throw `std::runtime_error` and any `recv` after `close` should return `std::nullopt` for empty channel.

Implement this channel in `buffered_channel.h`.

## Testing

For tasks with multithreading there is an additional `TSAN` target built with thread sanitizer. Your program should pass tests with it as well.

Apart from traditional `test.cpp`, there is also a benchmark in `bench.cpp`. There are no time limits currently, but your implementation should finish successfully.
