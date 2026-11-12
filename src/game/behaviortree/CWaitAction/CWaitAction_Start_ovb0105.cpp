// behaviortree::CWaitAction's unit (tools/tu_map.py): the action's start. After the base start, the wait
// time is reset from the node's default (+0x2c); when both tunable names (+0x38, +0x40) are set it is read
// from the global tunable registry (created on first use), else from the blackboard entry named at +0x30
// when there is one (fn_7100394e2c). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkTunableRegistry.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkTunableRegistry;

struct UnkStruct_0x68;
void fn_710013c7e0(UnkTunableRegistry* registry, const char* group, const char* name, f32* value);

struct UnkWaitName_b0105 {
    u8 unk_0x0[0x10];
    const char* text;
    u8 unk_0x18[0x2c];
    // A union member: the original reloads the ids after the increment.
    union {
        u32 uses;
    };
};

struct UnkWaitAction_b0105 {
    u8 pad_0x0[0x2c];
    f32 default_time;  // 0x2c
    CStrId key;  // 0x30
    UnkWaitName_b0105* group;  // 0x38
    UnkWaitName_b0105* name;  // 0x40
    f32 time;  // 0x48
    f32 elapsed;  // 0x4c
    bool field_0x50;
    bool field_0x51;
};

struct UnkSelf;

template <class T>
static inline bool IsSet(T* data) {
    return reinterpret_cast<u64>(data) != reinterpret_cast<u64>(&g_unk_7101d47108->empty);
}

void fn_7100393378(UnkWaitAction_b0105* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    self->time = self->default_time;
    self->elapsed = 0.0f;
    if (IsSet(self->group) && IsSet(self->name)) {
        UnkTunableRegistry* registry = g_unk_7101d47350;
        if (!registry) {
            registry = static_cast<UnkTunableRegistry*>(fn_71000839a0(0x58, 0x3f));
            fn_710013ac14(registry);
            g_unk_7101d47350 = registry;
        }
        ++self->group->uses;
        const char* group = self->group->text;
        ++self->name->uses;
        const char* name = self->name->text;
        fn_710013c7e0(registry, group, name, &self->time);
    } else if (self->key.data != &g_unk_7101d47108->empty) {
        fn_7100394e2c(reinterpret_cast<UnkSelf*>(self), &self->key, &self->time);
    }
    self->field_0x51 = self->field_0x50;
}
