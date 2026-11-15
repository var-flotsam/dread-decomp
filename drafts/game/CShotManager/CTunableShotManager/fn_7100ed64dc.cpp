// CShotManager::CTunableShotManager unit: takes the first free effect slot, starts it for a type
// and variant at the given place and queues it on that type's explosion. Names and layouts are
// placeholders.
#include "types.h"

struct UnkShotEffect_ed64dc {
    u8 pad_0x0[0x18];
    bool field_0x18;  // live
    u8 pad_0x19[0x7];
    f32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;  // the type
    u32 field_0x2c;  // the variant
    u8 pad_0x30[0xcc];
};

struct UnkShotManager_ed64dc {
    u8 pad_0x0[0x30];
    UnkShotEffect_ed64dc* effects;
    u32 effectCount;
    u8 pad_0x3c[0x240 - 0x3c];
    void* field_0x240[5][5];
};

void fn_7100ddec50(UnkShotEffect_ed64dc* self, u32 a, u32 b);
void fn_7100ddec68(UnkShotEffect_ed64dc* self, const void* a, const void* b);
f32 fn_7100de1388(void* self);
void fn_7100de08f0(void* self, UnkShotEffect_ed64dc* target);

void fn_7100ed64dc(UnkShotManager_ed64dc* self, const void* a, const void* b, u32, u32 type, u32, u32 variant) {
    for (UnkShotEffect_ed64dc *it = self->effects, *end = self->effects + self->effectCount; it != end; ++it) {
        if (it->field_0x18)
            continue;
        void* explosion = self->field_0x240[type][variant];
        fn_7100ddec50(it, 1, 1);
        fn_7100ddec68(it, a, b);
        it->field_0x2c = variant;
        it->field_0x28 = type;
        it->field_0x20 = fn_7100de1388(explosion);
        it->field_0x24 = 0;
        fn_7100de08f0(explosion, it);
        return;
    }
}
