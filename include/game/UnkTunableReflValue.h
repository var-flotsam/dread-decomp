#pragma once

// A reflected value that is released when it goes out of scope (fn_71000e76dc's result, read by
// fn_710013ab54). Names are placeholders.
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"

struct UnkTunableReflValue : base::reflection::UnkReflValue {
    UnkTunableReflValue() {}
    UnkTunableReflValue(const base::reflection::UnkReflValue& value) : UnkReflValue(value) {}
    ~UnkTunableReflValue() { fn_71000a1c60(this); }
};
