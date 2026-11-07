// CCharClassLavaPoolComponent unit (tools/tu_map.py, the name from reflection): after the base step
// (fn_7100900610), keeps the character class (+0x10) when it is a CCharClassLavaPoolComponent
// (fn_710009d50c against the type in the unit's TU data; the getter runs first so the type is
// registered) at +0xca8 and, when +0x21 is set, copies five of its values to +0xc90. Answers the base
// step's result. Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;

extern CType g_unk_7172733578 __attribute__((visibility("hidden")));  // the class's type

bool fn_7100900610(char* s);

struct UnkLavaPoolCharClass {
    u8 unk_0x0[0x88];
    u32 values[5];  // 0x88
};

static inline UnkLavaPoolCharClass* AsLavaPool(UnkReflObject* obj) {
    if (__builtin_expect(obj == nullptr, 0))
        return nullptr;
    const CType* type = obj->vf_0x0();
    fn_71008fabac();
    return fn_710009d50c(type, &g_unk_7172733578) ? reinterpret_cast<UnkLavaPoolCharClass*>(obj) : nullptr;
}

// `arg` (x1) reaches the base step untouched in the original (tools/forwarded_args.py).
bool fn_71008fbdcc(char* self, void* arg) {
    bool ok = reinterpret_cast<bool (*)(char*, void*)>(fn_7100900610)(self, arg);
    UnkLavaPoolCharClass* cls = AsLavaPool(*reinterpret_cast<UnkReflObject**>(self + 0x10));
    *reinterpret_cast<UnkLavaPoolCharClass**>(self + 0xca8) = cls;
    if (*reinterpret_cast<bool*>(self + 0x21)) {
        u32* out = reinterpret_cast<u32*>(self + 0xc90);
        out[0] = cls->values[0];
        out[1] = cls->values[1];
        out[2] = cls->values[2];
        out[3] = cls->values[3];
        out[4] = cls->values[4];
    }
    return ok;
}
