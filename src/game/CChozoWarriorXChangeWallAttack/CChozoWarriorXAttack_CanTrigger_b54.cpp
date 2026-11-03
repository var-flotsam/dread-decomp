// CChozoWarriorX attacks (ChangeWall fn_710048098c, Land fn_7100482a44, Spit fn_7100484a68): one
// identical "can start" test per class. Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

bool fn_7100493b00(void* self);

namespace {

inline bool UnkChozoWarriorXAttack_CanStart(char* self) {
    if (!fn_7100493b00(self))
        return false;
    char* owner = *reinterpret_cast<char**>(self + 0x8);
    char* a = *reinterpret_cast<char**>(owner + 0x1d0);
    if (a && !(*reinterpret_cast<float*>(a + 0x54) > 0.0f))
        return false;
    base::reflection::UnkReflObject* o = *reinterpret_cast<base::reflection::UnkReflObject**>(owner + 0x240);
    if (!o)
        return false;
    if (!fn_710009d50c(o->vf_0x0(), fn_710048db44()))
        return false;
    char* p = reinterpret_cast<char*>(o);
    char* b = *reinterpret_cast<char**>(p + 0x30);
    return *reinterpret_cast<float*>(b + 0x54) > *reinterpret_cast<float*>(p + 0x149a4) + 50.0f;
}

}  // namespace

bool fn_710048098c(char* self) { return UnkChozoWarriorXAttack_CanStart(self); }
bool fn_7100482a44(char* self) { return UnkChozoWarriorXAttack_CanStart(self); }
bool fn_7100484a68(char* self) { return UnkChozoWarriorXAttack_CanStart(self); }
