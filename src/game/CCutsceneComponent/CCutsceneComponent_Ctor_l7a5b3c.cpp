// CCutsceneComponent's unit: a constructor over the base (fn_710024f694): seven empty arrays with flags
// 15, an empty string id and file path, a sub-object built by fn_71007a8db0 and given its own vtable,
// the default mask and cleared flags. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CCutsceneComponent.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntVector;
using base::global::UnkStringData;

struct UnkElem_0x88;
struct UnkCutsceneEntry_l7a5b3c;

void fn_710024f694(void* self);

extern void* g_unk_7101a6ff40[];
extern char g_unk_7101a70078[];

struct UnkCutsceneSub_l7a5b3c {
    void** vtable;
    u8 pad_0x8[0x80];
    u64 field_0x88;
    u64 field_0x90;
    u32 field_0x98;
    u8 pad_0x9c[4];
    u64 field_0xa0;
    u64 field_0xa8;
};

struct UnkCutsceneComponent_l7a5b3c {
    void** vtable;
    u8 pad_0x8[0x28];
    CRntVector<UnkCutsceneEntry_l7a5b3c> list_0x30;
    CRntVector<UnkCutsceneEntry_l7a5b3c> list_0x58;
    CRntVector<UnkCutsceneEntry_l7a5b3c> list_0x80;
    CRntVector<UnkCutsceneEntry_l7a5b3c> list_0xa8;
    CRntVector<UnkCutsceneEntry_l7a5b3c> list_0xd0;
    CRntVector<UnkCutsceneEntry_l7a5b3c> list_0xf8;
    CRntVector<UnkCutsceneEntry_l7a5b3c> list_0x120;
    UnkStringData* name;  // 0x148
    CFilePathStrId path;  // 0x150
    UnkCutsceneSub_l7a5b3c sub;  // 0x158
    u32 mask;  // 0x208
    u8 field_0x20c;
    u8 field_0x20d;
    u8 field_0x20e;
    u8 field_0x20f;
    u8 field_0x210;
    u8 field_0x211;
    u8 field_0x212;
    u8 field_0x213;
    u8 field_0x214;
    u8 field_0x215;
    u8 field_0x216;
    u8 field_0x217;
};
static_assert(__builtin_offsetof(UnkCutsceneComponent_l7a5b3c, sub) == 0x158, "layout");
static_assert(__builtin_offsetof(UnkCutsceneComponent_l7a5b3c, mask) == 0x208, "layout");

void fn_71007a5b3c(UnkCutsceneComponent_l7a5b3c* self) {
    fn_710024f694(self);
    self->vtable = g_unk_7101a6ff40 + 2;
    new (&self->list_0x30) CRntVector<UnkCutsceneEntry_l7a5b3c>(15);
    new (&self->list_0x58) CRntVector<UnkCutsceneEntry_l7a5b3c>(15);
    new (&self->list_0x80) CRntVector<UnkCutsceneEntry_l7a5b3c>(15);
    new (&self->list_0xa8) CRntVector<UnkCutsceneEntry_l7a5b3c>(15);
    new (&self->list_0xd0) CRntVector<UnkCutsceneEntry_l7a5b3c>(15);
    new (&self->list_0xf8) CRntVector<UnkCutsceneEntry_l7a5b3c>(15);
    new (&self->list_0x120) CRntVector<UnkCutsceneEntry_l7a5b3c>(15);
    self->name = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->name = &g_unk_7101d47108->empty;
    new (&self->path) CFilePathStrId();
    fn_71007a8db0(reinterpret_cast<UnkElem_0x88*>(&self->sub));
    self->sub.vtable = reinterpret_cast<void**>(g_unk_7101a70078 + 0x10);
    self->sub.field_0x88 = 0;
    self->sub.field_0x90 = 0;
    self->sub.field_0x98 = 0;
    self->sub.field_0xa0 = 0;
    self->sub.field_0xa8 = 0;
    self->mask = base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>();
    self->field_0x20c = 0;
    self->field_0x20d = 0;
    self->field_0x20e = 1;
    self->field_0x20f = 0;
    self->field_0x210 = 0;
    self->field_0x211 = 0;
    self->field_0x212 = 0;
    self->field_0x213 = 0;
    self->field_0x214 = 0;
    self->field_0x215 = 0;
    self->field_0x216 = 0;
    self->field_0x217 = 0;
}
