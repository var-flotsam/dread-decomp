// CTunableMinimapManager's unit (2566): for every central unit whose tracker sees the player, sets the unit's
// blackboard flag (when its link holds a target) and the flag of every door, in the map's blackboard section.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CBossAIComponent.h"
#include "game/CDoorEmmyFXComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkSubId_7100fc4b34.h"
#include "game/UnkTU1176.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2566.h"
#include "game/UnkTU365.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkSetFlagDef_b0200 : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedName name;
};
extern UnkSetFlagDef_b0200 g_unk_71727c9968;

struct UnkMinimapSelf_le9d7fc {
    u8 pad_0x0[0x110];
    CStrId name;  // 0x110
};

struct UnkNamedDef_le9d7fc {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
};

struct UnkCentralUnit_le9d7fc : UnkReflObject {
    u8 pad_0x8[0x10];
    UnkNamedDef_le9d7fc* def;  // 0x18
    u8 pad_0x20[0x418];
    u8 link[0x170];  // 0x438
    u8 part[0x8];    // 0x5a8
};

struct UnkEntry_le9d7fc {
    u8 pad_0x0[0x10];
    UnkReflObject* object;  // 0x10
};

struct UnkEntryList_le9d7fc {
    UnkEntry_le9d7fc** data;
    u32 count;
};

struct UnkDoor_le9d7fc {
    u8 pad_0x0[0x18];
    UnkNamedDef_le9d7fc* def;  // 0x18
};

struct UnkDoorEntry_le9d7fc {
    u8 pad_0x0[0x10];
    UnkDoor_le9d7fc* object;  // 0x10
};

struct UnkDoorList_le9d7fc {
    UnkDoorEntry_le9d7fc** data;
    u32 count;
};

struct UnkSlot_le9d7fc {
    u8 pad_0x0[0x10];
    void* object;  // 0x10
};

struct UnkView_le9d7fc {
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
    virtual bool vf_0x50(const base::math::CVector2D* position);
};

struct UnkTracker_le9d7fc {
    u8 pad_0x0[0x30];
    UnkView_le9d7fc* view;  // 0x30
};

struct UnkPlayerXf_le9d7fc {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

template <const base::reflection::CClass* (*Getter)()>
static inline bool IsA(UnkReflObject* object) {
    const base::reflection::CType* type = object->vf_0x0();
    return fn_710009d50c(type, Getter());
}

static inline UnkSubId_7100fc4b34 SubId(UnkSetFlagDef_b0200& def, const CStrId& name) {
    if (!def.name.str || !def.name.str[0])
        fn_7100ae9d7c(&def.name, &def, g_unk_7101d470e0);
    return fn_71003327a4(&def.name, &name);
}


void fn_7100e9d7fc(UnkMapData* map) {
    UnkMinimapSelf_le9d7fc* self = reinterpret_cast<UnkMinimapSelf_le9d7fc*>(map);
    UnkGameManager* manager = g_unk_7101d47310;
    CStrId section_id = fn_7100dfbda4(manager, self->name);
    UnkEntryList_le9d7fc* list = reinterpret_cast<UnkEntryList_le9d7fc*>(fn_7100772c98());
    CBlackboard* blackboard = &manager->field_0x968;
    for (UnkEntry_le9d7fc **it = list->data, **end = list->data + list->count; it != end; ++it) {
        UnkReflObject* object = *it ? (*it)->object : nullptr;
        UnkCentralUnit_le9d7fc* unit =
            object && IsA<fn_710076cf58>(object) ? static_cast<UnkCentralUnit_le9d7fc*>(object) : nullptr;
        UnkSlot_le9d7fc* slot =
            *reinterpret_cast<UnkSlot_le9d7fc**>(fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(unit->part)));
        if (!slot || !slot->object)
            continue;
        if (object && IsA<fn_710077d9b4>(object))
            continue;
        UnkTracker_le9d7fc* tracker = reinterpret_cast<UnkTracker_le9d7fc*>(fn_710028aca8(slot->object));
        if (!tracker || !tracker->view)
            continue;
        UnkView_le9d7fc* view = tracker->view;
        UnkPlayerXf_le9d7fc* player = reinterpret_cast<UnkPlayerXf_le9d7fc*>(fn_7100dec788(g_unk_7101d47310));
        f32 x = player->x;
        f32 y = player->y;
        f32 z = player->z;
        base::math::CVector3D position3d;
        position3d.x = x;
        position3d.y = y;
        position3d.z = z;
        base::math::CVector2D position;
        fn_710007b430(&position, &position3d);
        if (!view->vf_0x50(&position))
            continue;

        // The unit's own flag, when its link holds a target.
        UnkSlot_le9d7fc* target = *reinterpret_cast<UnkSlot_le9d7fc**>(
            fn_71003b0c4c(reinterpret_cast<UnkZiplineLink_b49*>(unit->link)));
        if (target && target->object) {
            UnkSubId_7100fc4b34 key = SubId(g_unk_71727c9968, unit->def->name);
            bool value = true;
            if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section_id)) {
                if (!blackboard->field_0xe8) {
                    UnkTypedValue v;
                    v.type = fn_710000d228();
                    v.value = &value;
                    fn_7100332c80(s, (UnkPropertyName*)&key.id, &v);
                }
            }
        }

        // Every door's flag.
        UnkDoorList_le9d7fc* doors = reinterpret_cast<UnkDoorList_le9d7fc*>(fn_71007b5d90());
        for (UnkDoorEntry_le9d7fc **d = doors->data, **dend = doors->data + doors->count; d != dend; ++d) {
            UnkDoor_le9d7fc* door = *d ? (*d)->object : nullptr;
            UnkSubId_7100fc4b34 key = SubId(g_unk_71727c9968, door->def->name);
            bool value = true;
            if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section_id)) {
                if (!blackboard->field_0xe8) {
                    UnkTypedValue v;
                    v.type = fn_710000d228();
                    v.value = &value;
                    fn_7100332c80(s, (UnkPropertyName*)&key.id, &v);
                }
            }
        }
    }
}