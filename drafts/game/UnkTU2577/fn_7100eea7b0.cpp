// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100eea7b0 tells whether an
// actor of the current sub-area that the manager tracks touches the logic shape of "LS_EmmyZone" with the
// centre of its bounds. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkGameManager.h"
#include "game/UnkShapeHit_A5_b68.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/UnkColliderResult.h"
#include "types.h"

struct UnkRefl_fn_7100a48b60;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_7100a48b60>(void*);

struct UnkZoneLogicShape_leea7b0 {
    u8 pad_0x0[0x30];
    void* field_0x30;
};

struct UnkBounds_leea7b0 {
    base::math::CVector3D min;
    base::math::CVector3D max;
    f32 CentreX() const { return (min.x + max.x) * 0.5f; }
    f32 CentreY() const { return (min.y + max.y) * 0.5f; }
};

struct UnkBoundsOwner_leea7b0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkBounds_leea7b0 GetBounds(void* transform);
    u8 field_0x8[8];
    UnkBounds_leea7b0 Bounds() { return GetBounds(field_0x8); }
};

struct UnkZoneActor_leea7b0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void* vf_0x70();
    u8 pad_0x8[0x150];
    UnkBoundsOwner_leea7b0* field_0x158;
};
static_assert(__builtin_offsetof(UnkZoneActor_leea7b0, field_0x158) == 0x158, "layout");

// A reference-counted holder of a tracked actor.
struct UnkActorRef_leea7b0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Destroy();
    u32 refs;
    UnkZoneActor_leea7b0* actor;
};

struct UnkActorRefPtr_leea7b0 {
    UnkActorRef_leea7b0* ptr;
    explicit UnkActorRefPtr_leea7b0(UnkActorRef_leea7b0* p) : ptr(p) { ++ptr->refs; }
    ~UnkActorRefPtr_leea7b0() {
        if (--ptr->refs == 0)
            ptr->Destroy();
    }
    UnkActorRef_leea7b0* operator->() const { return ptr; }
};

struct UnkTrackedKey_leea7b0 {
    u8 pad_0x0[0x30];
    u32 field_0x30;
    u32 field_0x34;
};

struct UnkTrackedNode_leea7b0 {
    UnkTrackedKey_leea7b0* key;
    UnkActorRef_leea7b0* value;
    u8 pad_0x10[0x10];
    UnkTrackedNode_leea7b0* next;
};

// The manager's actors by key: chained buckets hashed by the key's two words.
struct UnkTrackedMap_leea7b0 {
    UnkTrackedNode_leea7b0** buckets;
    u32 count;
    u32 bucket_count;

    UnkTrackedNode_leea7b0* Find(UnkTrackedKey_leea7b0* key) const {
        if (!count)
            return nullptr;
        UnkTrackedNode_leea7b0** slot = &buckets[(key->field_0x30 ^ key->field_0x34) % bucket_count];
        if (!slot)
            return nullptr;
        for (UnkTrackedNode_leea7b0* node = *slot; node; node = node->next) {
            if (node->key == key)
                return node;
        }
        return nullptr;
    }
    bool Contains(UnkTrackedKey_leea7b0* key) const { return Find(key) != nullptr; }
};

struct UnkSubAreaKeys_leea7b0 {
    u8 pad_0x0[0xb0];
    UnkTrackedKey_leea7b0** data;
    u32 count;
};

struct UnkSubAreaMgr_leea7b0 {
    u8 pad_0x0[0x3f0];
    UnkSubAreaKeys_leea7b0* field_0x3f0;
    u8 pad_0x3f8[0x8e8 - 0x3f8];
    UnkTrackedMap_leea7b0 actors;
};
static_assert(__builtin_offsetof(UnkSubAreaMgr_leea7b0, actors) == 0x8e8, "layout");

static inline base::math::CVector2D MakeVector2D(f32 x, f32 y) {
    base::math::CVector2D v;
    v.x = x;
    v.y = y;
    return v;
}

static inline UnkShapeTester_A5_b68* GetTester_leea7b0() {
    if (!g_unk_71726a47a0) {
        UnkShapeTester_A5_b68* t = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(t);
        g_unk_71726a47a0 = t;
    }
    return g_unk_71726a47a0;
}

// The object registered under a name with the given kind.
static inline void* FindObject_leea7b0(UnkGameManager* gm, const char* name, u64 length, u32 kind) {
    u64 id = StringCrc64(name, length);
    alignas(8) u32 flags = kind;  // in an 8-byte slot, as the other callers of fn_7100deba8c
    return fn_7100deba8c(gm, &id, &flags);
}

bool fn_7100eea7b0(UnkSubAreaMgr_leea7b0* self) {
    UnkGameManager* gm = g_unk_7101d47310;
    void* zone = FindObject_leea7b0(gm, "LS_EmmyZone", 11, g_unk_71727bebb8);
    if (!zone || !self->field_0x3f0)
        return false;
    UnkZoneLogicShape_leea7b0* logic =
        reinterpret_cast<UnkZoneLogicShape_leea7b0*>(base::reflection::refl_FindTyped<UnkRefl_fn_7100a48b60>(zone));
    UnkSubAreaKeys_leea7b0* keys = self->field_0x3f0;
    bool touches = false;
    void* other = logic->field_0x30;
    UnkTrackedKey_leea7b0** end = keys->data + keys->count;
    for (UnkTrackedKey_leea7b0** it = keys->data; it != end; ++it) {
        if (!self->actors.Contains(*it))
            continue;
        UnkActorRefPtr_leea7b0 ref(self->actors.Find(*it)->value);
        UnkBoundsOwner_leea7b0* owner = ref->actor->field_0x158;
        UnkShapeTester_A5_b68* tester = GetTester_leea7b0();
        UnkColliderResult result = fn_7100d51368(tester, owner, ref->actor->vf_0x70(), other, g_unk_7101a1a680, nullptr, 0);
        touches = false;
        if (result.bits & 1) {
            base::math::CVector2D centre = MakeVector2D(owner->Bounds().CentreX(), owner->Bounds().CentreY());
            UnkShapeHit_A5_b68 hit = fn_7100d572f8(tester, other, &centre, &g_unk_710164779c, 0);
            touches = hit.flags & 1;
        }
        if (touches)
            return true;
    }
    return touches;
}
