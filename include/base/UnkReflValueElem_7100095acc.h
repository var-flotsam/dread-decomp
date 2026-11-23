#pragma once

#include "base/reflection.h"
#include "types.h"

struct UnkReflValueElem_7100095acc {
    base::reflection::UnkReflValue value;

    UnkReflValueElem_7100095acc(UnkReflValueElem_7100095acc&& other) { fn_71000a179c(&value, &other.value); }
    ~UnkReflValueElem_7100095acc() { fn_71000a1c60(&value); }
};
