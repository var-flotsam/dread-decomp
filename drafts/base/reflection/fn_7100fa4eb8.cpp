// base::reflection's unit (tools/tu_map.py, unit 2610): the move constructor of the 0xd0-byte object
// fn_7100fa45a4 constructs: it is built as that constructor builds it, then takes over the other's ids
// and lists (the nine ids are copied by fn_7100fa4b64). Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CGameObject.h"
#include "types.h"

#include <new>

using base::global::CFilePathStrId;
using base::global::CRntVector;
using base::global::CStrId;

extern char g_unk_7101cab428[];  // the object's vtable

struct UnkStrIdRecord_lfa4b64;
UnkStrIdRecord_lfa4b64* fn_7100fa4b64(UnkStrIdRecord_lfa4b64* self, const UnkStrIdRecord_lfa4b64* other);

// An empty id, creating the pool first when there is none.
static inline void MakeEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

struct UnkObject_lfa4eb8 {
    void** vtable;
    u8 unk_0x8[0x8];
    CStrId field_0x10;
    CStrId field_0x18;
    CFilePathStrId field_0x20;
    CStrId field_0x28;
    CStrId field_0x30;  // nine ids, assigned together (fn_7100fa4b64)
    CStrId field_0x38;
    CStrId field_0x40;
    CStrId field_0x48;
    CStrId field_0x50;
    CStrId field_0x58;
    CStrId field_0x60;
    CStrId field_0x68;
    CStrId field_0x70;
    CRntVector<CStrId> field_0x78;
    CRntVector<CFilePathStrId> field_0xa0;
    f32 field_0xc8;
    bool field_0xcc;
    bool field_0xcd;
};
static_assert(__builtin_offsetof(UnkObject_lfa4eb8, field_0xa0) == 0xa0, "");
static_assert(__builtin_offsetof(UnkObject_lfa4eb8, field_0xcd) == 0xcd, "");

void fn_7100fa4eb8(void* dst, void* src) {
    UnkObject_lfa4eb8* self = static_cast<UnkObject_lfa4eb8*>(dst);
    UnkObject_lfa4eb8& other = *static_cast<UnkObject_lfa4eb8*>(src);

    // Built as fn_7100fa45a4 builds it.
    fn_7101043904(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101cab428 + 0x10);
    new (&self->field_0x10) CStrId(g_unk_7101d470e0);
    new (&self->field_0x18) CStrId(g_unk_7101d470e0);
    new (&self->field_0x20) CFilePathStrId(g_unk_7101d470e8);
    new (&self->field_0x28) CStrId(g_unk_7101d470e0);
    MakeEmpty(&self->field_0x30);
    MakeEmpty(&self->field_0x38);
    MakeEmpty(&self->field_0x40);
    MakeEmpty(&self->field_0x48);
    MakeEmpty(&self->field_0x50);
    MakeEmpty(&self->field_0x58);
    MakeEmpty(&self->field_0x60);
    MakeEmpty(&self->field_0x68);
    MakeEmpty(&self->field_0x70);
    new (&self->field_0x78) CRntVector<CStrId>(0xf);
    new (&self->field_0xa0) CRntVector<CFilePathStrId>(0xf);
    self->field_0xc8 = -1.0f;
    self->field_0xcc = false;
    self->field_0xcd = false;

    // Then takes over the other's contents (the nine ids are copied).
    self->field_0x10.ReleaseAndTake(other.field_0x10);
    self->field_0x18.ReleaseAndTake(other.field_0x18);
    fn_7100fa4b64(reinterpret_cast<UnkStrIdRecord_lfa4b64*>(&self->field_0x30),
                  reinterpret_cast<const UnkStrIdRecord_lfa4b64*>(&other.field_0x30));
    self->field_0x78 = static_cast<CRntVector<CStrId>&&>(other.field_0x78);
    self->field_0x28.ReleaseAndTake(other.field_0x28);
    self->field_0xa0 = static_cast<CRntVector<CFilePathStrId>&&>(other.field_0xa0);
    self->field_0x20 = static_cast<CFilePathStrId&&>(other.field_0x20);
    self->field_0xc8 = other.field_0xc8;
    self->field_0xcc = other.field_0xcc;
    self->field_0xcd = other.field_0xcd;
}
