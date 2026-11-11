// Unit 243: an object with per-slot owned buffers and a counted resource that selects the loaded
// decal. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU174.h"
#include "game/UnkTU182.h"
#include "game/UnkTU211.h"
#include "game/UnkTU221.h"
#include "game/UnkTU228.h"
#include "game/UnkTU239.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkDecalResource;
struct UnkDecalResourceMgr;
struct UnkCountedObject259;
UnkDecalResource* fn_710019ffe8(UnkDecalResourceMgr* mgr, const CFilePathStrId* path, bool flag);
void fn_71001a8d38(void* target, u32 index, void* value, u32 flag);

struct UnkLoaded_b260 {
    u8 unk_0x0[0x20];
    CStrId name;  // 0x20
    u8 unk_0x28[0x368];
    void* field_0x390;
};

struct UnkResource_b260 {
    u8 unk_0x0[0x10];
    CFilePathStrId path;  // 0x10
    u8 unk_0x18[0x3c];
    union {
        s32 ref_count;  // 0x54
    };
    u8 unk_0x58[0x20];
    s32 kind;  // 0x78
};

struct UnkHolder_b260 {
    u8 unk_0x0[0x78];
    UnkResource_b260* resource;  // 0x78
    u8 unk_0x80[0x128];
    UnkLoaded_b260* loaded;      // 0x1a8
    u8 unk_0x1b0[8];
    UnkLoaded_b260* loaded_copy; // 0x1b8
    u8 unk_0x1c0[0xe0];
    void* field_0x2a0;
    u8 unk_0x2a8[0x38];
    void* field_0x2e0;
    CStrId name;                 // 0x2e8
    u8 unk_0x2f0[0x85];
    u8 flag;                     // 0x375
};
static_assert(__builtin_offsetof(UnkHolder_b260, flag) == 0x375, "layout");

void fn_71001b5178(UnkHolder_b260* self, UnkResource_b260* res) {
    if (self->resource != res) {
        if (res)
            ++res->ref_count;
        UnkResource_b260* old = self->resource;
        if (old && --old->ref_count == 0)
            fn_710001737c(reinterpret_cast<UnkCountedResource*>(old));
        self->resource = res;
    }
    if (!res) {
        if (!self->loaded)
            return;
        fn_71001a080c(g_unk_716e352450, reinterpret_cast<UnkDecalResource*>(self->loaded));
        self->loaded = nullptr;
        self->flag = 0;
        return;
    }
    if (res->kind == 2) {
        self->field_0x2a0 = fn_71001b363c(reinterpret_cast<UnkCountedObject259*>(res));
        if (self->loaded)
            fn_71001a8d38(self->loaded, 0, &self->field_0x2a0, 1);
        self->flag = 0;
        return;
    }
    const CFilePathStrId* path = &res->path;
    UnkLoaded_b260* old = self->loaded;
    UnkLoaded_b260* loaded = reinterpret_cast<UnkLoaded_b260*>(fn_710019ffe8(g_unk_716e352450, path, true));
    self->loaded = loaded;
    self->loaded_copy = loaded;
    self->field_0x2e0 = loaded->field_0x390;
    self->name = loaded->name;
    if (old)
        fn_71001a080c(g_unk_716e352450, reinterpret_cast<UnkDecalResource*>(old));
    self->flag = 1;
}


struct UnkObj_71001541a0;
struct UnkStruct_710019ab0c;
extern s32 g_unk_716e5546f0;

struct UnkSlot_b260 {
    void* field_0x0;       // 0x1e0
    void* field_0x8;       // 0x1e8
    u8* buffers[7];        // 0x1f0
    u8 unk_0x28[4];
    u8 active;             // 0x4c in the slot
    u8 unk_0x4d[3];
};
static_assert(sizeof(UnkSlot_b260) == 0x50, "slot");

struct UnkSlotOwner_b260 {
    u8 unk_0x0[0x80];
    u8 field_0x80[8];
    u64 field_0x88;
    u8 unk_0x90[0x150];
    UnkSlot_b260 slots[1];  // 0x1e0
};

void fn_71001b5018(UnkSlotOwner_b260* self, u32 index) {
    UnkSlot_b260* slot = &self->slots[index];
    if (slot->active) {
        for (int i = 0; i < 7; i++) {
            if (slot->buffers[i]) {
                delete[] slot->buffers[i];
                slot->buffers[i] = nullptr;
            }
        }
        if (slot->field_0x0) {
            fn_7100161b90(slot->field_0x0);
            slot->field_0x0 = nullptr;
        }
        if (slot->field_0x8) {
            fn_71001541a0(reinterpret_cast<UnkObj_71001541a0*>(slot->field_0x8));
            slot->field_0x8 = nullptr;
        }
        slot->active = 0;
        --g_unk_716e5546f0;
    }
    self->field_0x88 = 0;
    fn_710019ab0c(reinterpret_cast<UnkStruct_710019ab0c*>(self->field_0x80));
    *reinterpret_cast<u64*>(self->field_0x80) = 0;
}
