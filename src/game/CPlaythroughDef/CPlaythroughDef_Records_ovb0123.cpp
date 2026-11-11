// CPlaythroughDef's unit (tools/tu_map.py): the destructor of the holder of two string ids (+0x58,
// +0x60) and a list of owned objects (+0x68) that fn_71002f458c constructs, and the copy assignment
// of a record of three string ids (+0x8, +0x10, +0xa0), a string (+0x18, fn_710007eaa0) and an array
// (+0xa8, fn_71000ce19c). Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/UnkVector.h"
#include "base/core/CAsset.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU83.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

extern void* g_unk_7101a22240[];  // the holder's vtable

struct UnkIdListHolder_ovb0123 {
    void** vtable;
    u8 pad_0x8[0x50];
    CStrId field_0x58;
    CStrId field_0x60;
    UnkOwnedList field_0x68;
};

void fn_71002f2d80(UnkIdListHolder_ovb0123* self) {
    self->vtable = g_unk_7101a22240 + 2;
    self->field_0x68.~UnkOwnedList();
    self->field_0x60.Reset();
    self->field_0x58.Reset();
    fn_7100016f88(self);
}

struct UnkStruct_0x28;

struct UnkPlaythroughRecord_ovb0123 {
    void** vtable;
    CStrId field_0x8;
    CStrId field_0x10;
    CRntString field_0x18;
    u8 pad_0x48[0x58];
    CStrId field_0xa0;
    u8 field_0xa8[0x28];
};
static_assert(__builtin_offsetof(UnkPlaythroughRecord_ovb0123, field_0xa0) == 0xa0, "layout");

UnkPlaythroughRecord_ovb0123* fn_71002f4df0(UnkPlaythroughRecord_ovb0123* self,
                                            const UnkPlaythroughRecord_ovb0123* other) {
    self->field_0x8 = other->field_0x8;
    self->field_0x10 = other->field_0x10;
    fn_710007eaa0(&self->field_0x18, &other->field_0x18);
    self->field_0xa0 = other->field_0xa0;
    fn_71000ce19c(reinterpret_cast<UnkStruct_0x28*>(self->field_0xa8),
                  reinterpret_cast<const UnkStruct_0x28*>(other->field_0xa8));
    return self;
}

// The record fn_71002f4820 constructs: two string ids, a string with inline storage (+0x18, its
// storage object at +0x48), a third id (+0xa0) and an array of ids (+0xa8).
struct UnkPlaythroughStorage_ovb0123 {
    void** vtable;
    u8 pad_0x8[0x40];
    u64 field_0x48;
    u32 field_0x50;
};

extern void* g_unk_7101a1b678[];  // the storage's vtable

// A string whose storage object follows it.
struct UnkPlaythroughText_ovb0123 {
    u8 raw[0x30];                           // the string
    UnkPlaythroughStorage_ovb0123 storage;  // +0x30

    CRntString* Str() { return reinterpret_cast<CRntString*>(raw); }
    const CRntString* Str() const { return reinterpret_cast<const CRntString*>(raw); }

    UnkPlaythroughText_ovb0123() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage));
        storage.vtable = g_unk_7101a1b678 + 2;
        storage.field_0x48 = 0;
        storage.field_0x50 = 0;
    }
    UnkPlaythroughText_ovb0123(const UnkPlaythroughText_ovb0123& other) : UnkPlaythroughText_ovb0123() {
        fn_710007eaa0(Str(), other.Str());
    }
    ~UnkPlaythroughText_ovb0123() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

struct UnkPlaythroughNode_ovb0123 {
    void** vtable;
    CStrId field_0x8;
    CStrId field_0x10;
    UnkPlaythroughText_ovb0123 text;  // 0x18
    CStrId field_0xa0;
    base::UnkStrIdVec ids;  // 0xa8
};
static_assert(__builtin_offsetof(UnkPlaythroughNode_ovb0123, ids) == 0xa8, "layout");

extern void* g_unk_7101a22280[];  // the record's vtable

// The record's copy constructor.
void fn_71002f4c38(UnkPlaythroughNode_ovb0123* self, const UnkPlaythroughNode_ovb0123* other) {
    self->vtable = g_unk_7101a22280 + 2;
    new (&self->field_0x8) CStrId(other->field_0x8);
    new (&self->field_0x10) CStrId(other->field_0x10);
    new (&self->text) UnkPlaythroughText_ovb0123(other->text);
    new (&self->field_0xa0) CStrId(other->field_0xa0);
    new (&self->ids) base::UnkStrIdVec(other->ids);
}

// The record's destructor.
void fn_71002f4a5c(UnkPlaythroughNode_ovb0123* self) {
    self->vtable = g_unk_7101a22280 + 2;
    if (self->ids.data) {
        for (u64 i = 0; i < self->ids.count; ++i)
            self->ids.data[i].Reset();
        self->ids.count = 0;
        self->ids.allocator->vf_0x48(self->ids.data);
        self->ids.capacity = 0;
    }
    self->field_0xa0.Reset();
    fn_710007ed40(self->text.Str());
    fn_710007ecd4(self->text.Str());
    self->field_0x10.Reset();
    self->field_0x8.Reset();
}
