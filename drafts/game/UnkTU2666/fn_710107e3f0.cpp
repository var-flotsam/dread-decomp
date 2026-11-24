// Unit 2666 (lua_bind): moves the entity named by argument 1 to the entity or landmark named by
// argument 2 (onto the ground below it when argument 3 is set or the target stands on ground),
// turns it the same way and tells its movement and animation. Returns 1 when there is no such
// entity. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CLandmarkComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1176.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2677.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;

struct lua_State;

// A position returned through x8: its copy constructor is not trivial.
struct UnkGroundPoint : CVector3D {
    UnkGroundPoint(f32 x_, f32 y_, f32 z_) {
        x = x_;
        y = y_;
        z = z_;
    }
    UnkGroundPoint(const UnkGroundPoint& o) {
        x = o.x;
        y = o.y;
        z = o.z;
    }
};

struct UnkGroundEntity;
UnkGroundPoint fn_7100d7b540(UnkGroundEntity* self, const UnkGroundPoint* pos);
struct UnkLuaNode_b0160;
UnkVec3f fn_71008a32f8(UnkLuaNode_b0160* node);
UnkVec3f fn_71008a3314(UnkLuaNode_b0160* node);
void fn_7100e01fc8(void*);

struct UnkDesc_SpitSend;
extern UnkDesc_SpitSend g_unk_710164b968;  // reached through the GOT

struct UnkTeleportZero_l07e3f0 {
    u32 value;
    UnkTeleportZero_l07e3f0() : value(0) {}
};

inline const UnkTeleportZero_l07e3f0& UnkTeleportZero() {
    static UnkTeleportZero_l07e3f0 s_value asm("g_unk_7101d57cc8");
    return s_value;
}

inline u32 UnkTeleportDefaultMask() {
    static u32 s_mask asm("g_unk_7101d57cb8") = ~UnkTeleportZero().value & 0xff;
    return s_mask;
}

// What fn_7100774014 gives for a target: whether it stands on ground (+0x79).
struct UnkTeleportFooting_l07e3f0 {
    u8 pad_0x0[0x79];
    bool on_ground;
};

struct UnkTeleportMovement_l07e3f0 {
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
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual void vf_0x1c0();
    virtual void vf_0x1c8();
    virtual void vf_0x1d0();
    virtual void vf_0x1d8();
    virtual void vf_0x1e0();
    virtual void vf_0x1e8(bool enable);
    virtual void vf_0x1f0();
    virtual void vf_0x1f8();
    virtual void vf_0x200();
    virtual void vf_0x208(UnkVec3f* direction);
    u8 pad_0x8[0x38];
    UnkVec3f direction;  // 0x40
};

struct UnkTeleportAnimDesc_l07e3f0 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkTeleportAnimation_l07e3f0 {
    u8 pad_0x0[0x2b8];
    UnkTeleportAnimDesc_l07e3f0* desc;
};

struct UnkTeleportEntity_l07e3f0 {
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
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0(const UnkGroundPoint* position, bool now);
    virtual void vf_0xd8(const UnkGroundPoint* angle, bool now);

    u8 pad_0x8[0x30];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x1f0 - 0x94];
    UnkTeleportMovement_l07e3f0* movement;  // 0x1f0
    u8 pad_0x1f8[0x220 - 0x1f8];
    UnkTeleportAnimation_l07e3f0* animation;  // 0x220
};

// The landmark's vectors are the same three floats.
inline const UnkGroundPoint& AsPoint(const UnkVec3f& v) { return reinterpret_cast<const UnkGroundPoint&>(v); }

static inline UnkTeleportEntity_l07e3f0* FindById(UnkGameManager* manager, const CStrId& id, u32 kind) {
    u64 key = id.data->Key();
    alignas(8) u32 flags = kind;
    return reinterpret_cast<UnkTeleportEntity_l07e3f0*>(fn_7100debbc8(manager, &key, &flags));
}

static inline UnkTeleportEntity_l07e3f0* FindByName(UnkGameManager* manager, const char* name, u32 kind) {
    u64 hash = StringCrc64(name, __builtin_strlen(name));
    alignas(8) u32 flags = kind;
    return static_cast<UnkTeleportEntity_l07e3f0*>(fn_7100deba8c(manager, &hash, &flags));
}

int fn_710107e3f0(lua_State* L) {
    const char* entity_name = nullptr;
    const char* target_name = nullptr;
    bool to_ground = false;
    if (!fn_71010c0270(L, "ssb", &entity_name, &target_name, &to_ground))
        return 0;

    UnkTeleportEntity_l07e3f0* entity;
    {
        UnkGameManager* manager = g_unk_7101d47310;
        CStrId id(entity_name, true);
        entity = FindById(manager, id, g_unk_71727bebb8);
    }
    if (!entity)
        return 1;

    UnkGroundPoint position(entity->xform.m[0][3], entity->xform.m[1][3], entity->xform.m[2][3]);
    UnkGroundPoint angle = AsPoint(entity->xform.GetAngle());
    base::reflection::UnkReflTypeIdArg mode;  // what the animation is sent at the end
    base::reflection::UnkReflTypeIdArg mask;
    if (UnkTeleportEntity_l07e3f0* target = FindByName(g_unk_7101d47310, target_name, g_unk_71727bebb8)) {
        position = UnkGroundPoint(target->xform.m[0][3], target->xform.m[1][3], target->xform.m[2][3]);
        angle = AsPoint(target->xform.GetAngle());
        UnkTeleportFooting_l07e3f0* footing = static_cast<UnkTeleportFooting_l07e3f0*>(
            fn_7100774014(reinterpret_cast<UnkScenarioObject_ovb0032*>(target)));
        if (to_ground || (footing && footing->on_ground))
            position = fn_7100d7b540(reinterpret_cast<UnkGroundEntity*>(entity), &position);
    } else {
        UnkLuaNode_b0160* node;
        {
            CStrId id(target_name, true);
            node = CLandmarkComponent_FindById(&id);
        }
        if (node) {
            position = AsPoint(fn_71008a32f8(node));
            angle = AsPoint(fn_71008a3314(node));
            if (to_ground)
                position = fn_7100d7b540(reinterpret_cast<UnkGroundEntity*>(entity), &position);
        }
    }

    entity->vf_0xd0(&position, true);
    entity->vf_0xd8(&angle, true);
    fn_7100e01fc8(g_unk_7101d47310);
    if (UnkTeleportMovement_l07e3f0* movement = entity->movement) {
        movement->vf_0x1e8(true);
        UnkVec3f direction(!(angle.y >= 0.0f) ? -1.0f : 1.0f, 0.0f, 0.0f);
        movement->vf_0x208(&direction);
        movement->direction = direction;
    }
    if (UnkTeleportAnimation_l07e3f0* animation = entity->animation) {
        u64 hash = animation->desc->hash;
        mode.id = 1;
        mask.id = UnkTeleportDefaultMask();
        CAnimationComponent_SendActionToTrack(animation, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b968, &g_unk_7172769740, &mask);
    }
    return 0;
}
