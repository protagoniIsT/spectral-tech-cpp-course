#include "../strategy.h"
#include "../PlayerStrategy.h"
#include <dlfcn.h>
#include <iostream>
#include <memory>

static constexpr int DRAW = 2;
// static constexpr int NEUTRAL_CELL_VAL_ = -1;
// static constexpr int BOARD_SIZE = 3;

// static constexpr int X = 0;
// static constexpr int O = 1;

int check_win(const int board[3][3]) {
    for (size_t i = 0; i < 3; ++i) {
        if (board[i][0] != NEUTRAL_CELL_VAL_
            && board[i][0] == board[i][1]
            && board[i][1] == board[i][2]) {
            return board[i][0];
        }
    }

    for (size_t i = 0; i < 3; ++i) {
        if (board[0][i] != NEUTRAL_CELL_VAL_
            && board[0][i] == board[1][i]
            && board[1][i] == board[2][i]) {
            return board[0][i];
        }
    }

    if (board[0][0] != NEUTRAL_CELL_VAL_
        && board[0][0] == board[1][1]
        && board[1][1] == board[2][2]) {
        return board[0][0];
    }

    if (board[0][2] != NEUTRAL_CELL_VAL_ &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0]) {
        return board[0][2];
    }
        
    return NEUTRAL_CELL_VAL_;
}

bool is_board_full(const int board[3][3]) {
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            if (board[i][j] == NEUTRAL_CELL_VAL_) return false;
        }
    }
    return true;
}   

int main(int argc, char* argv[]) {
    const char* lib_x = argv[1];
    const char* lib_o = argv[2];

    void* handleX = dlopen(lib_x, RTLD_LAZY);
    if (!handleX) {
        std::cerr << "Could not open " << lib_x << ": " << dlerror() << "\n";
        return -1;
    }

    void* handleO = dlopen(lib_o, RTLD_LAZY);
    if (!handleO) {
        std::cerr << "Could not open " << lib_o << ": " << dlerror() << "\n";
        dlclose(handleX);
        return -1;
    }

    using CreateStrategyFunc = Strategy* (*)();

    dlerror();

    CreateStrategyFunc createX = (CreateStrategyFunc) dlsym(handleX, "CreateStrategy");
    CreateStrategyFunc createO = (CreateStrategyFunc) dlsym(handleO, "CreateStrategy");

    const char* err = dlerror();
    if (!createX || !createO || err) {
        std::cerr << "Could not find CreateStrategy: " << (err ? err : "unknown error") << "\n";
        dlclose(handleX);
        dlclose(handleO);
        return -1;
    }

    Strategy* strategyX = createX();
    Strategy* strategyO = createO();

    strategyX->Init(0);
    strategyO->Init(1);

    int board[BOARD_SIZE][BOARD_SIZE]{};

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j] = NEUTRAL_CELL_VAL_;
        }
    }

    Strategy* players[2] = { strategyX, strategyO };
    int curr_player = X;

    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) {
        Strategy* curr = players[curr_player];
        std::pair<int, int> move;

        try {
            move = curr->MakeMove();
        } catch (...) {
            return -1;
        }

        if (move.first < 0 || move.first >= BOARD_SIZE ||
            move.second < 0 || move.second >= BOARD_SIZE ||
            board[move.first][move.second] != NEUTRAL_CELL_VAL_) {
            return -1;
        }

        board[move.first][move.second] = curr_player;

        try {
            players[1 - curr_player]->OpponentMove(move);
        } catch (...) {
            return -1;
        }

        int winner = check_win(board);
        if (winner != NEUTRAL_CELL_VAL_) {
            return winner;
        }

        curr_player = 1 - curr_player;
    }

    dlclose(handleX);
    dlclose(handleO);

    return DRAW;
}
