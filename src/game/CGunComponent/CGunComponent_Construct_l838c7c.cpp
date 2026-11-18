// CGunComponent unit (tools/tu_map.py unit 1304): the gun component's constructor. It builds the
// component base, two callbacks on itself, the "GUN_INPUT" listener, the weapon groups and id lists,
// the charge settings, the listener lists, an empty sound handle and four animation tracks.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkNamedRegistry.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityLayout.h"
#include "game/CComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkTU101.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

extern void* g_unk_7101a77bc0[];
extern void* g_unk_7101a77dc8[];
bool fn_7100840db8(void* object, bool value);
bool fn_7100840eb8(void* object, bool value);

// A callback on the component: called with `object`.
struct UnkGunCallback_l838c7c {
    void* field_0x0;
    void* object;
    bool (*callback)(void* object, bool value);
};

// Three pointer lists and a lock count behind a vtable of their own.
struct UnkGunListeners_l838c7c {
    void** vtable;
    CRntVector<void*> added;    // 0x8
    CRntVector<void*> removed;  // 0x30
    CRntVector<void*> items;    // 0x58
    s32 lock;                   // 0x80
};

// An animation track the gun watches: its class, no instance yet and no time.
struct UnkGunTrack_l838c7c {
    const base::reflection::CClass* type;
    void* field_0x8;
    f32 time;

    void Init(const base::reflection::CClass* track_type) {
        type = track_type;
        field_0x8 = nullptr;
        time = -1.0f;
    }
};

struct UnkGunComponent_l838c7c : UnkBaseComponent {
    void* field_0x40;
    UnkGunCallback_l838c7c field_0x48;
    UnkGunCallback_l838c7c field_0x60;
    UnkAbilityInputListener input;  // 0x78
    void* field_0xc0;
    CRntVector<CRntVector<void*>> primary;    // 0xc8
    CRntVector<CRntVector<void*>> secondary;  // 0xf0
    void* alt;                 // 0x118
    void* main;                // 0x120
    bool use_alt;              // 0x128
    bool field_0x129;
    bool field_0x12a;
    f32 field_0x12c;
    u8 field_0x130;
    u8 field_0x131;
    s32 mode;                  // 0x134
    s32 field_0x138;
    CRntVector<CStrId> ids;    // 0x140
    CStrId id;                 // 0x168
    f32 charge_start;          // 0x170
    f32 charge_full;           // 0x174
    bool charging;             // 0x178
    f32 charge;                // 0x17c
    u8 field_0x180;
    u8 field_0x181;
    f32 field_0x184;
    u8 field_0x188;
    u8 field_0x189;
    f32 field_0x18c;
    f32 field_0x190;
    u32 field_0x194;
    CRntVector<void*> lists[3];  // 0x198
    bool field_0x210;
    u8 pad_0x211[0x7];
    bool field_0x218;
    UnkGunListeners_l838c7c listeners;  // 0x220
    UnkSoundHandle sound;               // 0x2a8
    u8 field_0x2b8;
    u8 field_0x2b9;
    UnkGunTrack_l838c7c tracks[4];      // 0x2c0
};
static_assert(__builtin_offsetof(UnkGunComponent_l838c7c, ids) == 0x140, "layout");
static_assert(__builtin_offsetof(UnkGunComponent_l838c7c, lists) == 0x198, "layout");
static_assert(__builtin_offsetof(UnkGunComponent_l838c7c, listeners) == 0x220, "layout");
static_assert(__builtin_offsetof(UnkGunComponent_l838c7c, tracks) == 0x2c0, "layout");

// The derived Samus gun component (CSamusGunComponent) calls this as its parent constructor.
struct UnkSamusGun_ovb0070;

void fn_7100838c7c(UnkSamusGun_ovb0070* gun) {
    UnkGunComponent_l838c7c* self = reinterpret_cast<UnkGunComponent_l838c7c*>(gun);
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a77bc0 + 2;
    self->field_0x40 = nullptr;
    self->field_0x48.field_0x0 = nullptr;
    self->field_0x48.object = self;
    self->field_0x48.callback = fn_7100840db8;
    self->field_0x60.field_0x0 = nullptr;
    self->field_0x60.object = self;
    self->field_0x60.callback = fn_7100840eb8;

    self->input.vtable = g_unk_7101a22878 + 2;
    fn_710007e704(&self->input.name, "GUN_INPUT");
    self->input.capacity = 20;
    // An array of 20 0x1c-byte entries, after its element count.
    u64* block = static_cast<u64*>(fn_7100083ba0(0x238, 5));
    *block = 20;
    self->input.entries = block + 1;
    self->field_0xc0 = nullptr;
    self->input.vtable = g_unk_7101a22858 + 2;

    new (&self->primary) CRntVector<CRntVector<void*>>(0xf);
    new (&self->secondary) CRntVector<CRntVector<void*>>(0xf);
    self->alt = nullptr;
    self->main = nullptr;
    self->use_alt = true;
    self->field_0x129 = true;
    self->field_0x12a = false;
    self->field_0x12c = 0.0f;
    self->field_0x130 = 0;
    self->field_0x131 = 0;
    self->mode = 1;
    new (&self->ids) CRntVector<CStrId>(0xf);
    new (&self->id) CStrId(base::global::CStrId::Empty());

    self->charge_start = 0.5f;
    self->charge_full = 1.0f;
    self->charging = false;
    self->charge = 0.0f;
    self->field_0x180 = 0;
    self->field_0x181 = 0;
    self->field_0x184 = -1.0f;
    self->field_0x188 = 0;
    self->field_0x189 = 0;
    self->field_0x18c = 0.0f;
    self->field_0x190 = 1.2f;
    self->field_0x194 = 0;
    for (CRntVector<void*>& list : self->lists)
        new (&list) CRntVector<void*>(0xf);
    self->field_0x210 = false;
    self->field_0x218 = false;

    self->listeners.vtable = g_unk_7101a77dc8 + 2;
    new (&self->listeners.added) CRntVector<void*>(0xf);
    new (&self->listeners.removed) CRntVector<void*>(0xf);
    new (&self->listeners.items) CRntVector<void*>(0xf);
    self->listeners.lock = 0;

    self->sound = g_unk_7108b6c460;
    self->field_0x2b8 = 0;
    self->field_0x2b9 = 0;

    self->tracks[0].Init(fn_7100cf0d38());  // CCharClassCantFireTrack
    self->tracks[1].Init(fn_7100cf6ef8());  // CCharClassFireActionTrack
    self->tracks[2].Init(fn_7100cf2ae8());  // CCharClassConvertingToMorphBallTrack
    self->tracks[3].Init(fn_7100d14d38());  // CCharClassInstantMorphballConversionTrack
}
