# Read-write spinlock

Implement rw-lock primitive from the lecture using atomic and busy waiting (i.e. like a spinlock). The interface is given in `rw_spinlock.h`.
Note, that rw-lock implementation requires 2 counters: for readers and writers. However, dealing with 2 separate counters atomically is much harder, so
it makes sense to put both in the same `atomic<int>`. Use the lowest bit for writers count (since there can't be more then 1 simultaneously) and the rest for readers.

You'll have to use `compare_exchange` to update this atomic counter correctly.
