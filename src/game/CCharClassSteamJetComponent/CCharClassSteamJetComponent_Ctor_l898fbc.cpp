// CCharClassSteamJetComponent's unit (tools/tu_map.py): a constructor: the base, four empty lists,
// two list holders, two listeners bound to this object, a string-id list and a zero vector.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CAbilityGhostAura.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CPullOffGrapplePointComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

void fn_7100877960(void* self);
void fn_7100893a1c(char* self);
void fn_710089b03c(void* self, void* arg);
void fn_710089b040(void* self, void* arg);
extern void* g_unk_7101a7cad8[];

// A list with its own vtable, a count and a link.
struct UnkSteamJetListHolder_l898fbc {
    void** vtable;
    base::global::CRntVector<void*> list;
    u32 field_0x30;
    u64 field_0x38;

    UnkSteamJetListHolder_l898fbc() : vtable(g_unk_7101a20998 + 2), list(0xf), field_0x30(0) {
        vtable = g_unk_7101a33fa0 + 2;
        field_0x38 = 0;
    }
};

// A listener calling a function on its owner.
struct UnkSteamJetListener_l898fbc {
    void** vtable;
    void* owner;
    void (*callback)(void*, void*);
    u64 field_0x18;
};

struct UnkSteamJetComponent_l898fbc {
    void** vtable;
    u8 pad_0x8[0x38];
    base::global::CRntVector<void*> lists[4];  // 0x40
    u64 field_0xe0;
    u64 field_0xe8;
    u32 field_0xf0;
    bool field_0xf4;
    UnkSteamJetListHolder_l898fbc holder_a;  // 0xf8
    UnkSteamJetListHolder_l898fbc holder_b;  // 0x138
    UnkSteamJetListener_l898fbc listener_a;    // 0x178
    UnkSteamJetListener_l898fbc listener_b;    // 0x198
    u8 ids[0x38];                              // 0x1b8, built by fn_7100893a1c
    base::math::CVector3D field_0x1f0;
    bool field_0x1fc;
    u64 field_0x200;
    bool field_0x208;
};
static_assert(__builtin_offsetof(UnkSteamJetComponent_l898fbc, holder_a) == 0xf8, "layout");
static_assert(__builtin_offsetof(UnkSteamJetComponent_l898fbc, ids) == 0x1b8, "layout");
static_assert(__builtin_offsetof(UnkSteamJetComponent_l898fbc, field_0x208) == 0x208, "layout");

void fn_7100898fbc(UnkSteamJetComponent_l898fbc* self) {
    fn_7100877960(self);
    self->vtable = g_unk_7101a7cad8 + 2;
    for (base::global::CRntVector<void*>& list : self->lists)
        new (&list) base::global::CRntVector<void*>(0xf);
    self->field_0xe0 = 0;
    self->field_0xe8 = 0;
    self->field_0xf0 = 0;
    self->field_0xf4 = false;
    new (&self->holder_a) UnkSteamJetListHolder_l898fbc();
    new (&self->holder_b) UnkSteamJetListHolder_l898fbc();
    self->listener_a.vtable = g_unk_7101a22d48 + 2;
    self->listener_a.owner = nullptr;
    self->listener_a.callback = nullptr;
    self->listener_a.field_0x18 = 0;
    self->listener_b.vtable = g_unk_7101a77000 + 2;
    self->listener_b.owner = nullptr;
    self->listener_b.callback = nullptr;
    self->listener_b.field_0x18 = 0;
    fn_7100893a1c(reinterpret_cast<char*>(self->ids));
    self->field_0x1f0.x = g_unk_710164779c.x;
    self->field_0x1f0.y = g_unk_710164779c.y;
    self->field_0x1f0.z = g_unk_710164779c.z;
    self->field_0x1fc = false;
    self->field_0x200 = 0;
    self->field_0x208 = false;
    self->listener_a.owner = self;
    self->listener_a.callback = fn_710089b03c;
    self->listener_b.owner = self;
    self->listener_b.callback = fn_710089b040;
}
