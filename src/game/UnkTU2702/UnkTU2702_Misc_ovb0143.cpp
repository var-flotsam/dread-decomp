// Unit 2702 (tools/tu_map.py, hint CAIComponent): small AI component helpers, run overnight batch b0143. Names and
// layouts are placeholders.
#include <new>

#include "base/UnkEmptyDtorElem.h"
#include "base/UnkMatrix44.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttack.h"
#include "game/CAttackComponent.h"
#include "game/CCharacterMovement.h"
#include "game/CCoreXAIComponent.h"
#include "game/CEnemyMovement.h"
#include "game/CFXComponent.h"
#include "game/CInputComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU108.h"
#include "game/UnkTU134.h"
#include "game/UnkTU2702.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkGuiComponent_b0143 {
    u8 pad_0x0[0x14578];
    UnkGuiKeyList keys;
};

void fn_7101105e24(UnkGuiComponent_b0143* self, bool add, base::global::CStrId* key) {
    if (add)
        fn_710002fd10(&self->keys, key);
    else
        fn_710002ffb4(&self->keys, key);
}

struct UnkVecOwner_b0143 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
    u8 pad_0x78[0x1a0];
    void* field_0x218;
};

struct UnkVecSelf_b0143 {
    u8 pad_0x0[0x30];
    UnkVecOwner_b0143* owner;
};

struct UnkVecSource_b0143 {
    void** vtable;
    UnkVec3f vf_0x370() { return reinterpret_cast<UnkVec3f (*)(UnkVecSource_b0143*)>(vtable[0x370 / 8])(this); }
};

UnkVec3f fn_710110f1c8(UnkVecSelf_b0143* self) {
    UnkVecOwner_b0143* owner = self->owner;
    if (owner->field_0x218)
        return static_cast<UnkVecSource_b0143*>(owner->field_0x218)->vf_0x370();
    const UnkMatrix44& m = fn_710002614c(&owner->xform);
    return UnkVec3f(m.m[2], m.m[6], m.m[10]);
}



struct UnkAIEntry_b0143 {
    u8 pad_0x0[8];
    void* field_0x8;
};

typedef u64 u64_a4 __attribute__((aligned(4)));
typedef UnkAIEntry_b0143* UnkAIEntryPtr_b0143 __attribute__((aligned(4)));

struct UnkAIList_b0143 {
    void** vtable;
    u8 pad_0x8[0x28];
    void* owner;  // 0x30
    u8 pad_0x38[0x11a8];
    u8 field_0x11e0;
    u8 pad_0x11e1[0x42f];
    void* field_0x1610;
    u32 count;  // 0x1618

    bool vf_0x198() { return reinterpret_cast<bool (*)(UnkAIList_b0143*)>(vtable[0x198 / 8])(this); }
};

bool fn_71010f9dac(UnkAIList_b0143* self) {
    // The slots past the named members are addressed from the raw bytes.
    u8* raw = reinterpret_cast<u8*>(self);
    __builtin_memset(raw + 0x111c, 0, 8);
    u64 size = (u64)self->count << 3;
    // Keeps the byte size as the emptiness test (the loop itself compares against a reloaded count).
    asm("" : "+r"(size));
    if (size != 0) {
        UnkAIEntryPtr_b0143* list = reinterpret_cast<UnkAIEntryPtr_b0143*>(raw + 0x161c);
        UnkAIEntryPtr_b0143* it = list;
        do {
            UnkAIEntry_b0143* entry = *it++;
            // Untyped copy: the owner and the count are read again after each store.
            __builtin_memcpy(&entry->field_0x8, &self->owner, 8);
        } while (it != list + self->count);
    }
    if (self->field_0x1610) {
        // Keeps the flag's address separate from the one of the first store.
        u8* flag = raw + 0x11e0;
        asm("" : "+r"(flag));
        *flag = self->vf_0x198();
    }
    return true;
}

struct UnkAICall_b0143 {
    void** vtable;
    u8 pad_0x8[0x1138];
    u32 field_0x1140;

    bool vf_0x468(void* a, u32 b) { return reinterpret_cast<bool (*)(UnkAICall_b0143*, void*, u32)>(vtable[0x468 / 8])(this, a, b); }
    void vf_0x470(void* a, u32 b, bool c) {
        reinterpret_cast<void (*)(UnkAICall_b0143*, void*, u32, bool)>(vtable[0x470 / 8])(this, a, b, c);
    }
};

struct UnkAIResult_b0143 {
    u8 pad_0x0[0x38];
    void** field_0x38;
};

void* fn_71010ffe3c(UnkAICall_b0143* self, UnkAIResult_b0143* arg, bool flag) {
    if (self->vf_0x468(arg, self->field_0x1140)) {
        self->vf_0x470(arg, self->field_0x1140, flag);
        if (arg->field_0x38)
            return *arg->field_0x38;
    }
    return nullptr;
}

struct UnkWeakTarget;
struct UnkTileOutline;

struct UnkAIReset_b0143 {
    u8 pad_0x0[0x30];
    UnkWeakTarget* owner;  // 0x30
    u8 pad_0x38[0x30];
    u8 field_0x68[8];
    u32 field_0x70;
    u8 pad_0x74[0x1c];
    u16 field_0x90;
    u8 pad_0x92[2];
    u8 field_0x94[2];
    u8 pad_0x96[0x22];
    u32 field_0xb8;
    u8 pad_0xbc[0x1c];
    u32* field_0xd8;
    u32 count;  // 0xe0
    u8 pad_0xe4[0x1c];
    u32 field_0x100;
    u8 pad_0x104[0x1004];
    u64 field_0x1108;
    u8 pad_0x1110[0x798];
    u8 field_0x18a8[1];
};

void fn_71010f9cf8(UnkAIReset_b0143* self) {
    UnkTileOutline* outline = reinterpret_cast<UnkTileOutline*>(self->field_0x68);
    *reinterpret_cast<u16*>(self->field_0x94) = 0x100;
    self->field_0x70 = 0;
    *reinterpret_cast<u32*>(&self->field_0x90) = 0;
    self->field_0xb8 = 0;
    for (u32 i = 0; i < self->count; ++i)
        fn_71002874dc(&self->field_0xd8[i]);
    self->count = 0;
    self->field_0x100 = 0;
    fn_7100119f98(outline, 0);
    self->field_0x1108 = 0;
    UnkWeakTarget** item = &self->owner;
    fn_7100874460(self->field_0x18a8, item);
}

struct UnkPart0;
extern char g_unk_7101cb2648[] __attribute__((visibility("hidden")));

struct UnkCloneNode_b0143 {
    void* vtable;
    u64 field_0x8;
    u64 field_0x10[5];
    u32 field_0x38;
    u8 field_0x3c;
    u8 field_0x3d;
    f32 bounds[4];  // 0x40
    u32 field_0x50;
};

UnkCloneNode_b0143* fn_71011161c8(UnkCloneNode_b0143* src) {
    UnkCloneNode_b0143* copy = new UnkCloneNode_b0143;
    copy->vtable = g_unk_7101cb2648;
    copy->field_0x8 = src->field_0x8;
    fn_710011d8b4(reinterpret_cast<UnkPart0*>(copy->field_0x10), reinterpret_cast<const UnkPart0*>(src->field_0x10));
    copy->field_0x38 = src->field_0x38;
    copy->field_0x3c = src->field_0x3c;
    copy->field_0x3d = 1;
    copy->bounds[0] = 3.4028235e38f;
    copy->bounds[1] = 3.4028235e38f;
    copy->bounds[2] = -3.4028235e38f;
    copy->bounds[3] = -3.4028235e38f;
    copy->field_0x50 = src->field_0x50;
    return copy;
}


bool fn_71009745f0(void*);

struct UnkMoveObject_b0143 {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkMoveObject_b0143*)>(vtable[0])(this);
    }
};

struct UnkMoveOwner_b0143 {
    u8 pad_0x0[0x1f0];
    UnkMoveObject_b0143* field_0x1f0;
    u8 pad_0x1f8[0x20];
    void* field_0x218;
};

struct UnkMoveSelf_b0143 {
    u8 pad_0x0[0x30];
    UnkMoveOwner_b0143* owner;
};

bool fn_7101110cb0(UnkMoveSelf_b0143* self) {
    void* animation = self->owner->field_0x218;
    if (animation) {
        if (fn_71010cc3a4(animation) == 4)
            return true;
    }
    UnkMoveObject_b0143* object = self->owner->field_0x1f0;
    if (!object)
        return false;
    if (fn_710009d50c(object->vf_0x0(), fn_71009b57c4()))
        return fn_71010cab0c(animation);
    return false;
}

struct UnkTrackOwner_b0143 {
    u8 pad_0x0[0x218];
    void* field_0x218;
    UnkAnimationComponent* field_0x220;
};

struct UnkTrackSelf_b0143 {
    u8 pad_0x0[0x30];
    UnkTrackOwner_b0143* owner;
    u8 pad_0x38[0x16ad];
    u8 field_0x16e5;
};

bool fn_71010faac0(UnkTrackSelf_b0143* self) {
    if (self->field_0x16e5)
        return true;
    if (UnkAnimationComponent* cls = self->owner->field_0x220) {
        if (fn_710117b348(cls, fn_7100d0b278()))
            return true;
    }
    u8* animation = static_cast<u8*>(self->owner->field_0x218);
    if (fn_71010cc3a4(animation) == 4)
        return fn_71009745f0(animation + 0x17a0);
    return false;
}

struct UnkTrackOwner2_b0143 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x30];
    void* field_0x258;
};

struct UnkTrackSelf2_b0143 {
    u8 pad_0x0[0x30];
    UnkTrackOwner2_b0143* owner;
    u8 pad_0x38[0x2a];
    bool field_0x62;
};

bool fn_7101110268(UnkTrackSelf2_b0143* self) {
    void* target = self->owner->field_0x258;
    if (target && !self->field_0x62) {
        if (fn_7101136308(target))
            return false;
    }
    UnkAnimationComponent* cls = self->owner->field_0x220;
    if (!cls)
        return false;
    if (fn_710117b348(cls, fn_7100cf3508()))
        return false;
    cls = self->owner->field_0x220;
    if (!cls)
        return false;
    if (fn_710117b348(cls, fn_7100d253f8()))
        return false;
    cls = self->owner->field_0x220;
    if (!cls)
        return false;
    return !fn_710117b348(cls, fn_7100c34568());
}



const u64* fn_7100360890(CAttack* attack);

typedef CAttack* UnkAttackPtr_b0143 __attribute__((aligned(4)));

struct UnkAttackListSelf_b0143 {
    u8 pad_0x0[0x1618];
    u32 count;  // 0x1618
};

static inline UnkAttackPtr_b0143* AttackList(UnkAttackListSelf_b0143* self) {
    return reinterpret_cast<UnkAttackPtr_b0143*>(reinterpret_cast<u8*>(self) + 0x161c);
}

struct UnkKeyedAttack_b0143 {
    u8 pad_0x0[0x20];
    u64 field_0x20;
};

static inline u64 NameKey(const u64* name) {
    auto* info = reinterpret_cast<base::UnkNamedClassInfo*>(*name);
    ++info->field_0x44;
    return *reinterpret_cast<u64*>(reinterpret_cast<u8*>(*name) + 0x30);
}

CAttack* fn_7101109df4(UnkAttackListSelf_b0143* self, UnkKeyedAttack_b0143* key) {
    u64 size = (u64)self->count << 3;
    // Keeps the byte size as the emptiness test.
    asm("" : "+r"(size));
    if (size != 0) {
        UnkAttackPtr_b0143* list = AttackList(self);
        UnkAttackPtr_b0143* it = list;
        do {
            if (NameKey(fn_7100360890(*it)) == key->field_0x20)
                return *it;
            ++it;
        } while (it != list + self->count);
    }
    return nullptr;
}

CAttack* fn_7101105148(UnkAttackListSelf_b0143* self, const char* name) {
    u64 size = (u64)self->count << 3;
    // Keeps the byte size as the emptiness test.
    asm("" : "+r"(size));
    if (size != 0) {
        UnkAttackPtr_b0143* list = AttackList(self);
        UnkAttackPtr_b0143* it = list;
        do {
            const u64* attackName = fn_7100360890(*it);
            u64 id, keyId;
            {
                base::global::CRntString key(name);
                id = NameKey(attackName);
                keyId = *reinterpret_cast<u64*>(reinterpret_cast<u8*>(&key) + 0x20);
            }
            if (id == keyId)
                return *it;
            ++it;
        } while (it != list + self->count);
    }
    return nullptr;
}


struct UnkCancelAttack_ovb0128;
void* fn_7100362324(UnkCancelAttack_ovb0128*, u32);
void fn_710110bc0c(void* self, void* arg, u32 id, u32 kind, s32 amount);

typedef UnkCancelAttack_ovb0128* UnkCancelPtr_b0143 __attribute__((aligned(4)));

struct UnkCancelSelf_b0143 {
    u8 pad_0x0[0x1618];
    u32 count;  // 0x1618
};

// Both callers also set a flag as a fourth argument; nothing here reads it (the call this ends with
// gets its own fourth argument).
void fn_710110bb4c(UnkCancelSelf_b0143* self, void* arg, u32 id, bool) {
    u32 total = 0;
    u64 size = (u64)self->count << 3;
    // Keeps the byte size as the emptiness test.
    asm("" : "+r"(size));
    if (size != 0) {
        UnkCancelPtr_b0143* list = reinterpret_cast<UnkCancelPtr_b0143*>(reinterpret_cast<u8*>(self) + 0x161c);
        UnkCancelPtr_b0143* it = list;
        do {
            if (u8* found = static_cast<u8*>(fn_7100362324(*it, id)))
                total += found[0x10];
            ++it;
        } while (it != list + self->count);
    }
    f32 v = (f32)total * 1.5f;
    f32 rounded = v > 0.0f ? __builtin_ceilf(v) : __builtin_floorf(v);
    fn_710110bc0c(self, arg, id, 2, (s32)rounded);
}


struct UnkObject;
struct UnkComponent;
struct UnkRinkaControl_ruc;
struct UnkPart;

struct UnkEffectOwner_b0143 {
    u8 pad_0x0[0x280];
    UnkObject* effects;  // 0x280
};

struct UnkEffectSelf_b0143 {
    u8 pad_0x0[0x30];
    UnkEffectOwner_b0143* owner;
    u8 pad_0x38[0x1764];
    u8 field_0x179c;
};

struct UnkEffectComponent_b0143 {
    u8 pad_0x0[0xb];
    u8 field_0xb;
};

void fn_710110cc04(UnkEffectSelf_b0143* self, bool both) {
    if (!self->field_0x179c)
        return;
    self->field_0x179c = 0;
    UnkObject* effects = self->owner->effects;
    auto* component = reinterpret_cast<UnkEffectComponent_b0143*>(fn_7101152b90(reinterpret_cast<UnkWorld*>(effects),
                                                                                *reinterpret_cast<u64*>(g_unk_7172a062e0 + 0x30)));
    if (!component)
        return;
    if (!fn_710114e668(effects, component->field_0xb))
        return;
    fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(component), false);
    if (both)
        fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(component), false);
    fn_710115596c(reinterpret_cast<UnkRinkaControl_ruc*>(effects), component);
}


using base::reflection::UnkReflObject;

void XParasiteAI_SetPrey(UnkReflObject* component, UnkActor* owner);
void fn_71007c3a38(char* actor, void* a, void* b, void* c, u32 d, bool e);

struct UnkTargetRef_b0143 {
    u8 pad_0x0[0x10];
    UnkActor* field_0x10;
};

struct UnkSwarmSelf_b0143 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x1578];
    UnkTargetRef_b0143* field_0x15b0;
    u8 pad_0x15b8[0x1c];
    u32 field_0x15d4;
};

void fn_710110e278(UnkSwarmSelf_b0143* self, void* a, void* b, void* c, bool flag) {
    if (self->field_0x15b0 && self->field_0x15b0->field_0x10) {
        UnkReflObject* component = static_cast<UnkReflObject*>(self->field_0x15b0->field_0x10->field_0x240);
        if (!component)
            return;
        if (fn_710009d50c(component->vf_0x0(), fn_71006e5f28()))
            XParasiteAI_SetPrey(component, self->owner);
        return;
    }
    if (char* actor = fn_71004dad10(self->owner))
        fn_71007c3a38(actor, a, b, c, self->field_0x15d4, flag);
}


struct UnkVec2_b0143 {
    f32 x, y;
    UnkVec2_b0143() {}
    UnkVec2_b0143(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkVec2_b0143(const UnkVec2_b0143& o) : x(o.x), y(o.y) {}
};

struct UnkBounds_b0143 {
    f32 min_x, min_y, min_z;
    f32 max_x, max_y, max_z;
};

struct UnkBoundsSource_b0143 {
    void** vtable;
    UnkBounds_b0143 vf_0x28(void* arg) {
        return reinterpret_cast<UnkBounds_b0143 (*)(UnkBoundsSource_b0143*, void*)>(vtable[0x28 / 8])(this, arg);
    }
};

struct UnkBoundsPart_b0143 {
    u8 pad_0x0[0x158];
    UnkBoundsSource_b0143* field_0x158;
};

struct UnkBoundsHolder_b0143 {
    u8 pad_0x0[0x3b8];
    UnkBoundsPart_b0143* field_0x3b8;
    UnkBoundsPart_b0143* field_0x3c0;
};

struct UnkBoundsOwner_b0143 {
    u8 pad_0x0[0x218];
    UnkBoundsHolder_b0143* field_0x218;
};

struct UnkBoundsShift_b0143 {
    u8 pad_0x0[0x3a4];
    f32 field_0x3a4;
    f32 field_0x3a8;
};

struct UnkBoundsSelf_b0143 {
    u8 pad_0x0[0x30];
    UnkBoundsOwner_b0143* owner;
    u8 pad_0x38[0x15d8];
    UnkBoundsShift_b0143* field_0x1610;
};

UnkVec2_b0143 fn_710110f254(UnkBoundsSelf_b0143* self) {
    UnkVec2_b0143 result;
    UnkBoundsHolder_b0143* holder = self->owner->field_0x218;
    UnkBoundsPart_b0143* part = nullptr;
    if (holder) {
        part = holder->field_0x3c0;
        if (!part)
            part = holder->field_0x3b8;
    }
    if (!part || !part->field_0x158) {
        result.x = 0.0f;
        result.y = 0.0f;
        return result;
    }
    UnkBoundsSource_b0143* source = part->field_0x158;
    UnkBounds_b0143 bounds = source->vf_0x28(reinterpret_cast<u8*>(source) + 8);
    base::math::CVector3D d = {bounds.max_x - bounds.min_x, bounds.max_y - bounds.min_y, bounds.max_z - bounds.min_z};
    fn_710007b430(reinterpret_cast<base::math::CVector2D*>(&result), &d);
    if (UnkBoundsShift_b0143* shift = self->field_0x1610) {
        result.x = shift->field_0x3a4 + result.x;
        result.y = shift->field_0x3a8 + result.y;
    }
    return result;
}
