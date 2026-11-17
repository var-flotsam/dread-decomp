// CPlayerLifeComponent's unit (2716): applies two upgrades the archive blackboard has and the game blackboard
// does not have yet: marks each as applied there, then raises the maximum life by 100, or adds 10 to the
// missile maximum and current counts. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CBossAIComponent.h"
#include "game/CInventoryComponent.h"
#include "game/CLifeComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU2553.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkModeFlags {
    u8 unk_0x0[0xb8];
    bool field_0xb8;
};

struct UnkLifeChange_ovb0000 {
    f32 life;
    bool increased;
    UnkLifeChange_ovb0000(f32 life_, bool increased_) : life(life_), increased(increased_) {}
};

struct UnkStrIdHandle;
void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);
void fn_7101144104(UnkInventory* inventory, const UnkQualifiedId& item, f32 amount, bool flag);

// CPlayerLifeComponent_Life_ovb0049.cpp's UnkPlayerLife_ovb0049 (its caller) seen with the fields used here;
// its layout there does not name the change signal at 0x118 yet.
struct UnkPlayerLife_ovb0049;
struct UnkPlayerLife_l165180 {
    void** vtable;
    u8 pad_0x8[0x48];
    f32 max_life;  // 0x50
    f32 life;      // 0x54
    u8 pad_0x58[0xc0];
    u8 on_change[0x8];  // 0x118

    void vf_0x170(f32 value) {
        reinterpret_cast<void (*)(UnkPlayerLife_l165180*, f32)>(vtable[0x170 / 8])(this, value);
    }
};

struct UnkPlayerItems_l165180 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;  // 0x270
};

// An item's qualified id as fn_7100ae986c builds it in place; only the id is released.
struct UnkItemHandle_l165180 {
    UnkStringData* id;
    void* object;

    explicit UnkItemHandle_l165180(const CStrId& name) {
        fn_7100ae986c(reinterpret_cast<UnkStrIdHandle*>(this), name);
    }
    ~UnkItemHandle_l165180() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (id && id != &pool->empty) {
                --id->ref_count;
                if (id->ref_count == 0) {
                    fn_71000803a4(pool, id);
                    pool = g_unk_7101d47108;
                }
            }
            id = &pool->empty;
        }
    }
    const UnkQualifiedId& Id() const { return *reinterpret_cast<const UnkQualifiedId*>(this); }
};

static inline const CStrId& Section() {
    return base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0xb4];
}

static inline const UnkQualifiedId& DefId(UnkPropertyEx<>& def) {
    UnkQualifiedId& qid = ItemQualifiedId(def);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(def), g_unk_7101d470e0);
    return qid;
}

// Reads the flag `name` of the section `section` into `out` (left as it is when the section is missing).
static inline void GetFlag(CBlackboard* blackboard, const CStrId& section, const UnkQualifiedId& name, bool* out) {
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        fn_710006d11c(s, &name.id, out);
}

// Sets the flag `name` of the section `section`, unless the blackboard is locked.
static inline void SetFlag(CBlackboard* blackboard, const CStrId& section, const UnkQualifiedId& name) {
    bool value = true;
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = base::reflection::UnkReflTypeOf<bool>::Get();
            v.value = &value;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)), &v);
        }
    }
}

void fn_7101165180(UnkPlayerLife_ovb0049* component) {
    auto* self = reinterpret_cast<UnkPlayerLife_l165180*>(component);
    if (g_unk_71726bc570->field_0xb8)
        return;
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* archive = &manager->field_0x820;
    CBlackboard* game = &manager->field_0x968;

    bool archived = false;
    bool applied = false;
    GetFlag(archive, Section(), DefId(g_unk_717275b108), &archived);
    GetFlag(game, Section(), DefId(g_unk_717275b108), &applied);
    if (archived && !applied) {
        SetFlag(game, Section(), DefId(g_unk_717275b108));
        self->vf_0x170(self->max_life + 100.0f);
        fn_7101160348(self, 100.0f);
        UnkLifeChange_ovb0000 change(self->life, false);
        fn_710116179c(self->on_change, &change);
    }

    bool missiles_archived = false;
    bool missiles_applied = false;
    GetFlag(archive, Section(), DefId(g_unk_717275b148), &missiles_archived);
    GetFlag(game, Section(), DefId(g_unk_717275b148), &missiles_applied);
    if (missiles_archived && !missiles_applied) {
        SetFlag(game, Section(), DefId(g_unk_717275b148));
        CStrId max_name("ITEM_WEAPON_MISSILE_MAX", true);
        CStrId current_name("ITEM_WEAPON_MISSILE_CURRENT", true);
        UnkItemHandle_l165180 max(max_name);
        UnkItemHandle_l165180 current(current_name);
        if (UnkPlayer* player = fn_7100dec788(g_unk_7101d47310)) {
            if (UnkInventory* inventory = reinterpret_cast<UnkPlayerItems_l165180*>(player)->inventory) {
                fn_7101144104(inventory, max.Id(), 10.0f, true);
                fn_7101144104(inventory, current.Id(), 10.0f, true);
            }
        }
    }
}
