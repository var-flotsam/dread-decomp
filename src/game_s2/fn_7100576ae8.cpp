// A lazily looked-up value: while the cached value at 0x18 is negative and both string ids
// (0x20, 0x28) are set, asks the global registry (created on first use) for the value named by
// the two strings (each use counted on its string data at 0x44) and returns it; the default
// -1 stays when nothing is found.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::global::UnkStringPool;
using base::reflection::UnkTunableRegistry;

struct UnkNamedString {
    u8 unk_0x0[0x10];
    const char* text;
    u8 unk_0x18[0x2c];
    // A union member: the original reloads the ids after the increment.
    union {
        u32 uses;
    };
};
struct UnkSelf {
    u8 unk_0x0[0x18];
    f32 value;
    u8 unk_0x1c[4];
    UnkNamedString* group;
    UnkNamedString* name;
};

extern UnkTunableRegistry* g_unk_7101d47350;
void fn_710013c7e0(UnkTunableRegistry* registry, const char* group, const char* name, f32* value);

f32 fn_7100576ae8(UnkSelf* self) {
    f32 value = self->value;
    if (!(value < 0.0f))
        return value;
    UnkNamedString* group_data = self->group;
    u64 empty = (u64)&g_unk_7101d47108->empty;
    if ((u64)group_data == empty || (u64)self->name == empty)
        return value;
    f32 found = -1.0f;
    UnkTunableRegistry* registry = g_unk_7101d47350;
    if (!registry) {
        registry = static_cast<UnkTunableRegistry*>(fn_71000839a0(0x58, 0x3f));
        fn_710013ac14(registry);
        g_unk_7101d47350 = registry;
    }
    ++self->group->uses;
    const char* group = self->group->text ? self->group->text : "";
    ++self->name->uses;
    const char* name = self->name->text ? self->name->text : "";
    fn_710013c7e0(registry, group, name, &found);
    return found;
}
