// CSamusGunComponent unit (unit 1315 of tools/tu_map.py): tells the callback set at +0x198 `true`, then,
// when the owner's animation component is a CSamusAnimationComponent, sends it a 0x80 under the id of
// fn_710085a1e8 or fn_710085961c, picked by `aiming` and the autofocus camera. Names and layouts are
// placeholders.
#include "base/UnkDeferredSet.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CCameraComponent.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::UnkDeferredCallback;
using base::UnkDeferredSet;
using base::reflection::UnkReflObject;

struct UnkDesc_SpitSend;
extern UnkDesc_SpitSend g_unk_710164b958;  // reached through the GOT

struct UnkSamusGunOwner_l859e2c {
    u8 pad_0x0[0x220];
    UnkReflObject* animation;  // 0x220
    u8 pad_0x228[0x48];
    UnkInventory* inventory;  // 0x270
};

struct UnkSamusGun_l859e2c {
    u8 pad_0x0[0x30];
    UnkSamusGunOwner_l859e2c* owner;  // 0x30
    u8 pad_0x38[0x160];
    UnkDeferredSet<UnkDeferredCallback<bool>> field_0x198;
    u8 pad_0x198[0x384 - 0x198 - sizeof(UnkDeferredSet<UnkDeferredCallback<bool>>)];
    f32 field_0x384;
};
static_assert(__builtin_offsetof(UnkSamusGun_l859e2c, field_0x384) == 0x384, "layout");

// The autofocus aim camera controller: its flag at +0x240.
struct UnkAutofocusCtrl_l859e2c : UnkReflObject {
    u8 pad_0x8[0x238];
    bool field_0x240;
};

u64 fn_710085a1e8(UnkSamusGun_l859e2c* self);
u64 CSamusGunComponent_GetPrimaryWeaponKey(UnkSamusGun_l859e2c* self);

static inline UnkQualifiedId& Qid_l859e2c(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// Whether the camera is the autofocus aim camera with its flag clear, for an owner holding the central
// unit's energy.
static inline bool AutofocusFree(UnkInventory* inventory) {
    if (!inventory || !fn_71011428bc(inventory, Qid_l859e2c(g_unk_717275ac18)))
        return false;
    void* camera = fn_7100defcec(g_unk_7101d47310);
    if (!camera)
        return false;
    UnkReflObject* ctrl = fn_71010e37f4(camera);
    if (!ctrl || !fn_710009d50c(ctrl->vf_0x0(), fn_7100720464()))
        return false;
    return !static_cast<UnkAutofocusCtrl_l859e2c*>(ctrl)->field_0x240;
}

void fn_7100859e2c(UnkSamusGun_l859e2c* self, bool aiming) {
    using base::reflection::UnkReflTypeId;
    using base::reflection::UnkReflTypeIdArg;
    using base::reflection::UnkReflTypeIdTag;
    bool arg = true;
    fn_71000303c0(&self->field_0x198, &arg);
    UnkReflObject* animation = self->owner->animation;
    if (!animation || !fn_710009d50c(animation->vf_0x0(), fn_7100713bb0()))
        return;
    u64 hash;
    if (!aiming)
        goto idle;
    self->field_0x384 = 0.0f;
    if (!AutofocusFree(self->owner->inventory))
        goto aim;
    {
        UnkReflTypeIdArg focus_value, focus_type;
        hash = fn_710085a1e8(self);
        focus_value.id = 0x80;
        focus_type.id = UnkReflTypeId<UnkReflTypeIdTag>::id();
        fn_7101176690(animation, hash, 0x5d523009b8f43e5f, &focus_value, &g_unk_710164b958, &g_unk_7172769740,
                      &focus_type);
    }
    return;
    // The gotos skip the declarations below, so they get no lifetime markers and keep slots of their own.
    UnkReflTypeIdArg aim_value, aim_type;
    UnkReflTypeIdArg idle_value, idle_type;
aim:
    hash = CSamusGunComponent_GetPrimaryWeaponKey(self);
    aim_value.id = 0x80;
    aim_type.id = UnkReflTypeId<UnkReflTypeIdTag>::id();
    fn_7101176690(animation, hash, 0x5d523009b8f43e5f, &aim_value, &g_unk_710164b958, &g_unk_717276974c,
                  &aim_type);
    return;
idle:
    self->field_0x384 = 0.25f;
    hash = fn_710085a1e8(self);
    idle_value.id = 0x80;
    idle_type.id = UnkReflTypeId<UnkReflTypeIdTag>::id();
    fn_7101176690(animation, hash, 0x5d523009b8f43e5f, &idle_value, &g_unk_710164b958, &g_unk_7172769740,
                  &idle_type);
}
