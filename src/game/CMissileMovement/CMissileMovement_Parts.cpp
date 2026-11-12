// A function in the CMissileMovement unit: fn_7100abaee8 marks the object with hash 0x07133f93f7cc2a9c in the
// owner's world (+0x280 of the owner at +0x30) by the state bits 0x8000400 at +0x3fc, then looks up the part
// named by the definition at +0x5d8 (its +0x30) and, when it is live, passes it the data at +0x4d8
// (fn_7100146c50). Names and layouts are placeholders.
#include "types.h"

struct UnkWorld;

struct UnkMissilePart {
    u8 pad_0x0[0xb];
    u8 field_0xb;
};

struct UnkMissileOwner {
    u8 pad_0x0[0x280];
    UnkWorld* field_0x280;
};

struct UnkMissileDef {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkMissileMovement {
    u8 pad_0x0[0x30];
    UnkMissileOwner* field_0x30;
    u8 pad_0x38[0x3fc - 0x38];
    u32 field_0x3fc;
    u8 pad_0x400[0x4d8 - 0x400];
    u8 field_0x4d8[0x100];
    UnkMissileDef* field_0x5d8;
};

struct UnkPart;
UnkPart* fn_7101152b90(UnkWorld* world, u64 hash);
struct UnkObject;
bool fn_710114e668(UnkObject*, u8);
void fn_710114fe8c(UnkWorld* world, u64 hash, u32 a, u32 b);
bool fn_7100146c50(UnkMissilePart* part, u32 a, void* data, u32 b);

void fn_7100abaee8(UnkMissileMovement* self) {
    bool active = (self->field_0x3fc & 0x8000400) != 0;
    fn_710114fe8c(self->field_0x30->field_0x280, 0x07133f93f7cc2a9cull, active, 1);
    UnkWorld* world = self->field_0x30->field_0x280;
    if (UnkMissilePart* part = reinterpret_cast<UnkMissilePart*>(fn_7101152b90(world, self->field_0x5d8->field_0x30))) {
        if (fn_710114e668(reinterpret_cast<UnkObject*>(world), part->field_0xb))
            fn_7100146c50(part, 0, self->field_0x4d8, 1);
    }
}
