#pragma once

#include "game/UnkIdVector.h"

struct UnkNamedIds {
    UnkStrIdTwoStep name;
    UnkIdVector ids;

    __attribute__((always_inline)) ~UnkNamedIds() {}
};
