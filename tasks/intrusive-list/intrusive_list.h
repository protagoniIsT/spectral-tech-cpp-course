#pragma once

#include <algorithm>

class ListHook {
public:
    ListHook() : prev_(nullptr), next_(nullptr) {};

    bool IsLinked() const {
        return (prev_ != nullptr && next_ != nullptr);
    }

    void Unlink() {
        if (!IsLinked()) return;
        if (prev_) {
            prev_->next_ = next_;
        }
        if (next_) {
            next_->prev_ = prev_;
        }
        prev_ = nullptr;
        next_ = nullptr;
    }

    ~ListHook() {
        Unlink();
    }

    ListHook(const ListHook&) = delete;

private:
    template<class T>
    friend class List;

    void LinkBefore(ListHook* other) {
        next_ = other;
        prev_ = other->prev_;
        if (other->prev_) {
            other->prev_->next_ = this;
        }
        other->prev_ = this;
    }

    ListHook* prev_;
    ListHook* next_;
};

template<typename T>
class List {
public:
    class Iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
    public:
        Iterator(ListHook* hook) : hook_(hook){};

        Iterator& operator++() {
            hook_ = hook_->next_;
            return *(this);
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        T& operator*() const {
            return *static_cast<T*>(hook_);
        }

        T* operator->() const {
            return static_cast<T *>(hook_);
        }

        bool operator==(const Iterator &rhs) const {
            return this->hook_ == rhs.hook_;
        }

        bool operator!=(const Iterator &rhs) const {
            return !(*this == rhs);
        }

    private:
        ListHook* hook_;
    };

    List() {
        dummy_.prev_ = &dummy_;
        dummy_.next_ = &dummy_;
    };

    List(const List&) = delete;

    List(List&& other) noexcept {
        dummy_.prev_ = &dummy_;
        dummy_.next_ = &dummy_;
        if (!other.IsEmpty()) {
            swap(other);
        }
    }

    ~List() {
        Clear();
    }

    List& operator=(const List&) = delete;

    List& operator=(List&& other) noexcept {
        if (this != &other) {
            Clear();
            swap(other);
        }
        return *this;
    }

    size_t Size() const {
        if (IsEmpty()) return 0;
        size_t size = 0;
        auto current = dummy_.next_;
        while (current != &dummy_) {
            ++size;
            current = current->next_;
        }
        return size;
    }

    bool IsEmpty() const {
        return dummy_.next_ == &dummy_;
    }

    void PushBack(T* elem) {
        elem->LinkBefore(&dummy_);
    }

    void PushFront(T* elem) {
        elem->LinkBefore(dummy_.next_);
    }

    T& Front() {
        if (IsEmpty()) {
            throw std::runtime_error("Cannot extract front element: list is empty.");
        }
        return *static_cast<T *>(dummy_.next_);
    }

    const T& Front() const {
        if (IsEmpty()) {
            throw std::runtime_error("Cannot extract front element: list is empty.");
        }
        return *static_cast<const T*>(dummy_.next_);
    }

    T& Back() {
        if (IsEmpty()) {
            throw std::runtime_error("Cannot extract last element: list is empty.");
        }
        return *static_cast<T*>(dummy_.prev_);
    }

    const T& Back() const {
        if (IsEmpty()) {
            throw std::runtime_error("Cannot extract last element: list is empty.");
        }
        return *static_cast<const T*>(dummy_.prev_);
    }

    void PopBack() {
        if (IsEmpty()) {
            throw std::runtime_error("List is empty.");
        }
        dummy_.prev_->Unlink();
    }

    void PopFront() {
        dummy_.next_->Unlink();
    }

    void Clear() {
        while (!IsEmpty()) {
            PopFront();
        }
    }

    Iterator Begin() {
        Iterator begin_iter(dummy_.next_);
        return begin_iter;
    }

    Iterator End() {
        Iterator end_iter(dummy_.prev_->next_);
        return end_iter;
    }

    Iterator IteratorTo(T* element) {
        Iterator elem_iter(element);
        return elem_iter;
    }

private:
    ListHook dummy_;

    void swap(List& other) {
        dummy_.prev_ = other.dummy_.prev_;
        dummy_.next_ = other.dummy_.next_;

        dummy_.next_->prev_ = &dummy_;
        dummy_.prev_->next_ = &dummy_;

        other.dummy_.next_ = &other.dummy_;
        other.dummy_.prev_ = &other.dummy_;
    }
};

template<typename T>
typename List<T>::Iterator begin(List<T>& list) { // NOLINT
    return list.Begin();
}

template<typename T>
typename List<T>::Iterator end(List<T>& list) { // NOLINT
    return list.End();
}
