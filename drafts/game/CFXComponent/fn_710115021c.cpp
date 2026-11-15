// CFXComponent unit: calls vf_0x158 on the object of every 32-byte entry (list at +0x50, count +0x60) and,
// when the object's byte at +0xb is 4, passes its +0x20 part to fn_7100206e84; then the base step.
// Names and layouts are placeholders.
#include "game/CComponent.h"
#include "types.h"

struct UnkFXPart_l15021c;
void fn_7100206e84(UnkFXPart_l15021c* part);

struct UnkFXObject_l15021c {
    void** vtable;
    u8 pad_0x8[3];
    u8 kind;  // 0xb
    u8 pad_0xc[0x14];
    u8 part[1];  // 0x20

    void vf_0x158() { reinterpret_cast<void (*)(UnkFXObject_l15021c*)>(vtable[0x158 / 8])(this); }
};

struct UnkFXEntry_l15021c {
    UnkFXObject_l15021c* object;
    u8 pad_0x8[0x18];
};

struct UnkFXList_l15021c {
    u8 pad_0x0[0x50];
    UnkFXEntry_l15021c* entries;  // 0x50
    u8 pad_0x58[8];
    u32 count;  // 0x60
};

static inline bool IsPart(const UnkFXObject_l15021c& o, const UnkFXObject_l15021c* p) { return (p == nullptr) | (o.kind != 4); }

void fn_710115021c(UnkFXList_l15021c* self) {
    for (UnkFXEntry_l15021c* it = self->entries; it != self->entries + self->count; ++it) {
        it->object->vf_0x158();
        if (!IsPart(*it->object, it->object))
            fn_7100206e84(reinterpret_cast<UnkFXPart_l15021c*>(it->object->part));
    }
    fn_71010c422c(reinterpret_cast<UnkMorphBallLauncher_b57*>(self));
}
