#include "PlayerStrategy.h"

extern "C" Strategy* CreateStrategy() {
    return new PlayerStrategy();
}