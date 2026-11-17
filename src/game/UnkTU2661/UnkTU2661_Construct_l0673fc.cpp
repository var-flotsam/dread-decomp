// build-flags: no-strict-aliasing
// Unit 2661 (tools/tu_map.py, hint CCutscene::CTunableCutscene): a constructor: the counted media base, a
// record, two arrays, a bone filter, an empty sound handle, two empty file paths, two inline strings and its
// settings. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkTU101.h"
#include "game/media/CMediaStream.h"
#include "game/sound/UnkSoundHandle.h"
#include "../../game_aca57c/UnkBufferString.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntVector;

struct UnkRecord_lc8aefc;
struct UnkBoneFilter_ovb0138;
void fn_7100c8aefc(UnkRecord_lc8aefc* self);
void fn_710070d188(UnkBoneFilter_ovb0138* self);

extern void* g_unk_7101cac9b0[];

struct UnkCutsceneStream_l0673fc {
    void** vtable;
    u32 ref_count;  // 0x8, released
    u8 pad_0xc[4];
    u8 record[0x110];  // 0x10
    u64 field_0x120;
    CRntVector<void*> field_0x128;
    CRntVector<void*> field_0x150;
    u64 field_0x178;
    u8 bone_filter[0x178];  // 0x180
    u64 field_0x2f8;
    UnkSoundHandle sound;  // 0x300
    CFilePathStrId path;  // 0x310
    CFilePathStrId field_0x318;
    UnkBufferString<0x80, g_unk_7101a1b750> name;  // 0x320
    UnkBufferString<0x80, g_unk_7101a1b750> field_0x3e8;
    s32 field_0x4b0;
    s32 field_0x4b4;
    s32 field_0x4b8;
    f32 field_0x4bc;
    f32 field_0x4c0;
    f32 field_0x4c4;
    f32 field_0x4c8;
    bool field_0x4cc;
    bool field_0x4cd;
    f32 field_0x4d0;
    f32 field_0x4d4;
    f32 field_0x4d8;
    bool field_0x4dc;
    f32 field_0x4e0;
    f32 field_0x4e4;
    f32 field_0x4e8;
    f32 field_0x4ec;
    u64 field_0x4f0;
    u64 field_0x4f8;
    bool field_0x500;
    bool field_0x501;
    bool field_0x502;
    bool field_0x503;
    bool field_0x504;
    bool field_0x505;
    u64 field_0x508;
    u64 field_0x510;
    f32 field_0x518;
    f32 field_0x51c;
    bool field_0x520;
    bool field_0x521;
    bool field_0x522;
    bool field_0x523;
    bool field_0x524;
};
static_assert(__builtin_offsetof(UnkCutsceneStream_l0673fc, sound) == 0x300, "layout");
static_assert(__builtin_offsetof(UnkCutsceneStream_l0673fc, field_0x4b0) == 0x4b0, "layout");
static_assert(__builtin_offsetof(UnkCutsceneStream_l0673fc, field_0x4e0) == 0x4e0, "layout");
static_assert(__builtin_offsetof(UnkCutsceneStream_l0673fc, field_0x524) == 0x524, "layout");

void fn_71010673fc(UnkCutsceneStream_l0673fc* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101a203f8 + 0x10);
    __atomic_store_n(&self->ref_count, 0, __ATOMIC_RELEASE);
    self->vtable = g_unk_7101cac9b0 + 2;
    fn_7100c8aefc(reinterpret_cast<UnkRecord_lc8aefc*>(self->record));
    self->field_0x120 = 0;
    new (&self->field_0x128) CRntVector<void*>(0xf);
    new (&self->field_0x150) CRntVector<void*>(0xf);
    self->field_0x178 = 0;
    fn_710070d188(reinterpret_cast<UnkBoneFilter_ovb0138*>(self->bone_filter));
    self->field_0x2f8 = 0;
    self->sound = g_unk_7108b6c460;
    new (&self->path) CFilePathStrId();
    new (&self->field_0x318) CFilePathStrId();
    new (&self->name) UnkBufferString<0x80, g_unk_7101a1b750>();
    new (&self->field_0x3e8) UnkBufferString<0x80, g_unk_7101a1b750>();
    self->field_0x4b0 = 0x7fffffff;
    self->field_0x4b4 = 0;
    self->field_0x4b8 = 0;
    self->field_0x4bc = -1.0f;
    self->field_0x4c0 = 0.0f;
    self->field_0x4c4 = 0.5f;
    self->field_0x4c8 = 0.0f;
    self->field_0x4cc = true;
    self->field_0x4cd = true;
    self->field_0x4d0 = 0.0f;
    self->field_0x4d4 = 0.0f;
    self->field_0x4d8 = 0.0f;
    self->field_0x4dc = false;
    self->field_0x4e0 = 100.0f;
    self->field_0x4e4 = 50.0f;
    self->field_0x4e8 = -1.0f;
    self->field_0x4ec = -1.0f;
    self->field_0x4f0 = 0;
    self->field_0x4f8 = 0;
    self->field_0x500 = false;
    self->field_0x501 = false;
    self->field_0x502 = true;
    self->field_0x503 = true;
    self->field_0x504 = true;
    self->field_0x505 = true;
    self->field_0x508 = 0;
    self->field_0x510 = 0;
    self->field_0x518 = 1.0f;
    self->field_0x51c = 0.0f;
    self->field_0x520 = false;
    self->field_0x521 = true;
    self->field_0x522 = false;
    self->field_0x523 = false;
    self->field_0x524 = true;
}
