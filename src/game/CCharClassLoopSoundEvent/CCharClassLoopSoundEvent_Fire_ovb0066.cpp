// CCharClassLoopSoundEvent's unit (tools/tu_map.py unit 2262): the event's fire (starts the loop
// sound through fn_7100cdc910 unless the GUI (g_unk_7101d47330) answers fn_7101198688 or the game
// manager answers fn_7100deabe4) and its copy-assignment. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2730.h"
#include "types.h"

using base::UnkStrIdMember;
using base::UnkStrIdMemberOf;
using base::global::CStrId;

extern void* g_unk_7101d47330;

struct UnkLoopSoundEvent_ovb0066 {
    virtual void vf_0x0();

    u32 field_0x8;
    UnkStrIdMemberOf<g_unk_7101d47200> path;  // 0x10
    UnkStrIdMember field_0x18;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    bool field_0x30;
    bool field_0x31;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    bool field_0x44;
    UnkStrIdMember sound;  // 0x48
};

void fn_7100cdc910(UnkLoopSoundEvent_ovb0066* self, UnkStrIdMemberOf<g_unk_7101d47200>* path,
                   UnkStrIdMember* sound, const CStrId* a, const CStrId* b, f32 f0, f32 f1, f32 f2, f32 f3,
                   bool c, bool d, f32 f4, f32 f5, f32 f6, f32 f7, bool e, bool g, f32 f8, void* arg);

void fn_7100cc7a78(UnkLoopSoundEvent_ovb0066* self, void* arg) {
    if (fn_7101198688(g_unk_7101d47330))
        return;
    if (fn_7100deabe4(g_unk_7101d47310))
        return;
    fn_7100cdc910(self, &self->path, &self->sound, &g_unk_7101d470e0, &g_unk_7101d470e0, self->field_0x20,
                  self->field_0x24, self->field_0x28, self->field_0x2c, self->field_0x30, true, -1.0f,
                  self->field_0x34, self->field_0x38, self->field_0x3c, self->field_0x31, self->field_0x44,
                  self->field_0x40, arg);
}

__attribute__((flatten)) UnkLoopSoundEvent_ovb0066* fn_7100cc7e18(UnkLoopSoundEvent_ovb0066* self, const UnkLoopSoundEvent_ovb0066* other) {
    *self = *other;
    return self;
}
