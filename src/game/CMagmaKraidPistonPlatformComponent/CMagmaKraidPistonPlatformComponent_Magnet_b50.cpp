// CMagmaKraidPistonPlatformComponent unit (tools/tu_map.py, the name from reflection): finds the
// "MAGNET_SURFACE" child of the object at +0x30 and tells whether its +0x30 field is set.
// Names are placeholders.
#include "base/reflection/UnkReflStatics.h"
#include "types.h"

struct UnkRefl_fn_71005b6320;
extern template base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_71005b6320>(void*);

// The signature src/game/CMagmaKraidScenarioControllerComponent/CMagmaKraidScenarioControllerComponent_b47.cpp
// calls it with.
struct UnkMagmaKraid_b47;

bool fn_7100918ef4(UnkMagmaKraid_b47* kraid) {
    char* self = reinterpret_cast<char*>(kraid);
    char* o = reinterpret_cast<char*>(
        base::reflection::refl_FindTyped<UnkRefl_fn_71005b6320>(*reinterpret_cast<void**>(self + 0x30)));
    return o && *reinterpret_cast<void**>(o + 0x30) != nullptr;
}
