// CDoorShieldLifeComponent's unit (tools/tu_map.py unit 1377): a reflected function's invoker (fn_71008dc34c) that
// passes four string arguments by address, with the object at +8, to fn_71008d3cb4 (the door's four kind masks,
// src/game/CDoorLifeComponent/CDoorLifeComponent_Kinds_b51.cpp) and answers the empty status {0, 0, 2, 1}.
// Names are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkDoorLifeKinds;
void fn_71008d3cb4(UnkDoorLifeKinds* self, const char* const* a, const char* const* b, const char* const* c,
                   const char* const* d);

static inline __attribute__((always_inline)) void* ArgPtr_A5_b69(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71008dc34c(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    UnkDoorLifeKinds* self = *reinterpret_cast<UnkDoorLifeKinds**>(obj + 8);
    fn_71008d3cb4(self, static_cast<const char* const*>(ArgPtr_A5_b69(&a[0])),
                  static_cast<const char* const*>(ArgPtr_A5_b69(&a[1])),
                  static_cast<const char* const*>(ArgPtr_A5_b69(&a[2])),
                  static_cast<const char* const*>(ArgPtr_A5_b69(&a[3])));
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}

// fn_71008dc938 (object and arguments only): adds the key argument to the key list at +0xaf8 (fn_710002fd10) or
// removes it (fn_710002ffb4); when the list's state (fn_710002fb70) changed, the door opens (fn_71008d0ef4) or
// closes (fn_71008d498c).
struct UnkGuiKeyList;
struct UnkDoorLife_b44a2;
bool fn_710002fb70(void*);
bool fn_71008d0ef4(void*);
void fn_71008d498c(UnkDoorLife_b44a2* self, u32 a, u32 b, u32 c);

struct UnkDoorShieldLife_A5_b69 {
    u8 pad_0x0[0xaf8];
    u8 field_0xaf8[1];
    UnkGuiKeyList* keys() { return reinterpret_cast<UnkGuiKeyList*>(field_0xaf8); }
};

static inline __attribute__((always_inline)) void* ArgPtrNoNull_A5_b69(UnkReflValue* arg) {
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71008dc938(char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    UnkDoorShieldLife_A5_b69* self = *reinterpret_cast<UnkDoorShieldLife_A5_b69**>(obj + 8);
    bool add = *static_cast<const bool*>(ArgPtrNoNull_A5_b69(&a[0]));
    base::global::CStrId* key = static_cast<base::global::CStrId*>(ArgPtr_A5_b69(&a[1]));
    UnkGuiKeyList* list = self->keys();
    bool before = fn_710002fb70(list);
    bool after;
    if (add) {
        fn_710002fd10(list, key);
        after = fn_710002fb70(list);
    } else {
        fn_710002ffb4(list, key);
        after = fn_710002fb70(list);
    }
    if (before != after) {
        if (after)
            fn_71008d0ef4(self);
        else
            fn_71008d498c(reinterpret_cast<UnkDoorLife_b44a2*>(self), 0, 0, 0);
    }
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}
