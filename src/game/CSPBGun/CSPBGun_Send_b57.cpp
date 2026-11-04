// CSPBGun's unit (tools/tu_map.py): fn_710085e918, while the flag at +0x1880 is set, gives the
// owner's (+0x18) timeline (+0x2e8), if any, the key 0xef56c664609e1641 (fn_7100a2b7b8), resets the
// gun's entry (+0x1874) in g_unk_71726af0b0 for slot 3 (fn_7100ed6af0) and, when the owner has its
// sub-object at +0x220 and vtable slot 0x60 says yes, sends it one hash with mode 0 (as UnkSpitSend,
// src/game_s2/UnkSpitSend.h) and another under kind 0x5d523009b8f43e5f with mode 0x80 and second
// descriptor g_unk_717276974c; then clears the flags at +0x1880 and +0x18e8. Names are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "../UnkNamedSend_b54.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

extern void* g_unk_71726af0b0;
void fn_7100ed6af0(void* manager, u32 id, u32 slot);


struct UnkSPBGunOwner_b57 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x2e8 - 0x228];
    UnkTimelineComponent* field_0x2e8;
};

struct UnkSPBGun_b57 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual bool vf_0x60();

    u8 pad_0x8[0x10];
    UnkSPBGunOwner_b57* owner;
    u8 pad_0x20[0x1874 - 0x20];
    u32 id;
    u8 pad_0x1878[8];
    bool field_0x1880;
    u8 pad_0x1881[0x18e8 - 0x1881];
    bool field_0x18e8;
};

void fn_710085e918(UnkSPBGun_b57* self) {
    if (!self->field_0x1880)
        return;
    // Every local at function scope: none of their stack slots is shared, as in the original.
    u64 key;
    base::reflection::UnkReflTypeIdArg mode, mask, mode2, mask2;
    if (UnkTimelineComponent* timeline = self->owner->field_0x2e8) {
        key = 0xef56c664609e1641;
        fn_7100a2b7b8(timeline, &key);
    }
    fn_7100ed6af0(g_unk_71726af0b0, self->id, 3);
    void* target = self->owner->field_0x220;
    if (target && self->vf_0x60()) {
        mode.id = 0;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(target, 0x33b8f5dce9971029, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                      &g_unk_7172769740, &mask);
        mode2.id = 0x80;
        mask2.id = UnkAll_SpitSend().value;
        fn_7101176690(target, 0x94a143a64e136055, 0x5d523009b8f43e5f, &mode2, &g_unk_710164b958,
                      &g_unk_717276974c, &mask2);
    }
    self->field_0x1880 = false;
    self->field_0x18e8 = false;
}
