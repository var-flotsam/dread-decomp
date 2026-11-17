// Unit 1444 of tools/tu_map.py: finds or creates the owner's circle collider under a name, links a new one
// into the unit's collider group, sets its radius and its data, and refreshes the container.
// Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "game/CCharacterMovement.h"
#include "game/CCollisionComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/logic/collision/CCircleShape2D.h"
#include "types.h"

using base::global::CStrId;

struct UnkCollisionComponent_Groups_ovb0107;
UnkColliderNode* fn_7100862400(UnkCollisionComponent_Groups_ovb0107* self, const CStrId* name);

extern void* const g_unk_717273a790 __attribute__((visibility("hidden")));
extern void* const g_unk_717273a788 __attribute__((visibility("hidden")));

// The id table's slot 0x9f0 (the header has no member for it yet).
inline const CStrId* ColliderKindId_l974cec() {
    return reinterpret_cast<const CStrId*>(reinterpret_cast<const u8*>(&GameManagerIdTable()) + 0x9f0);
}

struct UnkShape_l974cec {
    void** vtable;

    s32 Kind() { return reinterpret_cast<s32 (*)(UnkShape_l974cec*)>(vtable[0x18 / 8])(this); }
};

struct UnkGroup_l974cec;

// A fixed list of links: its count, then the linked objects, packed to 4 bytes.
#pragma pack(push, 4)
template <typename T>
struct UnkLinks_l974cec {
    u32 count;
    T* items[8];

    void Add(T* item) {
        items[count] = item;
        count++;
    }
};
#pragma pack(pop)

struct UnkCollider_l974cec {
    void** vtable;
    u8 pad_0x8[0x20];
    bool field_0x28;
    u8 pad_0x29[0x6b - 0x29];
    bool field_0x6b;
    u8 pad_0x6c[0x158 - 0x6c];
    UnkShape_l974cec* shape;  // 0x158
    u8 pad_0x160[4];
    UnkLinks_l974cec<UnkGroup_l974cec> groups;  // 0x164

    UnkShape_l974cec* Circle() { return shape->Kind() == 2 ? shape : nullptr; }
    u32 Flags() { return reinterpret_cast<u32 (*)(UnkCollider_l974cec*)>(vtable[0x58 / 8])(this); }
    void SetData(const void* data) {
        reinterpret_cast<void (*)(UnkCollider_l974cec*, const void*)>(vtable[0x78 / 8])(this, data);
    }
};

struct UnkGroup_l974cec {
    u8 pad_0x0[0x28];
    bool field_0x28;
    u8 pad_0x29[0x64 - 0x29];
    u32 flags;  // 0x64
    u8 pad_0x68[3];
    bool field_0x6b;
    u8 pad_0x6c[0x124 - 0x6c];
    UnkLinks_l974cec<UnkCollider_l974cec> members;  // 0x124
};

struct UnkContainer_l974cec {
    void** vtable;

    UnkCollider_l974cec* Create(const CStrId* name, const CStrId* group, const CStrId* type_name,
                                const CStrId* extra, u32 kind) {
        using Fn = UnkCollider_l974cec* (*)(UnkContainer_l974cec*, const CStrId*, const CStrId*, const CStrId*,
                                            const CStrId*, u32, u32, u32, void*, s32, void*, s32, void*, s32, bool,
                                            bool, bool);
        return reinterpret_cast<Fn>(vtable[0x168 / 8])(this, name, group, type_name, extra, kind, 0, 1, nullptr, -1,
                                                       nullptr, -1, nullptr, -1, false, false, true);
    }
};

struct UnkOwner_l974cec {
    u8 pad_0x0[0x1f8];
    void* movement;  // 0x1f8
    u8 pad_0x200[0x2a0 - 0x200];
    UnkContainer_l974cec* collision;  // 0x2a0
    u8 pad_0x2a8[0x37c - 0x2a8];
    f32 radius;  // 0x37c
};

struct UnkUnit1444_l974cec {
    u8 pad_0x0[0x30];
    UnkOwner_l974cec* owner;  // 0x30
};

UnkCollider_l974cec* fn_7100974cec(UnkUnit1444_l974cec* self, const CStrId* name, const void* data) {
    UnkContainer_l974cec* container = self->owner->collision;
    if (!container)
        return nullptr;
    UnkGroup_l974cec* group = reinterpret_cast<UnkGroup_l974cec*>(
        fn_710086406c(reinterpret_cast<UnkPlayerMovementSource*>(container), &g_unk_717273a788));
    if (!group)
        group = reinterpret_cast<UnkGroup_l974cec*>(
            fn_7100862400(reinterpret_cast<UnkCollisionComponent_Groups_ovb0107*>(container),
                          reinterpret_cast<const CStrId*>(&g_unk_717273a788)));
    UnkCollider_l974cec* collider = reinterpret_cast<UnkCollider_l974cec*>(
        fn_71008640a4(reinterpret_cast<UnkCollisionContainer*>(container), name));
    if (!collider) {
        collider = container->Create(name, reinterpret_cast<const CStrId*>(&g_unk_717273a790), &g_unk_7101d470e0,
                                     ColliderKindId_l974cec(), 0x10002000);
        if (group && collider) {
            group->members.Add(collider);
            collider->groups.Add(group);
            group->flags = group->flags | collider->Flags();
            group->field_0x6b = group->field_0x6b ? true : collider->field_0x6b;
            group->field_0x28 = collider->field_0x28;
            fn_7100d35a98(reinterpret_cast<UnkEntry*>(group), true);
        }
    }
    UnkShape_l974cec* circle = collider->Circle();
    if (collider && circle) {
        UnkOwner_l974cec* owner = self->owner;
        f32 radius = owner->movement ? fn_7100977b20(owner->movement) : owner->radius;
        fn_7100d618f8(reinterpret_cast<UnkSphereShape_ovb0135*>(circle), radius);
        collider->SetData(data);
        fn_7100d35a98(reinterpret_cast<UnkEntry*>(collider), true);
    }
    fn_7100861710(reinterpret_cast<UnkCollisionContainer_b42*>(container), true);
    return collider;
}
