// Unit 1501 (CSamusMovement): fn_71009f0c34 is the assignment of a fixed-capacity array of 0x48-byte
// targets (two counted slots and 13 words): the common entries are assigned, the rest copied in, and the
// count taken over. Names and layouts are placeholders.
#include <new>

#include "game/CAbilityShinespark.h"
#include "game/UnkAssignedSlot.h"
#include "types.h"

#pragma pack(push, 4)
struct UnkTarget_l9f0c34 {
    UnkAssignedSlot slot;  // 0x0
    UnkAssignedSlot slot_0x8;  // 0x8
    u32 words[13];  // 0x10
    u32 unk_0x44;

    UnkTarget_l9f0c34(const UnkTarget_l9f0c34& other) { fn_7100331c7c(this, &other); }
    UnkTarget_l9f0c34& operator=(const UnkTarget_l9f0c34& other) {
        slot = other.slot;
        slot_0x8 = other.slot_0x8;
        for (u32 i = 0; i < 13; ++i)
            words[i] = other.words[i];
        return *this;
    }
};

struct UnkTargetArray_l9f0c34 {
    u32 count;  // 0x0
    u8 storage[16 * sizeof(UnkTarget_l9f0c34)];  // 0x4, the entries, constructed as they are added

    UnkTarget_l9f0c34* data() { return reinterpret_cast<UnkTarget_l9f0c34*>(storage); }
    const UnkTarget_l9f0c34* data() const { return reinterpret_cast<const UnkTarget_l9f0c34*>(storage); }

    // Copies `target` into the next free entry.
    void PushBack(const UnkTarget_l9f0c34& target) {
        new (&data()[count]) UnkTarget_l9f0c34(target);
        count++;
    }
};
#pragma pack(pop)
static_assert(sizeof(UnkTarget_l9f0c34) == 0x48, "UnkTarget_l9f0c34 layout");

UnkTargetArray_l9f0c34* fn_71009f0c34(UnkTargetArray_l9f0c34* self, const UnkTargetArray_l9f0c34* other) {
    if (self != other) {
        if (self->count > other->count) {
            for (u64 i = 0; i < other->count; i++)
                self->data()[i] = other->data()[i];
        } else {
            for (u64 i = 0; i < self->count; i++)
                self->data()[i] = other->data()[i];
        }
        for (u32 i = self->count; i < other->count; i++)
            self->PushBack(other->data()[i]);
        self->count = other->count;
    }
    return self;
}
