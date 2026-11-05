// Unit 1115 (tools/tu_map.py): the destructor of the class with vtable 0x7101a6b820: it removes the
// entries of the array at +0x68 one by one from the front (fn_71006f9148), frees plain arrays at +0x98
// and +0x68, a keyed table at +0x40, a keyed table of 0x30-byte records (each an array of arrays) at
// +0x18 and releases the string id at +8. Names are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a6b820[];
void fn_71006f9148(void* self, u32 index);  // removes the entry at index

namespace {

struct UnkU1115Tag_A4_b71;

template <class T>
struct UnkU1115Vec_A4_b71 : base::UnkVector<T> {
    __attribute__((always_inline)) void Release() {
        if (this->data) {
            for (u64 i = 0; i < this->count; ++i)
                this->data[i].~T();
            this->count = 0;
            this->allocator->vf_0x48(this->data);
            this->capacity = 0;
        }
    }
};

struct UnkU1115Inner_A4_b71 {
    UnkU1115Vec_A4_b71<u32> v;
    ~UnkU1115Inner_A4_b71() { v.Release(); }
};

struct UnkU1115Record_A4_b71 {
    UnkU1115Vec_A4_b71<UnkU1115Inner_A4_b71> v;
    u64 field_0x28;
};

template <class T>
struct UnkU1115Table_A4_b71 {
    T* data;
    void* buckets;
    u32 count;
    u32 field_0x14;
    u64 field_0x18;
    base::UnkAllocator* allocator;
};

template <u32 Offset>
struct UnkU1115PlainVec_A4_b71 {
    static void Release(char* self) { reinterpret_cast<base::UnkVector<u32>*>(self + Offset)->Free(); }
};

template <u32 Offset>
struct UnkU1115PlainTable_A4_b71 {
    static void Release(char* self) {
        auto* t = reinterpret_cast<UnkU1115Table_A4_b71<u32>*>(self + Offset);
        t->count = 0;
        if (t->buckets)
            t->allocator->vf_0x48(t->buckets);
        if (t->data)
            t->allocator->vf_0x48(t->data);
    }
};

template <u32 Offset>
struct UnkU1115RecordTable_A4_b71 {
    static void Release(char* self) {
        auto* t = reinterpret_cast<UnkU1115Table_A4_b71<UnkU1115Record_A4_b71>*>(self + Offset);
        if (t->data)
            for (u64 i = 0; i < t->count; ++i)
                t->data[i].v.Release();
        t->count = 0;
        if (t->buckets)
            t->allocator->vf_0x48(t->buckets);
        if (t->data)
            t->allocator->vf_0x48(t->data);
    }
};

struct UnkU1115Drain_A4_b71 {
    static void Release(char* self) {
        while (*reinterpret_cast<u32*>(self + 0x70) != 0)
            fn_71006f9148(self, 0);
    }
};

}  // namespace

template <>
struct base::UnkDtorTraits<UnkU1115Tag_A4_b71> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a6b820); }
};

__attribute__((flatten)) void fn_71006f8f24(char* self) {
    base::refl_DtorNoBase<UnkU1115Tag_A4_b71, UnkU1115Drain_A4_b71, UnkU1115PlainVec_A4_b71<0x98>,
                          UnkU1115PlainVec_A4_b71<0x68>, UnkU1115PlainTable_A4_b71<0x40>,
                          UnkU1115RecordTable_A4_b71<0x18>, base::UnkStrIdAt<8>>(self);
}
