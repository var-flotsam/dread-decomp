#pragma once

// UnkTU2727: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2727,
// 0x71011902d4-0x7101192628). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

namespace base::global {
class CRntString;
}

struct UnkNamedObject;
struct UnkNamedUser;
// The user registered for `name` with the named object.
UnkNamedUser* fn_7101190890(UnkNamedObject* self, base::global::CRntString name);
