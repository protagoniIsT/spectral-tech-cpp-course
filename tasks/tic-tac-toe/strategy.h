#pragma once

#include <utility>

class Strategy {
public:
    virtual void Init(int player_num) = 0;
    virtual std::pair<int, int> MakeMove() = 0;
    virtual void OpponentMove(const std::pair<int, int>& move) = 0;
    virtual ~Strategy() = default;
};
