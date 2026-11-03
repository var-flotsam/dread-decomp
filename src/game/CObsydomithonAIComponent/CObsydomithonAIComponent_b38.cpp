// CObsydomithonAIComponent: initialisation. Runs the base step, keeps the owner's object at
// +0x14730 when it is a CCharClassObsydomithonAIComponent (null otherwise), registers the embedded
// entry at +0x146c8 in the list at 0x161c (initialised from +0x510 of that object) and clamps the
// angle at g_unk_71726f9404 to [0, 90]. Returns bit 0 of the base step's result.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

typedef u64 u64_a4 __attribute__((aligned(4)));

struct UnkObsydomithonObject {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkObsydomithonObject*)>(vtable[0])(this); }
};

struct UnkComponent;
bool fn_7100369490(UnkComponent*);
void fn_71006286c0(void* entry, u64 source);
extern f32 g_unk_71726f9404;

u32 fn_710062c8d0(char* self) {
    u32 result = reinterpret_cast<u32 (*)(char*)>(fn_7100369490)(self);
    UnkObsydomithonObject* object = *reinterpret_cast<UnkObsydomithonObject**>(self + 0x10);
    UnkObsydomithonObject* component = nullptr;
    if (object && fn_710009d50c(object->vf_0x0(), fn_710062bcd4()))
        component = object;
    u64* slot = reinterpret_cast<u64*>(self + 0x14730);
    *slot = reinterpret_cast<u64>(component);
    char* entry = self + 0x146c8;
    u32 index = *reinterpret_cast<u32*>(self + 0x1618);
    char* list = self + 0x1618;  // the count, then the entries
    *reinterpret_cast<u64_a4*>(list + ((u64)index << 3) + 4) = reinterpret_cast<u64>(entry);
    *reinterpret_cast<u32*>(self + 0x1618) = index + 1;
    fn_71006286c0(entry, *slot + 0x510);
    f32 angle = g_unk_71726f9404;
    if (!(angle >= 0.0f && angle <= 90.0f)) {
        f32 low = 0.0f;
        f32 high = angle >= 90.0f ? 90.0f : angle;
        g_unk_71726f9404 = angle > low || angle != angle ? high : low;
    }
    return result & 1;
}
