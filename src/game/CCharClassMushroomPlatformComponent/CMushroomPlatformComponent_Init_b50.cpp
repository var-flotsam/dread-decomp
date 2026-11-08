// CCharClassMushroomPlatformComponent unit (tools/tu_map.py, the name from reflection): after the base
// step (fn_710115fbe4), keeps the character class (+0x10) when it is a
// CCharClassMushroomPlatformComponent (fn_710009d50c against the type in the unit's TU data; the getter
// runs first so the type is registered) at +0x520, copies two of its values when +0x21 is set, and
// succeeds only when both the base step did and the class is one. Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;

extern CType g_unk_717272c1a8 __attribute__((visibility("hidden")));  // the class's type

bool fn_710115fbe4(char* s, void* arg);

struct UnkMushroomCharClass {
    u8 unk_0x0[0xa4];
    u32 field_0xa4;
    u32 field_0xa8;
};

static inline UnkMushroomCharClass* AsMushroom(UnkReflObject* obj) {
    if (__builtin_expect(obj == nullptr, 0))
        return nullptr;
    const CType* type = obj->vf_0x0();
    fn_710088a0bc();
    return fn_710009d50c(type, &g_unk_717272c1a8) ? reinterpret_cast<UnkMushroomCharClass*>(obj) : nullptr;
}

// `arg` (x1) reaches the base step untouched in the original (tools/forwarded_args.py).
bool fn_710088a88c(char* self, void* arg) {
    bool ok = fn_710115fbe4(self, arg);
    UnkMushroomCharClass* cls = AsMushroom(*reinterpret_cast<UnkReflObject**>(self + 0x10));
    asm("" : "+r"(cls));  // the original tests the stored class again instead of threading the jumps
    *reinterpret_cast<UnkMushroomCharClass**>(self + 0x520) = cls;
    if (cls && *reinterpret_cast<bool*>(self + 0x21)) {
        *reinterpret_cast<u32*>(self + 0x3f4) = cls->field_0xa4;
        *reinterpret_cast<u32*>(self + 0x46c) = cls->field_0xa8;
    }
    return ok & (cls != nullptr);
}
