// Asks the global registry (created on first use) for the value named by a fixed string id and the
// string id at +0x510 (each use counted on its string data at 0x44; the first text defaults to
// the empty string) and returns it; 0 when nothing is found.
#include "types.h"

struct UnkNamedString {
    u8 unk_0x0[0x10];
    const char* text;
    u8 unk_0x18[0x2c];
    // A union member: the original reloads the ids after the increment.
    union {
        u32 uses;
    };
};
struct UnkRegistry;
struct UnkSelf {
    u8 unk_0x0[0x510];
    UnkNamedString* name;
};
struct UnkGroupIds {
    u8 unk_0x0[0x18];
    UnkNamedString* group;
};

extern UnkRegistry* g_unk_7101d47350;
extern UnkGroupIds g_unk_717272fd38;
void* fn_71000839a0(u32 size, u32 align_mask);
void fn_710013ac14(UnkRegistry* registry);
void fn_710013c7e0(UnkRegistry* registry, const char* group, const char* name, f32* value);

f32 fn_7100408770(UnkSelf* self) {
    f32 found = 0.0f;
    UnkRegistry* registry = g_unk_7101d47350;
    if (!registry) {
        registry = static_cast<UnkRegistry*>(fn_71000839a0(0x58, 0x3f));
        fn_710013ac14(registry);
        g_unk_7101d47350 = registry;
    }
    ++g_unk_717272fd38.group->uses;
    const char* group = g_unk_717272fd38.group->text ? g_unk_717272fd38.group->text : "";
    ++self->name->uses;
    const char* name = self->name->text;
    fn_710013c7e0(registry, group, name, &found);
    return found;
}
