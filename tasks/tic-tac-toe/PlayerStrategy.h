#pragma once

#include "strategy.h"
#include <algorithm>
#include <stdexcept>

static constexpr int NEUTRAL_CELL_VAL_ = -1;
static constexpr int BOARD_SIZE = 3;

static constexpr int X = 0;
static constexpr int O = 1;

class PlayerStrategy : public Strategy {
public:
    PlayerStrategy() = default;

    void Init(int player_num) override;

    bool IsWinning(int player) const;

    std::pair<int, int> BestMove();

    int Minimax(int depth, bool is_maximizing);

    bool IsBoardFull() const;

    int Evaluate() const;

    std::pair<int, int> MakeMove() override;

    void OpponentMove(const std::pair<int, int>& move) override;


    int player_num_;
    int board_[BOARD_SIZE][BOARD_SIZE]{};

private:
    void FillBoard(int value, size_t to_row = BOARD_SIZE, size_t to_column = BOARD_SIZE);

    void SetCell(size_t row, size_t col, int val);

    bool IsValidPlayerNum(int player_num);
};

