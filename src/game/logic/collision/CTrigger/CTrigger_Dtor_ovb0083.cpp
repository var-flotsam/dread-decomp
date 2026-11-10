// game::logic::collision::CTrigger's unit (tools/tu_map.py unit 2519): two destructors (complete and deleting)
// of a trigger-side object that holds a counted reference (+0x18) and an array of counted references
// (+0x28, count at +0x24). Names and layouts are placeholders.
#include "types.h"

struct UnkTriggerRef_ovb0083;
struct UnkTriggerRefVtable_ovb0083 {
    void* slot_0x0;
    void* slot_0x8;
    void (*release)(UnkTriggerRef_ovb0083* self);  // 0x10
};
struct UnkTriggerRef_ovb0083 {
    UnkTriggerRefVtable_ovb0083* vtable;
    s32 ref_count;  // 0x8
};

struct UnkTriggerRefs_ovb0083 {
    char* vtable;
    u8 pad_0x8[0x10];
    UnkTriggerRef_ovb0083* owner;  // 0x18
    u8 pad_0x20[4];
    u32 count;  // 0x24
    UnkTriggerRef_ovb0083* refs[1];  // 0x28
};

extern char g_unk_7101aa8e40[];
struct UnkCameraCtrl_ovb0049;
void fn_7101043934(UnkCameraCtrl_ovb0049*);  // the base destructor

static inline void Release(UnkTriggerRef_ovb0083* ref) {
    s32* count = &ref->ref_count;
    if (--*count == 0)
        ref->vtable->release(ref);
}

static inline void DestroyRefs(UnkTriggerRefs_ovb0083* self) {
    self->vtable = g_unk_7101aa8e40 + 0x10;
    for (u64 i = 0; i < self->count; ++i) {
        if (UnkTriggerRef_ovb0083* ref = self->refs[i])
            Release(ref);
    }
    self->count = 0;
    if (UnkTriggerRef_ovb0083* owner = self->owner)
        Release(owner);
}

void fn_7100d70848(UnkTriggerRefs_ovb0083* self) {
    DestroyRefs(self);
    fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
}

void fn_7100d70904(UnkTriggerRefs_ovb0083* self) {
    DestroyRefs(self);
    fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
    operator delete(self);
}
