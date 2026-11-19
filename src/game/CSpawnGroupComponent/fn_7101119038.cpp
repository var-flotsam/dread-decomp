// CSpawnGroupComponent unit (tools/tu_map.py unit 2703): fn_7101119038 is the component's destructor: the
// collider goes back to the collision manager, the members are released last first, then the base component's
// destructor runs. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/UnkStrIdTable.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkComponent.h"
#include "game/UnkTU2506.h"
#include "game/UnkTU341.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::UnkStrIdMember;
using base::global::CRntString;

extern void* g_unk_7101cb2680[];  // The component's vtable.

struct UnkManager_7100863a8c;

struct UnkSpawnGroupLinks_l119038;
template <>
struct base::UnkCallbackTraits<UnkSpawnGroupLinks_l119038> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
// A link to an object of the scene (game/CGameLink.h).
using UnkSpawnGroupLink_l119038 = base::UnkCallbackListShort<UnkSpawnGroupLinks_l119038>;

// An element of the array at +0x68.
struct UnkSpawnGroupEntry_l119038 {
    virtual ~UnkSpawnGroupEntry_l119038();
    u8 pad_0x8[0x38];
};

struct UnkSpawnGroupComponentD_l119038 : UnkBaseComponent {
    base::global::CRntVector<base::UnkRefPtrMember> refs_a;          // 0x40
    base::global::CRntVector<UnkSpawnGroupEntry_l119038> entries;    // 0x68
    base::global::CRntVector<base::UnkRefPtrMember> refs_b;          // 0x90
    UnkSpawnGroupLink_l119038 link_a;                                // 0xb8
    UnkSpawnGroupLink_l119038 link_b;                                // 0xf8
    u8 pad_0x138[0x10];
    CRntString name;  // 0x148
    u8 pad_0x178[0x30];
    UnkCollisionObject* collider;  // 0x1a8
    u8 pad_0x1b0[0x10];
    UnkStrIdMember id_a;  // 0x1c0
    CRntString field_0x1c8;
    CRntString field_0x1f8;
    CRntString field_0x228;
    CRntString field_0x258;
    CRntString field_0x288;
    CRntString field_0x2b8;
    CRntString field_0x2e8;
    CRntString field_0x318;
    CRntString field_0x348;
    CRntString field_0x378;
    CRntString field_0x3a8;
    u8 pad_0x3d8[0x10];
    UnkStrIdMember id_b;   // 0x3e8
    UnkOwnedStrIdTable table;    // 0x3f0
    UnkStrIdMember id_c;   // 0x418
};
static_assert(__builtin_offsetof(UnkSpawnGroupComponentD_l119038, link_b) == 0xf8, "layout");
static_assert(__builtin_offsetof(UnkSpawnGroupComponentD_l119038, name) == 0x148, "layout");
static_assert(__builtin_offsetof(UnkSpawnGroupComponentD_l119038, collider) == 0x1a8, "layout");
static_assert(__builtin_offsetof(UnkSpawnGroupComponentD_l119038, field_0x3a8) == 0x3a8, "layout");
static_assert(__builtin_offsetof(UnkSpawnGroupComponentD_l119038, id_b) == 0x3e8, "layout");
static_assert(__builtin_offsetof(UnkSpawnGroupComponentD_l119038, id_c) == 0x418, "layout");

static inline UnkManager_7100863a8c* CollisionManager() {
    UnkManager_7100863a8c* m = g_unk_71726a4788;
    if (!m) {
        m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    return m;
}

void fn_7101119038(char* p) {
    UnkSpawnGroupComponentD_l119038* self = reinterpret_cast<UnkSpawnGroupComponentD_l119038*>(p);
    self->vtable = &g_unk_7101cb2680[2];
    if (self->collider) {
        fn_7100d46c60(CollisionManager(), self->collider);
        self->collider = nullptr;
    }
    self->id_c.~UnkStrIdMember();
    self->table.~UnkOwnedStrIdTable();
    self->id_b.~UnkStrIdMember();
    self->field_0x3a8.~CRntString();
    self->field_0x378.~CRntString();
    self->field_0x348.~CRntString();
    self->field_0x318.~CRntString();
    self->field_0x2e8.~CRntString();
    self->field_0x2b8.~CRntString();
    self->field_0x288.~CRntString();
    self->field_0x258.~CRntString();
    self->field_0x228.~CRntString();
    self->field_0x1f8.~CRntString();
    self->field_0x1c8.~CRntString();
    self->id_a.~UnkStrIdMember();
    self->name.~CRntString();
    self->link_b.~UnkSpawnGroupLink_l119038();
    self->link_a.~UnkSpawnGroupLink_l119038();
    self->refs_b.Free();
    self->entries.Free();
    self->refs_a.Free();
    fn_71010c4194(self);
}
