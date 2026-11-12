// CCutsceneComponent's unit (tools/tu_map.py unit 1208): a reflected function's invoker (fn_71007ae4e8) that passes
// two u32 arguments and a bool with the object at +8 to fn_71007a8724 and answers the empty status {0, 0, 2, 1}.
// Names are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CCutsceneComponent.h"
#include "types.h"

using base::reflection::UnkReflValue;

void fn_71007a8724(void* self, u32 a, u32 b, bool flag);

static inline __attribute__((always_inline)) void* ArgPtr_A5_b69(UnkReflValue* arg) {
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71007ae4e8(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    void* self = *reinterpret_cast<void**>(obj + 8);
    u32 x = *static_cast<const u32*>(ArgPtr_A5_b69(&a[0]));
    u32 y = *static_cast<const u32*>(ArgPtr_A5_b69(&a[1]));
    bool flag = *static_cast<const bool*>(ArgPtr_A5_b69(&a[2]));
    fn_71007a8724(self, x, y, flag);
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}

// fn_71007ae2bc: finds the 0x88-byte entry (array at +0xd0 of the object at +8) whose id at +0x30 is the first
// argument's and passes its index and the two flag arguments (normalised) to fn_71007a95f8.
struct UnkElem_0x88;

struct UnkCutsceneOwner_A5_b69 {
    u8 pad_0x0[0xd0];
    base::UnkVector<UnkElem_0x88> entries;
};

void fn_71007a95f8(UnkCutsceneOwner_A5_b69* self, u32 index, bool a, bool b, bool c);

static inline __attribute__((always_inline)) void* ArgPtrOrNull_A5_b69(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71007ae2bc(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    UnkCutsceneOwner_A5_b69* self = *reinterpret_cast<UnkCutsceneOwner_A5_b69**>(obj + 8);
    const u64* id = static_cast<const u64*>(ArgPtrOrNull_A5_b69(&a[0]));
    u8 first = *static_cast<const u8*>(ArgPtr_A5_b69(&a[1]));
    u8 second = *static_cast<const u8*>(ArgPtr_A5_b69(&a[2]));
    for (u32 i = 0; i < self->entries.count; ++i) {
        UnkElem_0x88& e = fn_71007a6b58(&self->entries, i);
        if (*reinterpret_cast<const u64*>(reinterpret_cast<char*>(&e) + 0x30) == *id) {
            bool b = second;
            bool a = first;
            fn_71007a95f8(self, i, a, a, b);
            break;
        }
    }
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}
