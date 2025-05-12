#include "PlayerStrategy.h"
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <iostream>


void PlayerStrategy::Init(int player_num) {
    if (!IsValidPlayerNum(player_num)) {
        throw std::runtime_error("Invalid player number");
    }
    this->player_num_ = player_num;
    FillBoard(NEUTRAL_CELL_VAL_);
}

bool PlayerStrategy::IsWinning(int player) const {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board_[i][0] == player && board_[i][1] == player && board_[i][2] == player)
            return true;
        if (board_[0][i] == player && board_[1][i] == player && board_[2][i] == player)
            return true;
    }
    if (board_[0][0] == player && board_[1][1] == player && board_[2][2] == player)
        return true;
    if (board_[0][2] == player && board_[1][1] == player && board_[2][0] == player)
        return true;

    return false;
}

bool PlayerStrategy::IsBoardFull() const {
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            if (board_[i][j] == NEUTRAL_CELL_VAL_)
                return false;
    return true;
}

int PlayerStrategy::Evaluate() const {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board_[i][0] == board_[i][1] && board_[i][1] == board_[i][2]) {
            if (board_[i][0] == player_num_) return 10;
            if (board_[i][0] == 1 - player_num_) return -10;
        }
        if (board_[0][i] == board_[1][i] && board_[1][i] == board_[2][i]) {
            if (board_[0][i] == player_num_) return 10;
            if (board_[0][i] == 1 - player_num_) return -10;
        }
    }

    if (board_[0][0] == board_[1][1] && board_[1][1] == board_[2][2]) {
        if (board_[0][0] == player_num_) return 10;
        if (board_[0][0] == 1 - player_num_) return -10;
    }
    if (board_[0][2] == board_[1][1] && board_[1][1] == board_[2][0]) {
        if (board_[0][2] == player_num_) return 10;
        if (board_[0][2] == 1 - player_num_) return -10;
    }

    return 0;
}

int PlayerStrategy::Minimax(int depth, bool is_maximizing) {
    int score = Evaluate();

    if (score == 10 || score == -10) return score - depth;

    if (IsBoardFull()) return 0;

    if (is_maximizing) {
        int best = -1000;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board_[i][j] == NEUTRAL_CELL_VAL_) {
                    board_[i][j] = player_num_;
                    best = std::max(best, Minimax(depth + 1, false));
                    board_[i][j] = NEUTRAL_CELL_VAL_;
                }
            }
        }
        return best;
    } else {
        int best = 1000;
        int opponent = 1 - player_num_;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board_[i][j] == NEUTRAL_CELL_VAL_) {
                    board_[i][j] = opponent;
                    best = std::min(best, Minimax(depth + 1, true));
                    board_[i][j] = NEUTRAL_CELL_VAL_;
                }
            }
        }
        return best;
    }
}

std::pair<int, int> PlayerStrategy::BestMove() {
    int best_val = -1000;
    std::pair<int, int> best_move = {0, 0};

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board_[i][j] == NEUTRAL_CELL_VAL_) {
                board_[i][j] = player_num_;
                int move_val = Minimax(0, false);
                board_[i][j] = NEUTRAL_CELL_VAL_;

                if (move_val > best_val) {
                    best_move = {i, j};
                    best_val = move_val;
                }
            }
        }
    }

    return best_move;
}

std::pair<int, int> PlayerStrategy::MakeMove() {
    auto move = BestMove();
    board_[move.first][move.second] = player_num_;
    return move;
}

void PlayerStrategy::OpponentMove(const std::pair<int, int>& move)  {
    int row = move.first;
    int col = move.second;

    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        throw std::runtime_error("Opponent move is out of board");
    }

    if (board_[row][col] != NEUTRAL_CELL_VAL_) {
        throw std::runtime_error("Opponent marks filled cell");
    }

    board_[row][col] = 1 - player_num_;
}

void PlayerStrategy::FillBoard(int value, size_t to_row, size_t to_column) {
    for (size_t i = 0; i < to_row; ++i) {
        for (size_t j = 0; j < to_column; ++j) {
            SetCell(i, j, value);
        }
    }
}

void PlayerStrategy::SetCell(size_t row, size_t col, int val) {
    board_[row][col] = val;
}

bool PlayerStrategy::IsValidPlayerNum(int player_num) {
    return player_num == O || player_num == X;
}
