// Unit 2506 (no reliable name): a container of refcounted handles in groups. A group is a count and up
// to 128 records of 0x70 bytes (five refcounted pointers, an id, cleared words and a float), kept next to an
// array of handles; the array header (groups, handles, count, capacity, step, flags, allocator) is shared by
// the functions below: the group clear (fn_7100d48e50), the pop that default-constructs on an empty array
// (fn_7100d4b4b0), the reserve (fn_7100d4b0f4), the pair appender of 8-byte values (fn_7100d47d1c), the
// named-object creator (fn_7100d475ec) and a record initialiser (fn_7100d4a740). Names and layouts are
// placeholders.
#include <new>

#include "base/UnkDestructors.h"
#include "base/UnkSList.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game_s2/UnkWeakHandle.h"
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

// The handle's count is a union member: the original reloads the pointers around count updates here.
struct UnkRefCountedU_ovb0244 {
    virtual void vf0();
    virtual void vf8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
};

struct UnkGroupArray_ovb0244 {
    UnkSlotGroup_ovb0244* groups;
    UnkRefCountedU_ovb0244** handles;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

char* fn_7100d4b2fc(char* p0, char* p1);

// Releases the five handles of every record and empties the group.
__attribute__((noinline)) void fn_7100d48e50(UnkSlotGroup_ovb0244* self) {
    for (u64 i = 0; i < self->count; ++i)
        self->items[i].~UnkSlotRecord_ovb0244();
    self->count = 0;
}

void fn_7100d4b4b0(UnkSlotGroup_ovb0244* self) {
    u32 n = self->count;
    u32 new_count = n - 1;
    if (n != 0) {
        for (u64 i = new_count; i < self->count; ++i)
            self->items[i].~UnkSlotRecord_ovb0244();
    } else {
        for (u32 i = n; i < new_count; ++i)
            new (&self->items[i]) UnkSlotRecord_ovb0244();
    }
    self->count = new_count;
}

void fn_7100d4b0f4(UnkGroupArray_ovb0244* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    UnkSlotGroup_ovb0244* groups = static_cast<UnkSlotGroup_ovb0244*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkSlotGroup_ovb0244), 8, self->flags));
    UnkRefCountedU_ovb0244** handles =
        static_cast<UnkRefCountedU_ovb0244**>(self->allocator->vf_0x40((u64)capacity * 8, 8, self->flags));
    UnkSlotGroup_ovb0244* old_groups = self->groups;
    if (old_groups) {
        if (self->count) {
            UnkSlotGroup_ovb0244* dst = groups;
            UnkSlotGroup_ovb0244* src = old_groups;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                for (u64 j = 0; j < src->count; ++j)
                    fn_7100d4b2fc(reinterpret_cast<char*>(&dst->items[j]), reinterpret_cast<char*>(&src->items[j]));
                dst->count = src->count;
                fn_7100d48e50(src);
            }
            old_groups = self->groups;
        }
        self->allocator->vf_0x48(old_groups);
    }
    UnkRefCountedU_ovb0244** old_handles = self->handles;
    if (old_handles) {
        if (self->count) {
            u64 idx = 0;
            for (u32 i = 0; i < self->count; ++i, ++idx) {
                handles[idx] = nullptr;
                if (UnkRefCountedU_ovb0244* h = old_handles[idx]) {
                    ++h->ref_count;
                    if (UnkRefCountedU_ovb0244* prev = handles[idx]) {
                        if (--prev->ref_count == 0)
                            prev->Delete();
                    }
                    handles[idx] = h;
                    if (UnkRefCountedU_ovb0244* o = old_handles[idx]) {
                        if (--o->ref_count == 0)
                            o->Delete();
                    }
                }
            }
            old_handles = self->handles;
        }
        self->allocator->vf_0x48(old_handles);
    }
    self->groups = groups;
    self->handles = handles;
    self->capacity = capacity;
}

// Two parallel arrays of 8-byte values sharing a count, capacity, step, flags and allocator.
struct UnkWordPairs_ovb0244 {
    u64* first;
    u64* second;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkWorld_ovb0244 {
    u8 pad_0x0[0x118];
    UnkWordPairs_ovb0244 pairs;  // 0x118
};

void fn_7100d47d1c(UnkWorld_ovb0244* self, u64 a, u64 b) {
    UnkWordPairs_ovb0244* p = &self->pairs;
    u32 n = p->count;
    if (n == p->capacity) {
        u32 grown = p->step + n;
        if (n < grown) {
            u64* first = static_cast<u64*>(p->allocator->vf_0x40((u64)grown * 8, 8, p->flags));
            u64* second = static_cast<u64*>(p->allocator->vf_0x40((u64)grown * 8, 8, p->flags));
            if (p->first) {
                __builtin_memcpy(first, p->first, (u64)p->count * 8);
                p->allocator->vf_0x48(p->first);
            }
            if (p->second) {
                __builtin_memcpy(second, p->second, (u64)p->count * 8);
                p->allocator->vf_0x48(p->second);
            }
            n = p->count;
            p->first = first;
            p->second = second;
            p->capacity = grown;
        }
    }
    // The stores go through an untyped path: the original reloads the array pointers and count after them.
    __builtin_memcpy(&p->second[n], &a, 8);
    __builtin_memcpy(&p->first[p->count], &b, 8);
    p->count = p->count + 1;
}

struct UnkNamed_ovb0244 {
    base::UnkStrIdMember id;
    u8 pad_0x8[0x658];
};

struct UnkNamedList_ovb0244 : base::UnkSList<UnkNamedList_ovb0244> {
    u32 field_0x30;
};

char* fn_7100d4b708(char* p0);

UnkNamed_ovb0244* fn_7100d475ec(UnkNamedList_ovb0244* self, base::UnkStrIdMember* id) {
    UnkNamed_ovb0244* obj = static_cast<UnkNamed_ovb0244*>(fn_71000839a0(0x660, self->field_0x30));
    fn_7100d4b708(reinterpret_cast<char*>(obj));
    if (obj)
        self->PushBack(reinterpret_cast<u64>(obj));
    obj->id = *id;
    return obj;
}

void fn_7100331c7c(void* self, const void* other);

struct UnkRec48_ovb0244 {
    u64 unk_0x0[9];
};

struct UnkPairRec_ovb0244 {
    UnkRefObject* field_0x0;
    UnkRefObject* field_0x8;
    UnkRec48_ovb0244 record;  // 0x10
    u32 field_0x58;
    u32 field_0x5c;
};

void fn_7100d4a740(UnkPairRec_ovb0244* self, UnkWeakTarget* first, UnkWeakTarget* second,
                   const UnkRec48_ovb0244* record, u32 a, u32 b) {
    UnkWeakHandle* h1 = GetWeakHandle(first);
    self->field_0x0 = nullptr;
    AssignRef(&self->field_0x0, static_cast<UnkRefObject*>(h1));
    UnkWeakHandle* h2 = GetWeakHandle(second);
    self->field_0x8 = nullptr;
    AssignRef(&self->field_0x8, static_cast<UnkRefObject*>(h2));
    fn_7100331c7c(&self->record, record);
    self->field_0x58 = a;
    self->field_0x5c = b;
}
