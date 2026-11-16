// CSwarmControllerComponent unit (tools/tu_map.py unit 1163): the controller's constructor. After the
// component base (fn_71010c4148) it sets the vtable, a callback set at +0x40, engine arrays (flags 0xf),
// two small dictionaries (flags 0xe), cleared state and a bound callback at +0x268. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "game/CComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkSwarmCtrl_ovb0063;
struct UnkSwarmController_l7477f8;

extern void* g_unk_7101a6dc58[];  // the controller's vtable
void fn_710074e588(UnkSwarmController_l7477f8* self);

struct UnkSwarmCallbackSet_l7477f8 {
    void** vtable;
    base::global::CRntVector<void*> callables;
    u32 field_0x30;
    void* owner;
    u64 field_0x40;
};

struct UnkSwarmController_l7477f8 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkSwarmCallbackSet_l7477f8 callbacks;  // 0x40
    u8 pad_0x88[0x8];
    base::global::CRntVector<void*> field_0x90;
    base::global::CRntVector<void*> field_0xb8;
    f32 field_0xe0;
    f32 field_0xe4;
    u64 field_0xe8;
    base::global::CRntSmallDictionary<void*, void*> field_0xf0;
    base::global::CRntVector<void*> field_0x118;
    base::global::CRntVector<void*> field_0x140;
    base::global::CRntVector<void*> field_0x168;
    u32 field_0x190;
    u32 field_0x194;
    u32 field_0x198;
    bool field_0x19c;
    u8 pad_0x1a0[0x8];
    base::global::CRntVector<void*> field_0x1a8;
    bool field_0x1d0;
    u32 field_0x1d4;
    u32 field_0x1d8;
    u32 field_0x1dc;
    u32 field_0x1e0;
    u32 field_0x1e4;
    u8 pad_0x1e8[0xc];
    u32 field_0x1f4;
    u8 pad_0x1f8[0x4];
    bool field_0x1fc;
    u64 field_0x200;
    base::global::CRntVector<void*> field_0x208;
    base::global::CRntVector<void*> sounds;  // 0x230
    u8 pad_0x258[0x4];
    bool field_0x25c;
    void* field_0x260;
    UnkSwarmController_l7477f8* handler_owner;  // 0x268
    void (*handler)(UnkSwarmController_l7477f8*);
    base::global::CRntSmallDictionary<void*, void*> field_0x278;
};
static_assert(__builtin_offsetof(UnkSwarmController_l7477f8, field_0x90) == 0x90, "layout");
static_assert(__builtin_offsetof(UnkSwarmController_l7477f8, field_0x190) == 0x190, "layout");
static_assert(__builtin_offsetof(UnkSwarmController_l7477f8, field_0x1fc) == 0x1fc, "layout");
static_assert(__builtin_offsetof(UnkSwarmController_l7477f8, field_0x278) == 0x278, "layout");
static_assert(sizeof(UnkSwarmController_l7477f8) == 0x2a0, "layout");

void fn_71007477f8(UnkSwarmCtrl_ovb0063* object) {
    auto* self = reinterpret_cast<UnkSwarmController_l7477f8*>(object);
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a6dc58 + 2;
    self->callbacks.vtable = g_unk_7101a20998 + 2;
    new (&self->callbacks.callables) base::global::CRntVector<void*>(0xf);
    self->callbacks.field_0x30 = 0;
    self->callbacks.owner = nullptr;
    self->callbacks.field_0x40 = 0;
    self->callbacks.vtable = g_unk_7101a20950 + 2;
    new (&self->field_0x90) base::global::CRntVector<void*>(0xf);
    new (&self->field_0xb8) base::global::CRntVector<void*>(0xf);
    self->field_0xe0 = 0.2f;
    self->field_0xe4 = 100.0f;
    self->field_0xe8 = 0;
    new (&self->field_0xf0) base::global::CRntSmallDictionary<void*, void*>(0xe);
    new (&self->field_0x118) base::global::CRntVector<void*>(0xf);
    new (&self->field_0x140) base::global::CRntVector<void*>(0xf);
    new (&self->field_0x168) base::global::CRntVector<void*>(0xf);
    self->field_0x190 = 0;
    self->field_0x194 = 0;
    self->field_0x198 = 0;
    self->field_0x19c = false;
    new (&self->field_0x1a8) base::global::CRntVector<void*>(0xf);
    self->field_0x1d0 = false;
    self->field_0x1d4 = 0;
    self->field_0x1d8 = 0;
    self->field_0x1dc = 0;
    self->field_0x1e0 = 0;
    self->field_0x1e4 = 0;
    self->field_0x1f4 = 0;
    self->field_0x1fc = false;
    self->field_0x200 = 0;
    new (&self->field_0x208) base::global::CRntVector<void*>(0xf);
    new (&self->sounds) base::global::CRntVector<void*>(0xf);
    self->field_0x25c = false;
    self->field_0x260 = nullptr;
    self->handler_owner = self;
    self->handler = fn_710074e588;
    new (&self->field_0x278) base::global::CRntSmallDictionary<void*, void*>(0xe);
}
