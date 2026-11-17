// sound::CMusicManager::CTunableMusicManager's unit (tools/tu_map.py): grows the slot array (+0xe0 of the
// manager) to at least the given capacity, copying each slot's node list into the new storage and
// destroying the old slot. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkPooledId.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/UnkMusicPathVec.h"
#include "types.h"

namespace {

// A music entry (0x60 bytes): settings, a file-path list and a string id.
struct UnkMusicEntry_lfde040 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u8 field_0x24;
    u8 field_0x25;
    u8 field_0x26;
    UnkMusicPathList field_0x28;
    u64 field_0x50;
    UnkPooledId<&g_unk_7101d47108> field_0x58;

    UnkMusicEntry_lfde040(const UnkMusicEntry_lfde040& o)
        : field_0x0(o.field_0x0), field_0x4(o.field_0x4), field_0x8(o.field_0x8), field_0xc(o.field_0xc),
          field_0x10(o.field_0x10), field_0x14(o.field_0x14), field_0x18(o.field_0x18),
          field_0x1c(o.field_0x1c), field_0x20(o.field_0x20), field_0x24(o.field_0x24),
          field_0x25(o.field_0x25), field_0x26(o.field_0x26), field_0x28(o.field_0x28),
          field_0x50(o.field_0x50), field_0x58(o.field_0x58) {}
};

// A node of a slot's list: the entry, then the links.
struct UnkMusicNode_lfde040 : UnkMusicEntry_lfde040 {
    UnkMusicNode_lfde040* next;  // 0x60
    UnkMusicNode_lfde040* prev;  // 0x68

    // Copies the entry, unlinked.
    explicit UnkMusicNode_lfde040(const UnkMusicEntry_lfde040& entry)
        : UnkMusicEntry_lfde040(entry), next(nullptr), prev(nullptr) {}
};
static_assert(sizeof(UnkMusicNode_lfde040) == 0x70, "UnkMusicNode_lfde040 layout");

// A slot (UnkMusicSlot_0x38) as its copy reads it: a node list whose vtable's first slot destroys it.
struct UnkMusicSlotList_lfde040 {
    char* vtable;
    u32 count;
    UnkMusicNode_lfde040* head;
    UnkMusicNode_lfde040* tail;
    u32 flags;
    base::UnkAllocator* allocator;
    void* field_0x30;

    // Appends a copy of `node`; nothing when the allocation fails.
    void PushBack(const UnkMusicEntry_lfde040& entry) {
        void* mem = allocator->vf_0x40(sizeof(UnkMusicNode_lfde040), 8, flags);
        if (!mem)
            return;
        UnkMusicNode_lfde040* n = new (mem) UnkMusicNode_lfde040(entry);
        if (count) {
            tail->next = n;
            n->prev = tail;
            tail = n;
            ++count;
        } else {
            head = n;
            tail = n;
            count = 1;
        }
    }

    // The copy: the base list's (its vtable, the nodes), then the slot's own.
    UnkMusicSlotList_lfde040(const UnkMusicSlotList_lfde040& other) {
        vtable = g_unk_7101cac270 + 0x10;
        count = 0;
        head = nullptr;
        tail = nullptr;
        flags = other.flags;
        allocator = other.allocator;
        for (UnkMusicNode_lfde040* n = other.head; n; n = n->next)
            PushBack(*n);
        vtable = g_unk_7101cac250 + 0x10;
        field_0x30 = other.field_0x30;
    }

    void Destroy() { (*reinterpret_cast<void (**)(UnkMusicSlotList_lfde040*)>(vtable))(this); }
};
static_assert(sizeof(UnkMusicSlotList_lfde040) == sizeof(UnkMusicSlot_0x38), "UnkMusicSlotList_lfde040 layout");

}  // namespace

void fn_7100fde040(base::global::CRntVector<UnkMusicSlot_0x38>* v, u32 n) {
    if (v->capacity < n) {
        UnkMusicSlotList_lfde040* p = static_cast<UnkMusicSlotList_lfde040*>(
            v->allocator->vf_0x40((u64)n * sizeof(UnkMusicSlotList_lfde040), 8, v->flags));
        if (v->data) {
            UnkMusicSlotList_lfde040* src = reinterpret_cast<UnkMusicSlotList_lfde040*>(v->data);
            UnkMusicSlotList_lfde040* dst = p;
            for (u32 i = 0; i < v->count; ++i, ++dst, ++src) {
                new (dst) UnkMusicSlotList_lfde040(*src);
                src->Destroy();
            }
            v->allocator->vf_0x48(v->data);
        }
        v->data = reinterpret_cast<UnkMusicSlot_0x38*>(p);
        v->capacity = n;
    }
}
