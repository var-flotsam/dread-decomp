// Unit 2200 (hint CCutSceneDef): the constructor of the record fn_7100c8c7b4 assigns: a kind of 1, empty
// string and path ids, a zero vector and four empty arrays (flags 0xf). Names and layouts are
// placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

#include <new>

using base::global::CFilePathStrId;
using base::global::CRntVector;
using base::global::CStrId;

struct UnkTake_lc8aefc;
struct UnkShot_lc8aefc;
struct UnkPathEntry_lc8aefc;

struct UnkRecord_lc8aefc {
    u32 kind;
    CStrId field_0x8;
    CFilePathStrId field_0x10;
    f32 field_0x18[3];
    CStrId field_0x28;
    CStrId field_0x30;
    CStrId field_0x38;
    CStrId field_0x40;
    CStrId field_0x48;
    CStrId field_0x50;
    u64 field_0x58;
    u32 field_0x60;
    bool field_0x64;
    CRntVector<UnkTake_lc8aefc> takes;  // 0x68
    CRntVector<UnkShot_lc8aefc> shots;  // 0x90
    CRntVector<CFilePathStrId> paths;  // 0xb8
    CRntVector<UnkPathEntry_lc8aefc> entries;  // 0xe0
    u32 field_0x108;
};
static_assert(__builtin_offsetof(UnkRecord_lc8aefc, takes) == 0x68, "takes");
static_assert(__builtin_offsetof(UnkRecord_lc8aefc, field_0x108) == 0x108, "field_0x108");

// An empty id, creating the pool first when there is none.
static inline void MakeEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100c8aefc(UnkRecord_lc8aefc* self) {
    self->kind = 1;
    MakeEmpty(&self->field_0x8);
    new (&self->field_0x10) CFilePathStrId();
    self->field_0x18[0] = 0.0f;
    self->field_0x18[1] = 0.0f;
    self->field_0x18[2] = 0.0f;
    MakeEmpty(&self->field_0x28);
    MakeEmpty(&self->field_0x30);
    MakeEmpty(&self->field_0x38);
    MakeEmpty(&self->field_0x40);
    MakeEmpty(&self->field_0x48);
    MakeEmpty(&self->field_0x50);
    new (&self->takes) CRntVector<UnkTake_lc8aefc>(0xf);
    new (&self->shots) CRntVector<UnkShot_lc8aefc>(0xf);
    new (&self->paths) CRntVector<CFilePathStrId>(0xf);
    new (&self->entries) CRntVector<UnkPathEntry_lc8aefc>(0xf);
}
