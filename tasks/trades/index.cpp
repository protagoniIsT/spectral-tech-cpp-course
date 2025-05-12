#include "index.h"

TradeIndex::TradeIndex(std::vector<Trade> trades) : trades_(std::move(trades)) {
    std::stable_sort(trades_.begin(), trades_.end(), [](const Trade& t1, const Trade& t2) {
        return t1.timestamp < t2.timestamp;
    });

    pref_sum_.assign(trades_.size() + 1, 0);
    for (size_t i = 0; i < trades_.size(); ++i) {
        pref_sum_[i + 1] = pref_sum_[i] + trades_[i].price * trades_[i].volume;
    }
}

int64_t TradeIndex::TotalVolume(int64_t from, int64_t to) {
    if (from > to) {
        throw std::out_of_range("'from' should be less than or equal to 'to'");
    }

    auto lower = std::lower_bound(trades_.begin(), trades_.end(), from,
                                  [](const Trade &t, int64_t value) {
                                      return t.timestamp < value;
                                  });

    auto upper = std::lower_bound(trades_.begin(), trades_.end(), to,
                                  [](const Trade &t, int64_t value) {
                                      return t.timestamp < value;
                                  });

    size_t idx_lower = lower - trades_.begin();
    size_t idx_upper = upper - trades_.begin();

    return pref_sum_[idx_upper] - pref_sum_[idx_lower];
}
