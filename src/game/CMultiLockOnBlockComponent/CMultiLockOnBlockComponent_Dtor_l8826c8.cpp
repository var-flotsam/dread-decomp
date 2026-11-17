// CMultiLockOnBlockComponent's unit (tools/tu_map.py unit 1339): a destructor (fn_71008826c8). The dictionary
// at +0xd0, the array of 0x40-byte polymorphic entries at +0xa8, the string-id table at +0x80, the name at
// +0x78 and the listener at +0x50, then the base component's destructor. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkStrIdTable.h"
#include "base/global/CRntDictionary.h"
#include "base/global/CRntVector.h"
#include "game/CPullOffGrapplePointComponent.h"
#include "game/CTimelineComponent.h"
#include "game/UnkComponent.h"
#include "types.h"

extern char g_unk_7101a7b9c8[];

struct UnkMultiLockLink_l8826c8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;           // 0x8
    void* owner;             // 0x10
};

// A listener on a counted link; it detaches itself from the link's owner on destruction.
struct UnkMultiLockListener_l8826c8 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkMultiLockLink_l8826c8* link;  // 0x18

    ~UnkMultiLockListener_l8826c8() {
        vtable = &g_unk_7101a77000[2];
        if (UnkMultiLockLink_l8826c8* l = link) {
            bool held = true;
            if (void* owner = l->owner) {
                fn_7100a2d4d0(owner, this);
                l = link;
                held = l != nullptr;
            }
            if (held && --l->ref_count == 0)
                l->Release();
        }
    }
};

struct UnkMultiLockTable_l8826c8 : UnkStrIdTable {
    ~UnkMultiLockTable_l8826c8() {
        fn_71004f11f8(this);
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

// An entry of the array at +0xa8, destroyed through its vtable.
struct UnkMultiLockEntry_l8826c8 {
    virtual ~UnkMultiLockEntry_l8826c8();
    u8 pad_0x8[0x38];
};

// A dictionary value: plain data.
struct UnkMultiLockValue_l8826c8 {
    u64 words[4];
};

struct UnkMultiLockDict_l8826c8 : base::global::CRntDictionary<u64, UnkMultiLockValue_l8826c8> {
    ~UnkMultiLockDict_l8826c8() {
        Clear();
        buckets.Free();
    }
};

struct UnkMultiLockComponent_l8826c8 {
    char* vtable;
    u8 pad_0x8[0x48];
    UnkMultiLockListener_l8826c8 listener;  // 0x50
    u8 pad_0x70[0x8];
    base::UnkStrIdMember name;  // 0x78
    UnkMultiLockTable_l8826c8 table;  // 0x80
    base::global::CRntVector<UnkMultiLockEntry_l8826c8> entries;  // 0xa8
    UnkMultiLockDict_l8826c8 dict;  // 0xd0
};
static_assert(__builtin_offsetof(UnkMultiLockComponent_l8826c8, entries) == 0xa8, "layout");
static_assert(__builtin_offsetof(UnkMultiLockComponent_l8826c8, dict) == 0xd0, "layout");

void fn_71008826c8(UnkMultiLockComponent_l8826c8* self) {
    self->vtable = g_unk_7101a7b9c8 + 0x10;
    self->dict.~UnkMultiLockDict_l8826c8();
    self->entries.Free();
    self->table.~UnkMultiLockTable_l8826c8();
    using base::UnkStrIdMember;
    self->name.~UnkStrIdMember();
    self->listener.~UnkMultiLockListener_l8826c8();
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
