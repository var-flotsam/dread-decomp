// Unit 372 (no reliable name): a navmesh item's key list reporting and its reader and destructor. Names and layouts are placeholders.
#include "game/UnkGuiRoot.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkTU372.h"
#include "game/UnkTU39.h"
#include "types.h"
#include "base/UnkVector.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "game_s2/UnkWeakHandle.h"

u16* fn_71000d1d24(void* vec, u32 index);
void fn_71002af93c(void* cell, bool flag);
void fn_710027daa4(void* mesh, const void* arg, bool first);

struct UnkOwner;

struct UnkKeyedOwner_b0246 {
    UnkNavMesh* mesh;  // 0x0
    u8 pad_0x8[0x10];
    u8 keys[0x10];  // 0x18
    u8 pad_0x28[0xa0];
    u8 cells[0x8];  // 0xc8
    u32 cell_count;  // 0xd0
};

void fn_71002b3aa0(UnkOwner* owner, bool flag, void* arg) {
    UnkKeyedOwner_b0246* self = reinterpret_cast<UnkKeyedOwner_b0246*>(owner);
    UnkGuiKeyList* list = reinterpret_cast<UnkGuiKeyList*>(self->keys);
    bool before = fn_710002fb70(list);
    base::global::CStrId* key = static_cast<base::global::CStrId*>(arg);
    if (flag)
        fn_710002ffb4(list, key);
    else
        fn_710002fd10(list, key);
    bool after = fn_710002fb70(list);
    if (before == after)
        return;
    bool value = !after;
    bool any = self->cell_count != 0;
    UnkNavMesh* mesh = self->mesh;
    if (any) {
        u32 i = 0;
        do {
            fn_71002af93c(fn_710027a33c(mesh, *fn_71000d1d24(self->cells, i)), value);
            i++;
            mesh = self->mesh;
        } while (i < self->cell_count);
    }
    fn_710027daa4(mesh, self, value);
}

u16* fn_71000d1d24(void* vec, u32 index);

struct UnkNavMeshItem;

struct UnkNavMeshReadItem_b0246 {
    u8 pad_0x0[0xc8];
    base::UnkVector<u16> cells;  // 0xc8
};

void fn_71002b3524(UnkNavMeshItem* item, base::global::CRntFile* file) {
    UnkNavMeshReadItem_b0246* self = reinterpret_cast<UnkNavMeshReadItem_b0246*>(item);
    u32 count = 0;
    fn_710007c714(file, &count, 4, 1);
    self->cells.Reserve(count);
    for (s32 i = 0; i < (s32)count; i++) {
        u32 value;
        fn_710007c714(file, &value, 4, 1);
        *fn_71000d1d24(&self->cells, i) = (u16)value;
    }
}

using base::global::CStrId;

struct UnkNavMeshItem;
struct UnkLinkList;

struct UnkNavLoadLink;
void fn_71002b52a4(UnkNavLoadLink*);

struct UnkDtorLink_b0246 {
    u8 unk_0x0[0x40];
    ~UnkDtorLink_b0246() { fn_71002b52a4(reinterpret_cast<UnkNavLoadLink*>(this)); }
};

struct UnkDtorEntry_b0246 {
    CStrId key;
    UnkWeakHandle* handle;
    u32 value;
};

struct UnkDtorItem_b0246 {
    u64 field_0x0;
    CStrId id;  // 0x8
    s32 field_0x10;
    u8 pad_0x14[4];
    u8 field_0x18[0xb0];
    base::UnkVector<u16> cells;  // 0xc8
    base::UnkVector<UnkDtorLink_b0246> links;  // 0xf0
    u32 field_0x118;
    u32 field_0x11c;
    u32 field_0x120;
    u32 count;  // 0x124
    UnkDtorEntry_b0246 entries[8];  // 0x128
    u16 field_0x1e8;
    u8 field_0x1ea;
    u8 pad_0x1eb;
    u16 field_0x1ec;
    u8 field_0x1ee;
    u8 pad_0x1ef;
    UnkLinkList* list;  // 0x1f0
    void* callback_object;  // 0x1f8
    void* callback;  // 0x200
};

void fn_71002b3328(UnkNavMeshItem* item) {
    UnkDtorItem_b0246* self = reinterpret_cast<UnkDtorItem_b0246*>(item);
    UnkLinkList** slot = &self->list;
    if (*slot) {
        fn_71002b46ec(*slot, slot);
        *slot = nullptr;
    }
    self->field_0x1ec = 0;
    self->field_0x1ee = 0xff;
    fn_710002fa90(self->field_0x18);
    if (self->list) {
        fn_71002b46ec(self->list, slot);
        *slot = nullptr;
    }
    self->callback_object = nullptr;
    self->callback = nullptr;
    for (u64 i = 0; i < self->count; i++) {
        UnkWeakHandle* h = self->entries[i].handle;
        if (h && --h->ref_count == 0)
            h->vf_0x10();
        self->entries[i].key.Reset();
    }
    self->count = 0;
    self->links.Free();
    self->cells.Free();
    fn_710002f67c(self->field_0x18);
    self->id.Reset();
}
