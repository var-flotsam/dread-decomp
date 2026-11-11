// Unit 2611: the destructor of the sub-area setup record. It resets the record (name, sub-object,
// the nine kind ids, the id vector, two masks), then destroys its members in reverse order.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

void fn_7100fa64bc(void* self);
void fn_7100ee1548(void* self);
void fn_7100fa597c(void* self);

struct UnkAllocator_lfaa3d8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(void* block);
};

template <typename T>
struct UnkArray_lfaa3d8 {
    T* data;
    u32 count;
    u32 capacity;
    u8 pad_0x10[0x10];
    UnkAllocator_lfaa3d8* allocator;

    void Clear() {
        for (u64 i = 0; i < count; ++i)
            data[i].Reset();
        count = 0;
    }
    void Free() {
        allocator->vf_0x48(data);
        capacity = 0;
    }
};

struct UnkSubareaStaticZero_lfaa3d8 {
    u32 value;
    UnkSubareaStaticZero_lfaa3d8() : value(0) {}
};

inline const UnkSubareaStaticZero_lfaa3d8& UnkSubareaStaticZero() {
    static UnkSubareaStaticZero_lfaa3d8 s_value asm("g_unk_7172719980");
    return s_value;
}

struct UnkKinds_lfaa3d8 {
    CStrId ids[9];                 // 0x108
    UnkArray_lfaa3d8<CStrId> list; // 0x150
    u32 field_0x70;                // 0x178
    u32 field_0x74;                // 0x17c

    __attribute__((always_inline)) void Clear() {
        ids[0] = g_unk_7101d470e0;
        ids[1] = g_unk_7101d470e0;
        ids[2] = g_unk_7101d470e0;
        ids[3] = g_unk_7101d470e0;
        ids[4] = g_unk_7101d470e0;
        ids[5] = g_unk_7101d470e0;
        ids[6] = g_unk_7101d470e0;
        ids[7] = g_unk_7101d470e0;
        ids[8] = g_unk_7101d470e0;
        list.Clear();
        field_0x70 = UnkSubareaStaticZero().value;
        field_0x74 = UnkSubareaStaticZero().value;
    }
    void Destroy() {
        if (list.data) {
            list.Clear();
            list.Free();
        }
        fn_7100ee1548(this);
    }
};

struct UnkSubAreaRecord_0x188 {
    CStrId name;                 // 0x0
    void* field_0x8;
    void* data;                  // 0x10
    u32 count;                   // 0x18
    u32 capacity;                // 0x1c
    u8 pad_0x20[0x10];
    UnkAllocator_lfaa3d8* allocator;  // 0x30
    u8 sub[0xd0];                // 0x38
    UnkKinds_lfaa3d8 kinds;      // 0x108
};
static_assert(__builtin_offsetof(UnkSubAreaRecord_0x188, kinds) == 0x108);
static_assert(__builtin_offsetof(UnkSubAreaRecord_0x188, kinds.field_0x70) == 0x178);

void fn_7100faa3d8(UnkSubAreaRecord_0x188* self) {
    self->name = g_unk_7101d470e0;
    self->field_0x8 = nullptr;
    self->count = 0;
    fn_7100fa64bc(self->sub);
    self->kinds.Clear();

    self->kinds.Destroy();
    fn_7100fa597c(self->sub);
    if (self->data) {
        self->count = 0;
        self->allocator->vf_0x48(self->data);
        self->capacity = 0;
    }
    self->name.Reset();
}
