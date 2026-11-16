// Unit 2506: erases one group (and its handle) from a group array, moving the last group and handle into
// its place; returns the group now at the erased position. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "types.h"

#pragma pack(push, 4)
struct UnkSlotRecord_ovb0244 {
    base::UnkRefPtrMember refs[5];
    s32 id;
    u64 words[6];
    u32 pad_0x5c;
    u64 field_0x60;
    f32 value;
    u32 pad_0x6c;
    UnkSlotRecord_ovb0244() : refs{}, id(-1), words{}, field_0x60(0), value(-1.0f) {}
};
struct UnkSlotGroup_ovb0244 {
    u32 count;
    UnkSlotRecord_ovb0244 items[128];
};
#pragma pack(pop)

char* fn_7100d4b2fc(char* p0, char* p1);

// Takes over the records of another group.
static inline void CopyGroup(UnkSlotGroup_ovb0244& dst, UnkSlotGroup_ovb0244& src) {
    for (u64 j = 0; j < src.count; ++j)
        fn_7100d4b2fc(reinterpret_cast<char*>(&dst.items[j]), reinterpret_cast<char*>(&src.items[j]));
    dst.count = src.count;
}

// The handle's count is a union member: the original reloads the pointers around count updates here.
struct UnkRefCountedU_ovb0244 {
    virtual void vf0();
    virtual void vf8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
};

using UnkGroupArray_ovb0244 = base::global::CRntSmallDictionary<UnkRefCountedU_ovb0244*, UnkSlotGroup_ovb0244>;

void fn_7100d48e50(UnkSlotGroup_ovb0244* self);

static inline void Release(UnkRefCountedU_ovb0244* h) {
    if (h) {
        if (--h->ref_count == 0)
            h->Delete();
    }
}

UnkSlotGroup_ovb0244* fn_7100d47434(UnkGroupArray_ovb0244* self, UnkSlotGroup_ovb0244* group) {
    u32 index = group - self->values;
    fn_7100d48e50(&self->values[index]);
    Release(self->keys[index]);
    u32 last = self->count - 1;
    if (last != index) {
        CopyGroup(self->values[index], self->values[last]);
        fn_7100d48e50(&self->values[self->count - 1]);

        UnkRefCountedU_ovb0244** handles = self->keys;
        handles[index] = nullptr;
        if (UnkRefCountedU_ovb0244* h = handles[self->count - 1]) {
            ++h->ref_count;
            Release(handles[index]);
            handles[index] = h;
        }
        Release(self->keys[self->count - 1]);
    }
    --self->count;
    return &self->values[index];
}
