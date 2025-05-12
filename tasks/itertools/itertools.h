#pragma once
#include <iterator>
#include <optional>
#include <ranges>
#include <utility>

template<class Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end) : begin_(begin), end_(end) {}

    Iterator begin() const {// NOLINT
        return begin_;
    }

    Iterator end() const {// NOLINT
        return end_;
    }

private:
    Iterator begin_, end_;
};


class Range {
public:
    class Iterator;
    using value_type = size_t;
    using iterator = Iterator;
    using const_iterator = Iterator;

    class Iterator {
        using iterator_category = std::forward_iterator_tag;

    public:
        Iterator(value_type it, value_type step) : current_(it), step_(step) {}

        Iterator &operator++() {
            current_ += step_;
            return *(this);
        }

        bool operator==(const Iterator &other) const {
            return current_ == other.current_;
        }

        bool operator!=(const Iterator &other) const {
            return current_ < other.current_;
        }

        value_type operator*() const {
            return current_;
        }

    private:
        value_type current_, step_;
    };

    Range(value_type from, value_type to, value_type step) : from_(from), to_(to), step_(step) {}

    explicit Range(value_type to) : Range(0, to, 1) {}

    Range(value_type from, value_type to) : Range(from, to, 1) {}

    Iterator begin() const { // NOLINT
        return {from_, step_};
    }

    Iterator end() const { // NOLINT
        return {to_, step_};
    };

private:
    value_type from_, to_, step_;
};


template<typename Container>
class Group {
public:
    class Iterator;
    using iterator = Iterator;
    using const_iterator = Iterator;
    using Iter = typename Container::const_iterator;

    class Iterator {
    public:
        using group_iterator = typename Container::const_iterator;
        using value_type = IteratorRange<group_iterator>;

        Iterator(group_iterator current, group_iterator end)
            : group_begin_(current), current_(current), end_(end) {
            if (current_ != end_) {
                group_value_.emplace(*current_);
                while (current_ != end_ && *current_ == group_value_.value()) {
                    ++current_;
                }
            }
        }

        Iterator &operator++() {
            group_begin_ = current_;
            if (current_ != end_) {
                group_value_.emplace(*current_);
                while (current_ != end_ && *current_ == group_value_.value()) {
                    ++current_;
                }
            }
            return *this;
        }

        bool operator!=(const Iterator &other) const {
            return group_begin_ != other.group_begin_;
        }

        value_type operator*() const {
            return {group_begin_, current_};
        }

    private:
        group_iterator group_begin_;
        group_iterator current_;
        group_iterator end_;
        std::optional<typename std::iterator_traits<group_iterator>::value_type> group_value_;
    };

    explicit Group(const Container &container)
        : begin_(std::begin(container)), end_(std::end(container)) {}

    Iterator begin() const {
        return {begin_, end_};
    }
    Iterator end() const {
        return {end_, end_};
    }

private:
    Iter begin_, end_;
};


template<typename Iterable1, typename Iterable2>
class Zip {
public:
    class Iterator;
    using iterator = Iterator;
    using const_iterator = Iterator;
    using Iter1 = typename Iterable1::const_iterator;
    using Iter2 = typename Iterable2::const_iterator;
    using value_type = std::pair<typename Iterable1::value_type, typename Iterable2::value_type>;

    class Iterator {
    public:
        Iterator(Iter1 it1, Iter1 end1, Iter2 it2, Iter2 end2)
            : current1_(it1), end1_(end1), current2_(it2), end2_(end2) {}

        Iterator &operator++() {
            ++current1_;
            ++current2_;
            return *this;
        }

        bool operator!=(const Iterator &other) const {
            return current1_ != other.current1_ && current2_ != other.current2_;
        }

        value_type operator*() const {
            return {*current1_, *current2_};
        }

    private:
        Iter1 current1_, end1_;
        Iter2 current2_, end2_;
    };

    Zip(const Iterable1 &seq1, const Iterable2 &seq2)
        : begin1(seq1.begin()), end1(seq1.end()),
          begin2(seq2.begin()), end2(seq2.end()) {}

    Iterator begin() const {
        return Iterator(begin1, end1, begin2, end2);
    }

    Iterator end() const {
        return Iterator(end1, end1, end2, end2);
    }

private:
    Iter1 begin1, end1;
    Iter2 begin2, end2;
};
