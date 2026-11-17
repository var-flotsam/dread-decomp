// Unit 2636: the destructor of the 0x150-byte entry fn_7100fd10cc constructs: its two blocks of settings (each a
// string id and an array of path records), its two arrays, then the base's reflected value. Names and layouts
// are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU2636.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntVector;
using base::global::CStrId;

extern void* g_unk_7101cac148[];

// The entry as its owner's destructor (fn_7100fd0264) declares it; its layout is UnkEntry_lfd11e8 below.
struct UnkEntry_0x150 {
    u64 data[0x150 / 8];
    ~UnkEntry_0x150() asm("fn_7100fd11e8");  // fn_7100fd11e8
};

struct UnkPathRecord_lfd11e8 {
    CFilePathStrId path;
    u8 pad_0x8[0x10];
};

struct UnkSettingsBlock_lfd11e8 {
    u8 pad_0x0[0x28];
    CRntVector<UnkPathRecord_lfd11e8> records;  // 0x28
    void* field_0x50;
    CStrId id;  // 0x58
};

struct UnkEntryBase_lfd11e8 {
    void** vtable;
    void* field_0x8;
    s32 kind;
    base::reflection::UnkReflValue value;  // 0x18
};

struct UnkEntry_lfd11e8 : UnkEntryBase_lfd11e8 {
    bool field_0x30;
    CRntVector<void*> field_0x38;
    CRntVector<void*> field_0x60;
    UnkSettingsBlock_lfd11e8 blocks[2];  // 0x88
};
static_assert(__builtin_offsetof(UnkEntry_lfd11e8, blocks) == 0x88, "layout");

static inline void DestroyBlock(UnkSettingsBlock_lfd11e8* block) {
    block->id.~CStrId();
    CRntVector<UnkPathRecord_lfd11e8>& records = block->records;
    if (records.data) {
        for (u64 i = 0; i < records.count; ++i)
            records.data[i].path.~CFilePathStrId();
        records.count = 0;
        records.allocator->vf_0x48(records.data);
        records.capacity = 0;
    }
}

UnkEntry_0x150::~UnkEntry_0x150() {
    UnkEntry_lfd11e8* self = reinterpret_cast<UnkEntry_lfd11e8*>(this);
    self->vtable = g_unk_7101cac0c0 + 2;
    DestroyBlock(&self->blocks[1]);
    DestroyBlock(&self->blocks[0]);
    self->field_0x60.Free();
    self->field_0x38.Free();
    self->vtable = g_unk_7101cac148 + 2;
    fn_71000a1c60(&self->value);
}
