// CPlayerInfo's unit (tools/tu_map.py unit 2592): a destructor of a class over the game object base
// (fn_7101043934), likely CPlayerInfo's (its vtable): frees its named entry table, its members and two inline fixed vectors.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CGameObject.h"
#include "game/CGunComponent.h"
#include "game/CPlayerInfo.h"
#include "game/UnkTU32.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

void fn_7100f110b0(void* member);

// An element destroyed through its first virtual slot (0x38 bytes).
struct UnkPlayerInfoElem_lf088c8 {
    virtual void vf_0x0();
    u8 pad_0x8[0x30];
};

// A reference-counted object released through its third virtual slot.
struct UnkPlayerInfoCounted_lf088c8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    u32 ref_count;
};

struct UnkPlayerInfoRef_lf088c8 {
    UnkPlayerInfoCounted_lf088c8* object;
    u64 field_0x8;
};

// An entry of the named table (0x30 bytes), destroyed by fn_7100f0e85c.
struct UnkPlayerInfoEntry_lf088c8 {
    u8 pad_0x0[0x30];
};

#pragma pack(push, 4)
struct UnkPlayerInfo_lf088c8 {
    char* vtable;
    u8 pad_0x8[0x8];
    u32 elem_count;                                   // 0x10
    UnkPlayerInfoElem_lf088c8 elems[2];               // 0x14
    u8 pad_0x84[0xc];
    u32 ref_count;                                    // 0x90
    UnkPlayerInfoRef_lf088c8 refs[11];                // 0x94
    u8 pad_0x144[0xc];
    u8 member_0x150[0x30];
    u8 member_0x180[0x38];
    u8 member_0x1b8[0x30];
    void* link;                                       // 0x1e8
    u64 field_0x1f0;
    u64 field_0x1f8;
    u8 ids_0x200[0x18];
    u8 ids_0x218[0x18];
    u8 ids_0x230[0x18];
    u8 ids_0x248[0x18];
    u8 pad_0x260[0x10];
    u8 member_0x270[0xb0];
    UnkPlayerInfoEntry_lf088c8* entries;              // 0x320
    UnkStringData** names;                            // 0x328
    u32 entry_count;                                  // 0x330
    u8 pad_0x334[0xc];
    base::UnkAllocator* allocator;                    // 0x340
    u8 member_0x348[0xb0];
};
#pragma pack(pop)
static_assert(__builtin_offsetof(UnkPlayerInfo_lf088c8, refs) == 0x94, "layout");
static_assert(__builtin_offsetof(UnkPlayerInfo_lf088c8, link) == 0x1e8, "layout");
static_assert(__builtin_offsetof(UnkPlayerInfo_lf088c8, entries) == 0x320, "layout");
static_assert(__builtin_offsetof(UnkPlayerInfo_lf088c8, member_0x348) == 0x348, "layout");

void fn_7100f088c8(UnkPlayerInfo_lf088c8* self) {
    self->vtable = g_unk_7101caa450 + 0x10;
    fn_710002f67c(self->member_0x348);
    if (self->names && self->entry_count) {
        for (u64 i = 0; i < self->entry_count; ++i) {
            UnkStringPool* pool = g_unk_7101d47108;
            if (!pool)
                continue;
            UnkStringData** names = self->names;
            UnkStringData* data = names[i];
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (names[i]->ref_count == 0) {
                    StringPool_Release(pool, names[i]);
                    pool = g_unk_7101d47108;
                }
            }
            names[i] = &pool->empty;
        }
    }
    if (self->entries) {
        for (u32 i = 0; i < self->entry_count; ++i)
            fn_7100f0e85c(reinterpret_cast<UnkTimedEntry_ovb0071*>(&self->entries[i]));
    }
    self->entry_count = 0;
    if (self->names)
        self->allocator->vf_0x48(self->names);
    if (self->entries)
        self->allocator->vf_0x48(self->entries);
    fn_710002f67c(self->member_0x270);
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(self->ids_0x248));
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(self->ids_0x230));
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(self->ids_0x218));
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(self->ids_0x200));
    if (self->link) {
        fn_7100840f50(self->link, &self->link);
        self->link = nullptr;
    }
    self->field_0x1f0 = 0;
    self->field_0x1f8 = 0;
    fn_7100f110b0(self->member_0x1b8);
    fn_7100f110b0(self->member_0x180);
    fn_7100f110b0(self->member_0x150);
    for (u32 i = 0; i < self->ref_count; ++i) {
        UnkPlayerInfoCounted_lf088c8* object = self->refs[i].object;
        if (!object)
            continue;
        u32& count = object->ref_count;
        if (--count == 0)
            object->vf_0x10();
    }
    self->ref_count = 0;
    for (u32 i = 0; i < self->elem_count; ++i)
        self->elems[i].vf_0x0();
    self->elem_count = 0;
    fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
}
