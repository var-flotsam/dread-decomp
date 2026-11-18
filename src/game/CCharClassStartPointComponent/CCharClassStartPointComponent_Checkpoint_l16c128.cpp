// CCharClassStartPointComponent's unit (tools/tu_map.py unit 2717): while the start point is enabled
// (+0x7b) and the game manager allows it (fn_7100dfbe3c), stores the checkpoint (the owner's name, an
// offset and an angle) under "LastCheckpointOffset" in the current scenario's section of the game
// manager's blackboard, then hands the game manager the owner's name twice with the flag (fn_7100dfaf88).
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CCharClassStartPointComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2553.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "game/UnkXformNode.h"
#include "../../game_aca57c/UnkBufferString.h"
#include <math.h>
#include "base/UnkCloneHelpers.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CElevatorUsableComponent.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::math::CVector3D;
using base::reflection::CType;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

typedef UnkBufferString<0x100, g_unk_7101a194b8> UnkStartPointName_l16c128;

// A name string built from another string.
struct UnkStartPointNameCopy_l16c128 : UnkStartPointName_l16c128 {
    explicit UnkStartPointNameCopy_l16c128(const CRntString& text) { fn_710007eaa0(Str(), &text); }
};

struct UnkEnumType;

// "LastCheckpointOffset" and the reflected type of its value (the unit's static initialiser builds both).
extern UnkEnumType g_unk_7172a0a4a8;

struct UnkStartPointRecord_l16c128 {
    UnkStartPointName_l16c128 name;
    UnkVec3f offset;
    UnkVec3f angle;

    UnkStartPointRecord_l16c128(const UnkStartPointName_l16c128& name_, const UnkVec3f& offset_,
                                const UnkVec3f& angle_)
        : name(name_), offset(offset_), angle(angle_) {}
};

struct UnkStartPointOwner_l16c128 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
};

struct UnkGeneratorTarget_ovb0120 {
    u8 pad_0x0[0x18];
    UnkStartPointOwner_l16c128* owner;  // 0x18
    u8 pad_0x20[0x5b];
    bool enabled;  // 0x7b
};

// The id's string, counting the read.
static inline const CRntString& NameString(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

// The property's qualified id, built on first use.
static inline UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), base::global::CStrId::Empty());
    return qid;
}

// Stores `value` under `name` in the blackboard's section `section` names (unless the blackboard is
// locked).
static inline void SetSectionValue(CBlackboard* blackboard, const CStrId& section, UnkQualifiedId& name, void* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = reinterpret_cast<const base::reflection::CType*>(&g_unk_7172a0a4a8);
            v.value = value;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&name), &v);
        }
    }
}

// Returns bool: include/game/CCharClassStartPointComponent.h declares it void (to change).
bool fn_710116c128(UnkGeneratorTarget_ovb0120* self, const UnkVec3f* offset, const UnkVec3f* angle, bool flag) {
    if (!self->enabled || !fn_7100dfbe3c(g_unk_7101d47310))
        return false;
    UnkStartPointNameCopy_l16c128 first(NameString(self->owner->name));
    UnkStartPointNameCopy_l16c128 second(NameString(self->owner->name));
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    UnkStartPointRecord_l16c128 record(first, *offset, *angle);
    {
        CStrId section_id = fn_7100de9c30(g_unk_7101d47310);
        SetSectionValue(blackboard, section_id, Qid(g_unk_7172a0a5e8), &record);
    }
    return fn_7100dfaf88(g_unk_7101d47310, first.Str(), *second.Str(), flag);
}

// The facing directions: right and left.
__attribute__((visibility("hidden"))) extern const CVector3D g_unk_710164d088;
__attribute__((visibility("hidden"))) extern const CVector3D g_unk_710164d094;

// The checkpoint as stored: the start point's name, an offset and an angle.
struct UnkStartPointRecord_l16c870 {
    UnkStartPointName_l16c128 name;
    UnkVec3f offset;
    UnkVec3f angle;

    UnkStartPointRecord_l16c870() : offset(0.0f, 0.0f, 0.0f), angle(0.0f, 0.0f, 0.0f) {}
};

// A section's values by key (at +0x20 of the section).
struct UnkSectionValues_l16c870 {
    UnkReflValue* values;  // 0x20
    CStrId* keys;  // 0x28
    u32 count;  // 0x30

    UnkReflValue* End() { return &values[count]; }
    UnkReflValue* Find(const CStrId& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i].data == key.data)
                return &values[i];
        return End();
    }
};

struct UnkSection_l16c870 {
    u8 pad_0x0[0x20];
    UnkSectionValues_l16c870 values;  // 0x20
};

struct UnkStartPointActor_l16c870 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
};

struct UnkStartPointOwner_l16c870 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkStartPoint_l16c870 {
    u8 pad_0x0[0x18];
    UnkStartPointActor_l16c870* actor;  // 0x18
    u8 pad_0x20[0x10];
    UnkStartPointOwner_l16c870* owner;  // 0x30
};

// What the spawned actor looks along.
struct UnkSpawnLook_l16c870 {
    void** vtable;
    u8 pad_0x8[0x38];
    CVector3D direction;  // 0x40
    u8 pad_0x4c[0xf9 - 0x4c];
    bool dirty;  // 0xf9
    u8 pad_0xfa[0x3a8 - 0xfa];
    u8 state;  // 0x3a8

    void vf_0x1e8(bool b) { reinterpret_cast<void (*)(UnkSpawnLook_l16c870*, bool)>(vtable[0x1e8 / 8])(this, b); }
    void vf_0x208(const CVector3D* dir) {
        reinterpret_cast<void (*)(UnkSpawnLook_l16c870*, const CVector3D*)>(vtable[0x208 / 8])(this, dir);
    }
    void vf_0x260() { reinterpret_cast<void (*)(UnkSpawnLook_l16c870*)>(vtable[0x260 / 8])(this); }
    void vf_0x2b8() { reinterpret_cast<void (*)(UnkSpawnLook_l16c870*)>(vtable[0x2b8 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkSpawnLook_l16c870, state) == 0x3a8, "layout");

struct UnkSpawned_l16c870 {
    void** vtable;
    u8 pad_0x8[0x1f0];
    UnkSpawnLook_l16c870* look;  // 0x1f8

    void SetPosition(UnkVec3f* pos, bool flag) {
        reinterpret_cast<void (*)(UnkSpawned_l16c870*, UnkVec3f*, bool)>(vtable[0xd0 / 8])(this, pos, flag);
    }
    void SetAngle(UnkVec3f* angle, bool flag) {
        reinterpret_cast<void (*)(UnkSpawned_l16c870*, UnkVec3f*, bool)>(vtable[0xd8 / 8])(this, angle, flag);
    }
};
static_assert(__builtin_offsetof(UnkSpawned_l16c870, look) == 0x1f8, "layout");



// Reads the value stored under `key` in the section into `out`, as the checkpoint's type; false when the key is
// missing or the value is empty.
static inline bool GetSectionValue(CBlackboard::CSection* section, const CStrId& key, void* out) {
    UnkSectionValues_l16c870& values = reinterpret_cast<UnkSection_l16c870*>(section)->values;
    UnkReflValue* v = values.Find(key);
    if (v == values.End())
        return false;
    const char* type = v->type;
    if (!type)
        return false;
    void* p = v->field_0x14 == 1 || *reinterpret_cast<const u64*>(type + 0x18) > 8 ? v->field_0x0 : v;
    if (!p)
        return false;
    const CType* t = reinterpret_cast<const CType*>(&g_unk_7172a0a4a8);
    UnkTypedRef dst{t, out};
    UnkTypedRef src;
    fn_71000a16e0(&src, type, p);
    return fn_710009d7a8(t, &dst, &src);
}

void fn_710116c870(UnkStartPoint_l16c870* self, UnkSpawned_l16c870* spawned) {
    UnkXform& xform = self->owner->xform;
    UnkVec3f position = xform.GetTranslation();
    UnkVec3f angle = xform.GetAngle();

    UnkStartPointRecord_l16c870 record;
    {
        CStrId section_id = fn_7100de9c30(g_unk_7101d47310);
        CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
        UnkQualifiedId& name = Qid(g_unk_7172a0a5e8);
        CBlackboard::CSection* section = fn_7101047270(blackboard, section_id);
        if (section && GetSectionValue(section, name.id, &record) &&
            fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(record.name.Str()),
                          reinterpret_cast<const UnkCompared_0x30*>(&NameString(self->actor->name)))) {
            position.x = record.offset.x + position.x;
            position.y = record.offset.y + position.y;
            position.z = record.offset.z + position.z;
            angle.x = record.angle.x + angle.x;
            angle.y = record.angle.y + angle.y;
            angle.z = record.angle.z + angle.z;
        }
        // Anything but a smart object faces exactly sideways.
        if (!fn_7100a81ed0(reinterpret_cast<UnkSmartObjectActor*>(self->owner)) &&
            fabsf(fabsf(angle.y) - 1.5707964f) > 0.1f)
            angle.y = 1.5707964f;
        spawned->SetPosition(&position, true);
        spawned->SetAngle(&angle, true);
        if (UnkSpawnLook_l16c870* look = spawned->look) {
            if (angle.y > 0.0f) {
                look->direction.x = 1.0f;
                look->direction.y = 0.0f;
                look->direction.z = 0.0f;
                look->vf_0x208(&g_unk_710164d088);
            } else {
                look->direction.x = -1.0f;
                look->direction.y = 0.0f;
                look->direction.z = 0.0f;
                look->vf_0x208(&g_unk_710164d094);
            }
            look->vf_0x1e8(true);
            if (look->state != 1) {
                look->state = 1;
                look->vf_0x2b8();
            }
            if (look->dirty) {
                look->dirty = false;
                look->vf_0x260();
            }
        }
    }
}
