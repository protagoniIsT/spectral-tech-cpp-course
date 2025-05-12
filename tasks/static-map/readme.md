# Static Map

## Problem

Implement template class `StaticMap<K, V>`, which is an associative container mapping `K -> V` and which stores it's content in a vector of pairs and uses a binary search for lookups.
Thus this map doesn't support update operations.

`Find` should return a pointer to the value associated with a given key or `nullptr` if there is no such value.

The second constructor expects `Iterator` to address `std::pair<K, V>`.

Use `operator<` of `K` for comparing elements only.

## Notes

- Check out this week's lecture if you don't understand why the constructor receives vector of pairs by value.
- You can use https://en.cppreference.com/w/cpp/algorithm/lower_bound instead of writing your own binary search.
- `map` and `unordered_map` are **forbidden** in this task.
- This map might work faster, than the previous two due to it's good data locality.
