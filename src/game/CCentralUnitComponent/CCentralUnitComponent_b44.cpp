// A constructor in the CCentralUnitComponent unit (tools/tu_map.py unit 1176): two adjacent
// 0x40-byte objects of the shape fn_71004076e4 builds at +0x14750 (each stores its base vtable,
// clears its array, capacity 10, flags 0xf, the default allocator, then its own vtable).
// The fields are addressed by offset, as in src/game/CCentralUnitAIComponent/fn_71004076e4.cpp.
// Names and layout are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

extern void* g_unk_7101a20998[];  // the objects' base vtable
extern void* g_unk_7101a20950[];  // their vtable

struct UnkCentralUnitPair_b44a2 {
    u8 unk_0x0;
};

template <class T>
static inline T& At(void* base, u64 offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
}

void fn_710077b454(UnkCentralUnitPair_b44a2* self) {
    __builtin_memset(&At<u64>(self, 0x10), 0, 0x70);
    void** vtable_a = g_unk_7101a20998 + 2;
    At<void**>(self, 0x0) = vtable_a;
    At<u64>(self, 0x8) = 0;
    At<u64>(self, 0x10) = 0;
    At<u32>(self, 0x18) = 10;
    At<u8>(self, 0x1c) = 1;
    At<u32>(self, 0x20) = 0xf;
    base::UnkAllocator* alloc_a = fn_7100083990();
    void** vtable_b = g_unk_7101a20950 + 2;
    At<void**>(self, 0x0) = vtable_b;
    At<base::UnkAllocator*>(self, 0x28) = alloc_a;
    At<u32>(self, 0x30) = 0;
    At<u64>(self, 0x38) = 0;
    At<void**>(self, 0x40) = vtable_a;
    At<u64>(self, 0x48) = 0;
    At<u64>(self, 0x50) = 0;
    At<u32>(self, 0x58) = 10;
    At<u8>(self, 0x5c) = 1;
    At<u32>(self, 0x60) = 0xf;
    base::UnkAllocator* alloc_b = fn_7100083990();
    At<base::UnkAllocator*>(self, 0x68) = alloc_b;
    At<u32>(self, 0x70) = 0;
    At<void**>(self, 0x40) = vtable_b;
    At<u64>(self, 0x78) = 0;
}

// Stores the owner (+0x18) and passes a copy of its string id (+0x30) to the member at +0x790
// (fn_710077018c).
struct UnkCentralUnitOwner_b44a2 {
    u8 pad_0x0[0x30];
    base::global::CStrId field_0x30;
};

struct UnkCentralUnitMember_b44a2;
void fn_710077018c(UnkCentralUnitMember_b44a2* self, base::global::CStrId* id);

struct UnkCentralUnitComponent_b44a2 {
    u8 pad_0x0[0x18];
    UnkCentralUnitOwner_b44a2* owner;
    u8 pad_0x20[0x770];
    UnkCentralUnitMember_b44a2* member() { return reinterpret_cast<UnkCentralUnitMember_b44a2*>(pad_0x20 + 0x770); }
};

void fn_71007700fc(UnkCentralUnitComponent_b44a2* self, UnkCentralUnitOwner_b44a2* owner) {
    self->owner = owner;
    UnkCentralUnitMember_b44a2* member = self->member();
    base::global::CStrId id = owner->field_0x30;
    fn_710077018c(member, &id);
}
