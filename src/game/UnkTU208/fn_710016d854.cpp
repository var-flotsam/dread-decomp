// Unit 208: constructor of a node that keeps an owner, a value and three short value lists.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkTU208.h"
#include "game/UnkValueList.h"
#include "types.h"

extern char g_unk_7101a1dae8[];

struct UnkNodeEntry_d854 {
    u32 words[3] = {};
};

struct UnkNodeOwner_d854 {
    u8 pad_0x00[8];
    u8* scene;
    u8 pad_0x10[0x10];
    void* entries;  // 0x20, 12 bytes each
};

struct UnkValueNode3 {
    char* vtable;
    UnkNodeOwner_d854* owner;
    void* field_0x10;
    f32 field_0x18;
    u8 pad_0x1c[4];
    void* field_0x20;
    u32 field_0x28;
    u8 pad_0x2c[4];
    UnkValueList a;  // 0x30
    UnkValueList b;  // 0x48
    UnkValueList c;  // 0x60
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_710016d854(UnkValueNode3* self, UnkNodeOwner_d854* owner, void* arg, u32 countA, const u32* valuesA, u32 countB,
                   const u32* valuesB, u32 countC, const u32* valuesC, f32 scale) {
    self->vtable = g_unk_7101a1db18 + 0x10;
    self->owner = owner;
    if (!owner->entries) {
        u32 n = *reinterpret_cast<const u32*>(owner->scene + 0x38c);
        UnkNodeEntry_d854* p = static_cast<UnkNodeEntry_d854*>(fn_7100083ba0((u64)n * sizeof(UnkNodeEntry_d854), 0x1a));
        if (n) {
            UnkNodeEntry_d854* e = p;
            do {
                new (e) UnkNodeEntry_d854;
                ++e;
            } while (e != p + n);
        }
        owner->entries = p;
    }
    self->field_0x10 = arg;
    self->vtable = g_unk_7101a1dae8 + 0x10;
    self->field_0x20 = nullptr;
    self->field_0x28 = 0;
    self->a = UnkValueList();
    self->b = UnkValueList();
    self->c = UnkValueList();
    self->field_0x18 = scale;
    self->a.Set(countA, valuesA);
    self->b.Set(countB, valuesB);
    self->c.Set(countC, valuesC);
}
