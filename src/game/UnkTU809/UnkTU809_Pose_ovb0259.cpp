// Unit 809's pose picker (tools/tu_map.py hint "CCharClassEmmyMovement"): the table entry of the
// movement for the owner's current situation. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkNavMeshCell.h"
#include "types.h"

struct UnkEmmyAIView;
bool fn_710054ad2c(UnkEmmyAIView*);
struct UnkEmmyAI2_b62a3;
bool fn_710054aecc(UnkEmmyAI2_b62a3*);
u32 fn_71010cc38c(void*);
void* fn_7100964f50(void* self, void* owner);

struct UnkEmmyTarget_ovb0259 {
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
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual void vf_0x1c0();
    virtual void vf_0x1c8();
    virtual void vf_0x1d0();
    virtual void vf_0x1d8();
    virtual void vf_0x1e0();
    virtual void vf_0x1e8();
    virtual void vf_0x1f0();
    virtual void vf_0x1f8();
    virtual void vf_0x200();
    virtual void vf_0x208();
    virtual void vf_0x210();
    virtual void vf_0x218();
    virtual void vf_0x220();
    virtual void vf_0x228();
    virtual void vf_0x230();
    virtual void vf_0x238();
    virtual void vf_0x240();
    virtual void vf_0x248();
    virtual void vf_0x250();
    virtual void vf_0x258();
    virtual void vf_0x260();
    virtual void vf_0x268();
    virtual void vf_0x270();
    virtual void vf_0x278();
    virtual void vf_0x280();
    virtual void vf_0x288();
    virtual void vf_0x290();
    virtual void vf_0x298();
    virtual void vf_0x2a0();
    virtual void vf_0x2a8();
    virtual void vf_0x2b0();
    virtual void vf_0x2b8();
    virtual void vf_0x2c0();
    virtual void vf_0x2c8();
    virtual UnkNavMeshCell* vf_0x2d0();
};

struct UnkEmmyOwner_ovb0259 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
    u8 pad_0x78[0x218 - 0x78];
    UnkEmmyTarget_ovb0259* target;  // 0x218
    u8 pad_0x220[0x240 - 0x220];
    base::reflection::UnkReflObject* ai;  // 0x240
};

struct UnkEmmyMovement_ovb0259 {
    u8 pad_0x0[0x1d0];
    u8 poses[0x1d0];  // 0x1d0, in entries of 0x40
};

static inline char* Entry(UnkEmmyMovement_ovb0259* self, u32 offset) {
    return reinterpret_cast<char*>(self) + offset;
}

// The entry for the owner's heading (the matrix element at +0x18 of its transform).
static inline char* ByHeading(UnkEmmyMovement_ovb0259* self, UnkEmmyOwner_ovb0259* owner, u32 positive, u32 other) {
    f32 heading = fn_710002614c(&owner->xform).m[6];
    char* a = Entry(self, positive);
    char* b = Entry(self, other);
    return heading > 0.0f ? a : b;
}

void* fn_710052d718(UnkEmmyMovement_ovb0259* self, UnkEmmyOwner_ovb0259* owner) {
    UnkEmmyTarget_ovb0259* target = owner->target;
    if (target) {
        UnkNavMeshCell* cell = target->vf_0x2d0();
        if (cell && fn_71002b005c(cell))
            return Entry(self, 0x390);
        base::reflection::UnkReflObject* ai = owner->ai;
        if (ai && fn_710009d50c(ai->vf_0x0(), fn_710053b934()) &&
            (fn_710054ad2c(reinterpret_cast<UnkEmmyAIView*>(ai)) ||
             fn_710054aecc(reinterpret_cast<UnkEmmyAI2_b62a3*>(ai)))) {
            switch (fn_71010cc38c(target)) {
            case 2:
                return Entry(self, 0x290);
            case 3:
                return ByHeading(self, owner, 0x2d0, 0x310);
            case 4:
                return Entry(self, 0x350);
            }
        } else {
            switch (fn_71010cc38c(target)) {
            case 2:
                return fn_7100964f50(self, owner);
            case 3:
                return ByHeading(self, owner, 0x1d0, 0x210);
            case 4:
                return Entry(self, 0x250);
            }
        }
    }
    return fn_7100964f50(self, owner);
}
