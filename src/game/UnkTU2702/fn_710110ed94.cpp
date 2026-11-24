// UnkTU2702 (CAIComponent's unit): points the owner's "dissolve" and "dissolveinternal" parts against the
// death direction, then places and turns the death effects of the unit's table (at the death position or
// at the owner plus an offset), tinting "deathparticles_melee" by the player's best suit.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "game/CFXComponent.h"
#include "game/CInventoryComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU134.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU2712.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector4D;
using base::math::UnkVector4D;

struct UnkPlayer;
struct UnkInventory;

// The origin the death effects are turned about.
extern const f32 g_unk_710164cf5c[3] __attribute__((visibility("hidden")));
// The melee death particles' colours: no suit, the varia suit, the gravity and hyper suits (one constant).
extern const CVector4D g_unk_7101647b9c __attribute__((visibility("hidden")));
extern const CVector4D g_unk_7101647c1c __attribute__((visibility("hidden")));
extern const CVector4D g_unk_7101647bfc __attribute__((visibility("hidden")));

// One death effect of the unit's statics: the part's name and where it goes (1 the death position, 2 the
// owner's position plus the offset).
struct UnkDeathFx_l10ed94 {
    base::global::CStrId name;
    s32 placement;
    u8 pad_0xc[0x4];
};

struct UnkDeathFxStatics_l10ed94 {
    u8 pad_0x0[0x140];
    UnkDeathFx_l10ed94 effects[10];
};
struct UnkStatics_7101116680;  // the unit's statics, as its initialiser fn_7101116680 lays them out

struct UnkFxSlot_l10ed94 {
    u8 pad_0x0[0x30];
    CVector4D* value;
};

struct UnkFxMaterial_l10ed94 {
    u8 pad_0x0[0x15c];
    CVector4D direction;  // 0x15c
    u8 pad_0x16c[0x8c];
    UnkFxSlot_l10ed94* slot;  // 0x1f8
};

struct UnkFxModel_l10ed94 {
    u8 pad_0x0[0x88];
    UnkFxMaterial_l10ed94* material;
};

struct UnkFxPart_l10ed94 {
    u8 pad_0x0[0xb];
    u8 id;  // 0xb
    u8 pad_0xc[0x144];
    UnkFxModel_l10ed94* model;  // 0x150
};

struct UnkDeathPlayer_l10ed94 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;
};

struct UnkDeathAI_l10ed94 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x14634 - 0x38];
    UnkVec3f position;   // 0x14634
    UnkVec3f offset;     // 0x14640
    UnkVec3f direction;  // 0x1464c
};
static_assert(__builtin_offsetof(UnkDeathAI_l10ed94, direction) == 0x1464c, "layout");

static inline UnkFxPart_l10ed94* FindPart(void* parts, u64 hash) {
    return reinterpret_cast<UnkFxPart_l10ed94*>(fn_7101152b90(static_cast<UnkWorld*>(parts), hash));
}

// Points the named part's material against `dir`.
static inline void PointDissolve(void* parts, const char* name, u64 length, const UnkVec3f& dir) {
    UnkFxPart_l10ed94* part = FindPart(parts, StringCrc64(name, length));
    if (part && fn_710114e674(static_cast<UnkObject*>(parts), part->id)) {
        if (UnkFxMaterial_l10ed94* material = part->model->material) {
            UnkVector4D v(-dir.x, -dir.y, -dir.z, 0.0f);
            fn_710007b518(&material->direction, &v);
            if (material->slot)
                *material->slot->value = v;
        }
    }
}

// An item's qualified id, built on first use.
static inline const UnkQualifiedId& ItemId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// The colour for the varia and gravity suits' amounts.
static inline const CVector4D& SuitColour(f32 varia, f32 gravity) {
    if (gravity > 0.0f)
        return g_unk_7101647bfc;
    if (varia > 0.0f)
        return g_unk_7101647c1c;
    return g_unk_7101647b9c;
}

static inline UnkVec3f Cross(const UnkVec3f& a, const UnkVec3f& b) {
    return UnkVec3f(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

void fn_710110ed94(UnkDeathAI_l10ed94* self) {
    void* parts = self->owner->field_0x280;
    if (!parts)
        return;
    PointDissolve(parts, "dissolve", 8, self->direction);
    PointDissolve(parts, "dissolveinternal", 16, self->direction);

    UnkVec3f dir = self->direction;
    UnkVec3f up(0.0f, 0.0f, 1.0f);
    UnkVec3f side = Cross(dir, up);
    UnkMatrix44 m;
    fn_71000769f0(&m, &side, &dir, &up, g_unk_710164cf5c);
    UnkVec3f angle = fn_7100076acc(reinterpret_cast<const UnkXform*>(&m));

    for (u32 i = 0; i < 10; ++i) {
        UnkDeathFx_l10ed94& fx = reinterpret_cast<UnkDeathFxStatics_l10ed94&>(g_unk_7172a062c0).effects[i];
        UnkFxPart_l10ed94* part = FindPart(parts, fx.name.data->Key());
        if (!part || !fn_710114e668(static_cast<UnkObject*>(parts), part->id))
            continue;
        UnkFxObject_ovb0041* object = reinterpret_cast<UnkFxObject_ovb0041*>(part);
        fn_7100146d98(object, &angle);
        switch (fx.placement) {
        case 1:
            fn_7100146d84(object, &self->position);
            break;
        case 2: {
            UnkActor* owner = self->owner;
            UnkVec3f pos(owner->field_0x44 + self->offset.x, owner->field_0x54 + self->offset.y,
                         owner->field_0x64 + self->offset.z);
            fn_7100146d84(object, &pos);
            break;
        }
        }
        u64 melee = StringCrc64("deathparticles_melee", 20);
        if (fx.name.data->Key() != melee)
            continue;
        auto* player = reinterpret_cast<UnkDeathPlayer_l10ed94*>(fn_7100dec788(g_unk_7101d47310));
        if (!player)
            continue;
        UnkInventory* inventory = player->inventory;
        if (!inventory)
            continue;
        f32 varia = fn_7101142928(inventory, ItemId(g_unk_717275a498));
        f32 gravity = fn_7101142928(inventory, ItemId(g_unk_717275a458));
        f32 hyper = fn_7101142928(inventory, ItemId(g_unk_717275a418));
        UnkVector4D c(hyper > 0.0f ? g_unk_7101647bfc : SuitColour(varia, gravity));
        fn_7100146c50(reinterpret_cast<UnkMissilePart*>(part), 0, &c, 1);
    }
}
