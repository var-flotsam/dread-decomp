// CEmmyGrabSamusAttack (unit 800): stores a string value in a property bag. The value gets a
// 256-byte slot in the bag's stream (found by the key's hash, or appended with type 's'), the key
// is added to the bag's names, and the text is written there, zero-padded. Names and layouts are
// placeholders.
#include <algorithm>

#include "../../game_s7/UnkPropBagS7.h"
#include "base/UnkCloneRecords.h"
#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "types.h"

namespace unk_prop_bag {

// A value's slot: where it is in the stream, its key's hash and its type.
struct UnkPropSlot {
    u64 offset;
    u64 hash;
    u32 type;

    UnkPropSlot& operator=(const UnkPropSlot& other) {
        offset = other.offset;
        hash = other.hash;
        type = other.type;
        return *this;
    }
};

// The bag's slots (CRntVector's layout), grown by its step.
struct UnkPropSlotVec {
    UnkPropSlot* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkPropSlot* Find(u64 hash) {
        return std::find_if(data, data + count, [hash](const UnkPropSlot& slot) { return slot.hash == hash; });
    }

};

// The bag as this unit sees it: the slots, the stream and the names.
struct UnkPropBagSlots {
    UnkPropSlotVec slots;
    UnkHashStream_0x38 stream;  // 0x28
    base::UnkStrIdVec names;    // 0x60

    void AddName(const CStrId& key) {
        CStrId* it = std::find(names.data, names.data + names.count, key);
        if (it == names.data + names.count)
            names.PushBack(key);
    }

};
static_assert(__builtin_offsetof(UnkPropBagSlots, names) == 0x60, "layout");

// Appends a slot, growing the array by its v->step.
static void PushBack(UnkPropSlotVec* v, u64 offset, u64 hash, u32 type) {
    u32 n = v->count;
    u32 old_capacity = v->capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && v->step != 0)
        new_capacity = (n / v->step + 1) * v->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    if (old_capacity < new_capacity) {
        UnkPropSlot* p =
            static_cast<UnkPropSlot*>(v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkPropSlot), 8, v->flags));
        if (v->data) {
            for (u32 i = 0; i < v->count; ++i)
                p[i] = v->data[i];
            v->allocator->vf_0x48(v->data);
        }
        v->data = p;
        v->capacity = new_capacity;
    }
    UnkPropSlot& slot = v->data[v->count];
    slot.offset = offset;
    slot.hash = hash;
    slot.type = type;
    ++v->count;
}

void fn_710050e4dc(UnkPropBag* bag, const CStrId& key, const char* value) {
    UnkPropBagSlots* self = reinterpret_cast<UnkPropBagSlots*>(bag);
    const u64 kSlotSize = 0x100;

    // The value's slot: the existing one for the key, or a new one at the stream's end.
    UnkPropSlot* it = self->slots.Find(key.data->Key());
    u64 offset = fn_710007c1a4(&self->stream);
    if (it == self->slots.data + self->slots.count)
        PushBack(&self->slots, offset, key.data->Key(), 's');
    else
        offset = it->offset;
    self->AddName(key);

    CRntFile_Seek(&self->stream, offset, 0);
    u64 length = __builtin_strlen(value) + 1;
    u64 size = length < kSlotSize ? length : kSlotSize;
    CRntFile_Write(&self->stream, value, size);
    self->AddName(key);
    if (length < kSlotSize)
        CRntFile_Fill(&self->stream, 0, kSlotSize - size);
}

}  // namespace unk_prop_bag
