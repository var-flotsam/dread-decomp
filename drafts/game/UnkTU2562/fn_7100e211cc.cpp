// UnkTU2562 (the dropped-item manager's unit): its constructor. Eight drops in place, each registered,
// seven id/path records, a small dictionary and the defaults around them.
// Names and layouts are placeholders.
#include <new>

#include "base/core/CBaseObject.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CStrId.h"
#include "game/UnkTU101.h"
#include "game/UnkTU2562.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntSmallDictionary;
using base::global::CStrId;

struct UnkDrop_b0105;
void fn_71007bf3f0(UnkDrop_b0105* self);
void fn_7100e215c0(void* self);

// An id/path resource record (0x18 bytes), released with fn_7100026f70.
struct UnkDropResource_b0105 {
    CStrId id;
    CFilePathStrId path;
    u64 field_0x10;
    UnkDropResource_b0105() : id(g_unk_7101d470e0), path(g_unk_7101d470e8), field_0x10(0) {}
};

// The settings block at 0x7808 (0x88 bytes).
struct UnkDropSettings_e211cc {
    s32 field_0x0;
    s32 field_0x4;
    u32 field_0x8;  // not set by the constructor
    s32 field_0xc;
    s32 field_0x10;
    UnkSoundHandle sound;  // 0x18
    CStrId id;             // 0x28
    CFilePathStrId path;   // 0x30
    u64 field_0x38[9];
    bool field_0x80;
    f32 field_0x84;

    UnkDropSettings_e211cc()
        : field_0x0(-1), field_0x4(-1), field_0xc(-1), field_0x10(-1), sound(g_unk_7108b6c460), id(g_unk_7101d470e0),
          path(g_unk_7101d470e8), field_0x38(), field_0x80(true), field_0x84(160.0f) {}
};
static_assert(sizeof(UnkDropSettings_e211cc) == 0x88, "layout");

struct UnkDroppedItems_e211cc {
    bool field_0x0;
    alignas(8) u8 field_0x8[0x7800];  // built by fn_7100e215c0
    UnkDropSettings_e211cc settings;  // 0x7808
    alignas(8) u8 drops[8][0x4f0];    // 0x7890: UnkDrop_b0105
    u64 field_0xa010;
    u64 field_0xa018;
    UnkDropResource_b0105 resources[7];  // 0xa020
    CRntSmallDictionary<u64, u64> dictionary;  // 0xa0c8
    bool field_0xa0f0;
    u32 field_0xa0f4;
};
static_assert(__builtin_offsetof(UnkDroppedItems_e211cc, field_0x8) == 0x8, "layout");
static_assert(__builtin_offsetof(UnkDroppedItems_e211cc, drops) == 0x7890, "layout");
static_assert(__builtin_offsetof(UnkDroppedItems_e211cc, dictionary) == 0xa0c8, "layout");
static_assert(sizeof(UnkDroppedItems_e211cc) == 0xa0f8, "layout");

// A drop is built in place, then registered.
inline void InitDrop(u8* storage) {
    UnkDrop_b0105* drop = reinterpret_cast<UnkDrop_b0105*>(storage);
    fn_71007bf3f0(drop);
    fn_7100023854(reinterpret_cast<UnkNode*>(drop));
}

void fn_7100e211cc(void* instance) {
    UnkDroppedItems_e211cc* self = static_cast<UnkDroppedItems_e211cc*>(instance);
    self->field_0x0 = true;
    fn_7100e215c0(self->field_0x8);
    new (&self->settings) UnkDropSettings_e211cc();
    InitDrop(self->drops[0]);
    InitDrop(self->drops[1]);
    InitDrop(self->drops[2]);
    InitDrop(self->drops[3]);
    InitDrop(self->drops[4]);
    InitDrop(self->drops[5]);
    InitDrop(self->drops[6]);
    InitDrop(self->drops[7]);
    self->field_0xa010 = 0;
    self->field_0xa018 = 0;
    for (UnkDropResource_b0105& resource : self->resources)
        new (&resource) UnkDropResource_b0105();
    new (&self->dictionary) CRntSmallDictionary<u64, u64>(14);
    self->field_0xa0f0 = true;
    self->field_0xa0f4 = 0;
}
