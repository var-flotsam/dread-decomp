// A path query (tools/tu_map.py unit 1130, no class name). fn_7100706818: its constructor; keeps the
// owner (+0x10), builds the three paths (+0x30, +0x12ab8, +0x25540, fn_71002acfe4) and two embedded
// listeners at +0x37ff8 and +0x38020 that point back at the query. Names and layouts are
// placeholders.
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

extern void* g_unk_7101a6bfd8[];
extern void* g_unk_7101a6bfa0[];
extern void* g_unk_7101a6bf68[];

struct UnkSelf_nq41;

struct UnkListenerA_nq41 {
    void** vtable;
    f32 field_0x8;
    u8 pad_0xc[0xc];
    UnkSelf_nq41* owner;  // 0x18
    u8 field_0x20;
};

struct UnkListenerB_nq41 {
    void** vtable;
    f32 field_0x8;
    u8 pad_0xc[0xc];
    UnkSelf_nq41* owner;  // 0x18
};

struct UnkSelf_nq41 {
    void** vtable;
    u64 field_0x8;
    void* owner;  // 0x10
    u64 field_0x18;
    u64 field_0x20;
    u64 field_0x28;
    u8 path_0x30[0x12a88];
    u8 path_0x12ab8[0x12a88];
    u8 path_0x25540[0x12a88];
    s32 field_0x37fc8;
    u32 field_0x37fcc;
    u32 field_0x37fd0;
    u32 field_0x37fd4;
    u32 field_0x37fd8;
    u8 field_0x37fdc;
    u8 pad_0x37fdd[3];
    u64 field_0x37fe0;
    u64 field_0x37fe8;
    u64 field_0x37ff0;
    UnkListenerA_nq41 listener_a;  // 0x37ff8
    UnkListenerB_nq41 listener_b;  // 0x38020
};

void fn_7100706818(UnkSelf_nq41* self, void* owner) {
    self->field_0x8 = 0;
    self->owner = owner;
    self->vtable = &g_unk_7101a6bfd8[2];
    self->field_0x18 = 0;
    self->field_0x20 = 0;
    self->field_0x28 = 0;
    fn_71002acfe4(self->path_0x30);
    fn_71002acfe4(self->path_0x12ab8);
    fn_71002acfe4(self->path_0x25540);
    self->field_0x37fc8 = -1;
    self->field_0x37fcc = 0;
    self->field_0x37fd0 = 0;
    self->field_0x37fd4 = 0;
    self->field_0x37fd8 = 0;
    self->field_0x37fdc = 1;
    self->field_0x37fe0 = 0;
    self->field_0x37fe8 = 0;
    self->field_0x37ff0 = 0;
    self->listener_a.field_0x8 = 0.5f;
    self->listener_a.vtable = &g_unk_7101a6bfa0[2];
    self->listener_a.field_0x20 = 0;
    self->listener_b.field_0x8 = 0.5f;
    self->listener_b.vtable = &g_unk_7101a6bf68[2];
    self->listener_a.owner = self;
    self->listener_b.owner = self;
}
