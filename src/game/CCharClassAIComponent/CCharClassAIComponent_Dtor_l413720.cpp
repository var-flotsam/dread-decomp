// CCharClassAIComponent (unit 616): the destructor. The owned objects are released, the two registrations
// with the navigation mesh manager are dropped, then the string ids, id arrays and string are released
// last first and the char class component base's destructor runs. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassAIComponent.h"
#include "game/CCharClassComponent.h"
#include "game/UnkNavMeshManager.h"
#include "types.h"

using base::UnkStrIdMember;
using base::global::CRntString;

void fn_7100ec1138(UnkNavMeshManager* self, const UnkStrIdMember* entry);

struct UnkOwned_l413720 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
};

// A pointer to an object that is released when the pointer goes away.
struct UnkOwnedPtr_l413720 {
    UnkOwned_l413720* ptr;
    ~UnkOwnedPtr_l413720() {
        UnkOwned_l413720* p = ptr;
        ptr = nullptr;
        if (p)
            p->Release();
    }
};

struct UnkCharClassAI_l413720 {
    void** vtable;
    u8 pad_0x8[0x50];
    base::global::CRntVector<UnkOwnedPtr_l413720> owned;  // 0x58
    CRntString field_0x80;
    u8 pad_0xb0[0x30];
    UnkStrIdMember id_0xe0;
    u8 pad_0xe8[0x68];
    UnkStrIdMember id_0x150;
    u8 pad_0x158[0x10];
    UnkStrIdMember id_0x168;
    u8 pad_0x170[0x8];
    UnkStrIdMember id_0x178;
    u8 pad_0x180[0x48];
    UnkStrIdMember id_0x1c8;
    void* link_0x1d0;
    UnkStrIdMember id_0x1d8;
    void* link_0x1e0;
    UnkStrIdMember id_0x1e8;
    UnkStrIdMember id_0x1f0;
    u8 pad_0x1f8[0x8];
    UnkStrIdMember id_0x200;
    u8 pad_0x208[0x8];
    UnkStrIdMember id_0x210;
    u8 pad_0x218[0x8];
    UnkStrIdMember id_0x220;
    u8 pad_0x228[0x8];
    UnkStrIdMember id_0x230;
    u8 pad_0x238[0x180];
    base::global::CRntVector<UnkStrIdMember> ids_0x3b8;
    base::global::CRntVector<UnkStrIdMember> ids_0x3e0;
    u8 pad_0x408[0x38];
    UnkStrIdMember id_0x440;
    UnkStrIdMember id_0x448;
    u8 pad_0x450[0x8];
    UnkStrIdMember id_0x458;
    u8 pad_0x460[0x18];
    UnkStrIdMember id_0x478;
    UnkStrIdMember id_0x480;
    u8 pad_0x488[0x60];
    UnkStrIdMember id_0x4e8;
};
static_assert(__builtin_offsetof(UnkCharClassAI_l413720, owned) == 0x58, "layout");
static_assert(__builtin_offsetof(UnkCharClassAI_l413720, id_0x1c8) == 0x1c8, "layout");
static_assert(__builtin_offsetof(UnkCharClassAI_l413720, id_0x230) == 0x230, "layout");
static_assert(__builtin_offsetof(UnkCharClassAI_l413720, ids_0x3b8) == 0x3b8, "layout");
static_assert(__builtin_offsetof(UnkCharClassAI_l413720, id_0x440) == 0x440, "layout");
static_assert(__builtin_offsetof(UnkCharClassAI_l413720, id_0x4e8) == 0x4e8, "layout");

static inline UnkNavMeshManager* NavMeshManager() {
    UnkNavMeshManager* m = g_unk_71726a4510;
    if (!m) {
        m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return m;
}

struct UnkStruct_7100416e80;

void fn_7100413720(UnkStruct_7100416e80* p) {
    UnkCharClassAI_l413720* self = reinterpret_cast<UnkCharClassAI_l413720*>(p);
    self->vtable = g_unk_7101a34100 + 2;
    self->owned.Clear();
    if (self->link_0x1d0) {
        self->link_0x1d0 = nullptr;
        fn_7100ec1138(NavMeshManager(), &self->id_0x1d8);
    }
    if (self->link_0x1e0) {
        self->link_0x1e0 = nullptr;
        fn_7100ec1138(NavMeshManager(), &self->id_0x1e8);
    }
    self->id_0x4e8.~UnkStrIdMember();
    self->id_0x480.~UnkStrIdMember();
    self->id_0x478.~UnkStrIdMember();
    self->id_0x458.~UnkStrIdMember();
    self->id_0x448.~UnkStrIdMember();
    self->id_0x440.~UnkStrIdMember();
    self->ids_0x3e0.Free();
    self->ids_0x3b8.Free();
    self->id_0x230.~UnkStrIdMember();
    self->id_0x220.~UnkStrIdMember();
    self->id_0x210.~UnkStrIdMember();
    self->id_0x200.~UnkStrIdMember();
    self->id_0x1f0.~UnkStrIdMember();
    self->id_0x1e8.~UnkStrIdMember();
    self->id_0x1d8.~UnkStrIdMember();
    self->id_0x1c8.~UnkStrIdMember();
    self->id_0x178.~UnkStrIdMember();
    self->id_0x168.~UnkStrIdMember();
    self->id_0x150.~UnkStrIdMember();
    self->id_0xe0.~UnkStrIdMember();
    self->field_0x80.~CRntString();
    self->owned.Free();
    fn_71011acd74(reinterpret_cast<UnkIdObject_8da5c0*>(self));
}
