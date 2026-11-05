// CXParasiteAIComponent's unit (tools/tu_map.py unit 1102): a component destructor (vtable 0x7101a6a1c8):
// refcounted pointers at +0x147b0 and +0x147a0..+0x14790, a keyed table of 0x38-byte records (each with a
// file-path id at +0x10) at +0x14768, an array of refcounted pointers at +0x14740, a refcounted pointer at
// +0x146d0, then the base's destructor fn_7100369380. Names are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a6a1c8[];
void fn_7100369380(void*);  // the base's destructor

struct UnkXParasiteDtor_A4_b71;

template <>
struct base::UnkDtorTraits<UnkXParasiteDtor_A4_b71> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a6a1c8); }
    static void base(char* self) { fn_7100369380(self); }
};

// The arrays of refcounted pointers.
template <u32 Offset>
struct UnkXParasiteRefs_A4_b71 {
    static void Release(char* self) {
        auto* v = reinterpret_cast<base::UnkVector<base::UnkRefPtrMember>*>(self + Offset);
        if (v->data) {
            for (u64 i = 0; i < v->count; ++i) {
                if (char* p = reinterpret_cast<char*>(v->data[i].ptr)) {
                    s32* rc = reinterpret_cast<s32*>(p + 8);
                    if (--*rc == 0)
                        delete v->data[i].ptr;
                }
            }
            v->count = 0;
            v->allocator->vf_0x48(v->data);
            v->capacity = 0;
        }
    }
};

struct UnkXParasiteRecord_A4_b71 {
    u8 unk_0x0[0x10];
    base::UnkStrIdMemberOf<g_unk_7101d47200> id;
    u8 unk_0x18[0x20];
};

struct UnkXParasiteTable_A4_b71 {
    UnkXParasiteRecord_A4_b71* data;
    void* buckets;
    u32 count;
    u32 field_0x14;
    u64 field_0x18;
    base::UnkAllocator* allocator;
};

template <u32 Offset>
struct UnkXParasiteTableRel_A4_b71 {
    static void Release(char* self) {
        auto* t = reinterpret_cast<UnkXParasiteTable_A4_b71*>(self + Offset);
        if (t->data)
            for (u64 i = 0; i < t->count; ++i)
                t->data[i].id.~UnkStrIdMemberOf();
        t->count = 0;
        if (t->buckets)
            t->allocator->vf_0x48(t->buckets);
        if (t->data)
            t->allocator->vf_0x48(t->data);
    }
};

__attribute__((flatten)) void fn_71006e6184(char* self) {
    base::refl_Dtor<UnkXParasiteDtor_A4_b71, base::UnkRefPtrAt<0x147b0>, base::UnkRefPtrAt<0x147a0>,
                    base::UnkRefPtrAt<0x14798>, base::UnkRefPtrAt<0x14790>,
                    UnkXParasiteTableRel_A4_b71<0x14768>, UnkXParasiteRefs_A4_b71<0x14740>,
                    base::UnkRefPtrAt<0x146d0>>(self);
}
