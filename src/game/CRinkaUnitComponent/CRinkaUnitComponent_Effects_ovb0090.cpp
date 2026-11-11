// CRinkaUnitComponent's unit (tools/tu_map.py unit 981). The destructor (fn_710065f250): the vtable, the
// counted object at +0x260, the member at +0x1a8 (vtable 0x7101a5be58, its words cleared), the three
// value holders at +0x138, +0xc8 and +0x58 (the layout of CSensorDoorComponent's), then the component
// destructor fn_71010c4194. The effect paths picked by the unit's kind (+0x48: 0, 1, 2 for a, b, c):
// spawning loop (fn_710065f4a8) and init loop (fn_710065f690). The init-loop spawn (fn_710066036c):
// clears +0x258 and, when the actor has its control object (+0x280), creates the effect through the
// model manager, places it at the actor's position and angles, enables it and hands it to the control
// with the unit's entry table g_unk_71726fd110. Names and layouts are placeholders.
#include <new>

#include "base/UnkAtExit.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CFXComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkModelManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU285.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CFilePathStrId;

extern void* g_unk_7101a5bc30[];  // the component's vtable
extern void* g_unk_7101a5be00[];  // the value holders' vtable
extern void* g_unk_7101a5be58[];  // the member's vtable

struct UnkRinkaPair_ovb0090 {
    u64 a, b;
    ~UnkRinkaPair_ovb0090() {
        a = 0;
        b = 0;
    }
};

struct UnkRinkaValueHolderBase_ovb0090 {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
    base::reflection::UnkReflValue value;
    ~UnkRinkaValueHolderBase_ovb0090() {
        vtable = reinterpret_cast<char*>(g_unk_7101a5be00) + 0x10;
        fn_71000a1c60(&value);
    }
};

struct UnkRinkaValueHolder_ovb0090 : UnkRinkaValueHolderBase_ovb0090 {
    UnkRinkaPair_ovb0090 pairs[4];
};
static_assert(sizeof(UnkRinkaValueHolder_ovb0090) == 0x70, "value holder layout");

struct UnkRinkaMember_ovb0090 {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
    ~UnkRinkaMember_ovb0090() {
        vtable = reinterpret_cast<char*>(g_unk_7101a5be58) + 0x10;
        field_0x10 = 0;
        field_0x18 = 0;
        field_0x20 = 0;
    }
};

struct UnkRinkaUnitBase_ovb0090 : UnkBaseComponent {
    ~UnkRinkaUnitBase_ovb0090() { fn_71010c4194(this); }
};

struct UnkRinkaUnitComponent_ovb0090 : UnkRinkaUnitBase_ovb0090 {
    u8 pad_0x40[0x18];
    UnkRinkaValueHolder_ovb0090 field_0x58;
    UnkRinkaValueHolder_ovb0090 field_0xc8;
    UnkRinkaValueHolder_ovb0090 field_0x138;
    UnkRinkaMember_ovb0090 field_0x1a8;
    u8 pad_0x1d0[0x260 - 0x1d0];
    base::UnkRefPtrMember field_0x260;
    ~UnkRinkaUnitComponent_ovb0090() { vtable = g_unk_7101a5bc30 + 2; }
};
static_assert(__builtin_offsetof(UnkRinkaUnitComponent_ovb0090, field_0x260) == 0x260, "layout");

void fn_710065f250(UnkRinkaUnitComponent_ovb0090* self) {
    self->~UnkRinkaUnitComponent_ovb0090();
}

struct UnkRinkaUnitKind_ovb0090 {
    u8 pad_0x0[0x48];
    s32 kind;  // 0x48: 0, 1 or 2 picks the a, b or c effect
};

const CFilePathStrId* fn_710065f4a8(UnkRinkaUnitKind_ovb0090* self) {
    static const CFilePathStrId a("actors/props/rinkaunit/fx/rinkaspawningloopa.bcptl", true);
    static const CFilePathStrId b("actors/props/rinkaunit/fx/rinkaspawningloopb.bcptl", true);
    static const CFilePathStrId c("actors/props/rinkaunit/fx/rinkaspawningloopc.bcptl", true);
    s32 kind = self->kind;
    const CFilePathStrId* path = kind == 1 ? &b : &a;
    return kind == 2 ? &c : path;
}

__attribute__((noinline)) const CFilePathStrId* fn_710065f690(UnkRinkaUnitKind_ovb0090* self) {
    static const CFilePathStrId a("actors/props/rinkaunit/fx/rinka_initloopa.bcptl", true);
    static const CFilePathStrId b("actors/props/rinkaunit/fx/rinka_initloopb.bcptl", true);
    static const CFilePathStrId c("actors/props/rinkaunit/fx/rinka_initloopc.bcptl", true);
    s32 kind = self->kind;
    const CFilePathStrId* path = kind == 1 ? &b : &a;
    return kind == 2 ? &c : path;
}

struct UnkFxObject_ovb0041;
UnkFxObject_ovb0041* fn_71003b7db0(UnkModelManager* self, const void* path, bool a, bool b);
void fn_7100146dac(UnkSwitchObject* self, bool enable);
void fn_7100146d84(UnkFxObject_ovb0041* fx, const UnkVec3f* position);
void fn_7100146d98(UnkFxObject_ovb0041* fx, const UnkVec3f* angle);

struct UnkRinkaEntry_ruc;
extern UnkRinkaEntry_ruc* g_unk_71726fd110[2] __attribute__((visibility("hidden")));

struct UnkRinkaControl_ovb0090;

struct UnkRinkaActor_ovb0090 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x280 - 0x94];
    UnkRinkaControl_ovb0090* control;  // 0x280
};

struct UnkRinkaUnitSpawn_ovb0090 {
    u8 pad_0x0[0x30];
    UnkRinkaActor_ovb0090* actor;  // 0x30
    u8 pad_0x38[0x220];
    u32 field_0x258;
    u8 pad_0x25c[0xc];
    UnkFxObject_ovb0041* fx;  // 0x268
};

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

void fn_710066036c(UnkRinkaUnitSpawn_ovb0090* self) {
    UnkRinkaActor_ovb0090* actor = self->actor;
    self->field_0x258 = 0;
    if (!actor->control)
        return;
    UnkModelManager* manager = ModelManager();
    UnkFxObject_ovb0041* fx =
        fn_71003b7db0(manager, fn_710065f690(reinterpret_cast<UnkRinkaUnitKind_ovb0090*>(self)), false, true);
    self->fx = fx;
    if (!fx)
        return;
    {
        UnkVec3f position = self->actor->xform.GetTranslation();
        fn_7100146d84(fx, &position);
    }
    {
        UnkRinkaActor_ovb0090* owner = self->actor;
        UnkFxObject_ovb0041* target = self->fx;
        UnkVec3f angle = owner->xform.GetAngle();
        fn_7100146d98(target, &angle);
    }
    fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(self->fx), true);
    fn_7100208a40(reinterpret_cast<char*>(self->fx) + 0x28);
    fn_7101152988(self->actor->control, self->fx, g_unk_71726fd110, nullptr, 0);
}
