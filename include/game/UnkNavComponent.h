#pragma once

#include "types.h"

struct UnkNavComponentVtbl;

struct UnkNavComponent {
    UnkNavComponentVtbl* vtable;
};
