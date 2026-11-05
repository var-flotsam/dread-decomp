// Unit 94 of tools/tu_map.py (no class hint): move constructors and destructors (reflection slots
// 0x38 and 0x40) of three reflected records built on one another. The base (0x80 bytes) holds two
// reflected values (base::reflection::UnkReflValue, moved with fn_71000a179c and destroyed with
// fn_71000a1c60) between plain words and an array of reflected values (+0x48); the first derived
// record adds a word and an owned polymorphic object (+0x88), the second two words, an owned object
// (+0x90), a flag, another value (+0xa0) and two words. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

void fn_71000a179c(UnkReflValue* self, const UnkReflValue* other);  // copy constructor

struct UnkReflValueElem_b58 {
    UnkReflValue value;

    UnkReflValueElem_b58(UnkReflValueElem_b58&& other) { fn_71000a179c(&value, &other.value); }
    ~UnkReflValueElem_b58() { fn_71000a1c60(&value); }
};
static_assert(sizeof(UnkReflValueElem_b58) == 0x18, "UnkReflValueElem_b58 layout");

// The array (base::UnkVector's layout), moved by taking the other's (left empty).
struct UnkReflValueArray_b58 {
    UnkReflValueElem_b58* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkReflValueArray_b58(UnkReflValueArray_b58&& other) {
        data = other.data;
        count = other.count;
        capacity = other.capacity;
        step = other.step;
        field_0x14 = other.field_0x14;
        flags = other.flags;
        allocator = other.allocator;
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    ~UnkReflValueArray_b58() { reinterpret_cast<base::UnkVector<UnkReflValueElem_b58>*>(this)->Free(); }
};

// An owned polymorphic object, released through its deleting destructor (vtable slot 1).
struct UnkOwnedObject_b58 {
    virtual void vf_0x0();
    virtual void vf_0x8();
};
struct UnkOwnedPtr_b58 {
    UnkOwnedObject_b58* ptr;

    UnkOwnedPtr_b58(UnkOwnedPtr_b58&& other) {
        UnkOwnedObject_b58* p = other.ptr;
        other.ptr = nullptr;
        ptr = p;
    }
    ~UnkOwnedPtr_b58() {
        UnkOwnedObject_b58* p = ptr;
        ptr = nullptr;
        if (p)
            p->vf_0x8();
    }
};

struct UnkReflRecord_b58 {
    u64 field_0x0;
    UnkReflValueElem_b58 field_0x8;
    u64 field_0x20;
    UnkReflValueElem_b58 field_0x28;
    u64 field_0x40;
    UnkReflValueArray_b58 field_0x48;
    u32 field_0x70;
    u64 field_0x78;

    UnkReflRecord_b58(UnkReflRecord_b58&& o)
        : field_0x0(o.field_0x0), field_0x8(static_cast<UnkReflValueElem_b58&&>(o.field_0x8)),
          field_0x20(o.field_0x20), field_0x28(static_cast<UnkReflValueElem_b58&&>(o.field_0x28)),
          field_0x40(o.field_0x40), field_0x48(static_cast<UnkReflValueArray_b58&&>(o.field_0x48)),
          field_0x70(o.field_0x70), field_0x78(o.field_0x78) {}
};
static_assert(sizeof(UnkReflRecord_b58) == 0x80, "UnkReflRecord_b58 layout");

struct UnkReflRecordA_b58 : UnkReflRecord_b58 {
    u64 field_0x80;
    UnkOwnedPtr_b58 field_0x88;

    UnkReflRecordA_b58(UnkReflRecordA_b58&&) = default;
};
static_assert(sizeof(UnkReflRecordA_b58) == 0x90, "UnkReflRecordA_b58 layout");

struct UnkReflRecordB_b58 : UnkReflRecord_b58 {
    u64 field_0x80;
    u64 field_0x88;
    UnkOwnedPtr_b58 field_0x90;
    bool field_0x98;
    UnkReflValueElem_b58 field_0xa0;
    u64 field_0xb8;
    u64 field_0xc0;

    UnkReflRecordB_b58(UnkReflRecordB_b58&&) = default;
};
static_assert(sizeof(UnkReflRecordB_b58) == 0xc8, "UnkReflRecordB_b58 layout");

template <class T>
__attribute__((always_inline)) inline void MoveConstruct(void* dst, const void* src) {
    new (dst) T(static_cast<T&&>(*static_cast<T*>(const_cast<void*>(src))));
}

template <class T>
__attribute__((always_inline)) inline void Destroy(void* obj) {
    static_cast<T*>(obj)->~T();
}

void fn_71000f2bf0(void* dst, const void* src) {
    MoveConstruct<UnkReflRecord_b58>(dst, src);
}
void fn_71000f2c84(void* obj) {
    Destroy<UnkReflRecord_b58>(obj);
}
void fn_71000f2d3c(void* dst, const void* src) {
    MoveConstruct<UnkReflRecordA_b58>(dst, src);
}
void fn_71000f2de4(void* obj) {
    Destroy<UnkReflRecordA_b58>(obj);
}
void fn_71000f2eb4(void* dst, const void* src) {
    MoveConstruct<UnkReflRecordB_b58>(dst, src);
}
void fn_71000f2f78(void* obj) {
    Destroy<UnkReflRecordB_b58>(obj);
}
