// Unit 208: constructor of a node that keeps an owner, a value and two short value lists.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkTU208.h"
#include "game/UnkValueList.h"
#include "types.h"

extern char g_unk_7101a1dab8[];

struct UnkNodeEntry {
    u32 words[2] = {};
};

struct UnkNodeOwner {
    u8 pad_0x00[8];
    u8* scene;
    u8 pad_0x10[8];
    void* entries;  // 0x18, 8 bytes each
};

struct UnkValueNode {
    char* vtable;
    UnkNodeOwner* owner;
    void* field_0x10;
    f32 field_0x18;
    u8 pad_0x1c[4];
    void* field_0x20;
    UnkValueList a;  // 0x28
    UnkValueList b;  // 0x40
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_710016d59c(UnkValueNode* self, UnkNodeOwner* owner, void* arg, u32 countA, const u32* valuesA, u32 countB,
                   const u32* valuesB, f32 scale) {
    self->owner = owner;
    self->vtable = g_unk_7101a1db48 + 0x10;
    if (!owner->entries) {
        u32 n = *reinterpret_cast<const u32*>(owner->scene + 0x38c);
        UnkNodeEntry* p = static_cast<UnkNodeEntry*>(fn_7100083ba0((u64)n * sizeof(UnkNodeEntry), 0x1a));
        if (n) {
            UnkNodeEntry* e = p;
            do {
                new (e) UnkNodeEntry;
                ++e;
            } while (e != p + n);
        }
        owner->entries = p;
    }
    self->field_0x10 = arg;
    self->vtable = g_unk_7101a1dab8 + 0x10;
    self->field_0x20 = nullptr;
    self->a = UnkValueList();
    self->b = UnkValueList();
    self->field_0x18 = scale;
    self->a.Set(countA, valuesA);
    self->b.Set(countB, valuesB);
}
