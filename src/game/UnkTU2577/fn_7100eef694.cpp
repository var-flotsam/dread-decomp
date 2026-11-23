// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100eef694 tells whether the
// player's shape touches the logic shape of the object named "LS_EmmyZone": the object is looked up by its id
// in the game manager, its logic shape component is found (fn_7100a48b60), and the player's shape
// (fn_7100976588 of the player's field at +0x1f8) is tested against it with the shape tester (created on first
// use) with the identity transform. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CCharacterMovement.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/UnkColliderResult.h"
#include "types.h"


struct UnkRefl_fn_7100a48b60;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_7100a48b60>(void*);

struct UnkZoneLogicShape_lef694 {
    u8 pad_0x0[0x30];
    void* field_0x30;
};

struct UnkPlayerShape_lef694 {
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
    void* field_0x158;
};
static_assert(__builtin_offsetof(UnkPlayerShape_lef694, field_0x158) == 0x158, "layout");

struct UnkPlayerView_lef694 {
    u8 pad_0x0[0x1f8];
    UnkFlagged* field_0x1f8;
};

static inline UnkShapeTester_A5_b68* GetTester_lef694() {
    if (!g_unk_71726a47a0) {
        UnkShapeTester_A5_b68* t = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(t);
        g_unk_71726a47a0 = t;
    }
    return g_unk_71726a47a0;
}

// The object registered under a name with the given kind.
static inline void* FindObject_lef694(UnkGameManager* gm, const char* name, u64 length, u32 kind) {
    u64 id = StringCrc64(name, length);
    alignas(8) u32 flags = kind;  // in an 8-byte slot, as the other callers of fn_7100deba8c
    return fn_7100deba8c(gm, &id, &flags);
}

bool fn_7100eef694() {
    UnkGameManager* gm = g_unk_7101d47310;
    void* zone = FindObject_lef694(gm, "LS_EmmyZone", 11, g_unk_71727bebb8);
    if (!zone)
        return false;
    UnkZoneLogicShape_lef694* logic =
        reinterpret_cast<UnkZoneLogicShape_lef694*>(base::reflection::refl_FindTyped<UnkRefl_fn_7100a48b60>(zone));
    void* other = logic->field_0x30;
    UnkPlayerView_lef694* player = reinterpret_cast<UnkPlayerView_lef694*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return false;
    if (player->field_0x1f8) {
        UnkPlayerShape_lef694* shape = reinterpret_cast<UnkPlayerShape_lef694*>(fn_7100976588(player->field_0x1f8));
        void* owner = shape->field_0x158;
        UnkShapeTester_A5_b68* tester = GetTester_lef694();
        UnkColliderResult result = fn_7100d51368(tester, owner, shape->vf_0x70(), other, g_unk_7101a1a680, nullptr, 0);
        if (result.bits & 1)
            return true;
    }
    return false;
}
