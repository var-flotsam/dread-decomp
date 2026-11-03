// CForcedMovementAreaComponent unit (tools/tu_map.py). fn_71007e94c0: after the base step
// (fn_710024f918), hands +0x30 to the object fn_710043acb4 finds for +0x18 (fn_71008656d8),
// resets +0x48 (fn_71007e9580), drops the references in the list at +0x70 (count +0x78) and
// registers itself with the game manager under the unit's id. Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkWeakTarget;

extern u64 g_unk_717271fdd8 __attribute__((visibility("hidden")));

struct UnkRefObj_b50fm {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        u32 ref_count;
    };
};

struct UnkForcedMovementArea_b50 {
    u8 pad_0x0[0x18];
    void* field_0x18;
    u8 pad_0x20[0x10];
    u8 field_0x30[0x18];
    u8 field_0x48[0x28];
    UnkRefObj_b50fm** refs;  // 0x70
    u32 ref_count;           // 0x78
};

void fn_710024f918(UnkForcedMovementArea_b50* self);
void* fn_710043acb4(void* key);
void fn_71008656d8(void* object, u32 a, u32 b, void* arg);
void fn_71007e9580(void* self);

void fn_71007e94c0(UnkForcedMovementArea_b50* self) {
    fn_710024f918(self);
    if (void* object = fn_710043acb4(self->field_0x18))
        fn_71008656d8(object, 0x10000, 0x10, self->field_0x30);
    fn_71007e9580(self->field_0x48);
    for (u64 i = 0; i < self->ref_count; ++i) {
        if (UnkRefObj_b50fm* ref = self->refs[i]) {
            if (--ref->ref_count == 0)
                ref->vf_0x10();
        }
    }
    self->ref_count = 0;
    fn_7100de9540(g_unk_7101d47310, &g_unk_717271fdd8, reinterpret_cast<UnkWeakTarget*>(self));
}
