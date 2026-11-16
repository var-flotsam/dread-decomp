// CSwarmControllerComponent unit (tools/tu_map.py unit 1163): a member is alerted. Unless it is forced
// or the swarm already has too many alerted members, the alert spreads to the members of its group (or
// of every active group) that accept it, each alerted in turn; otherwise the member leaves: its
// position is added to the controller's sum, it plays the billboard's effect, or its group lets it go
// (fn_7100748234 once the group is empty). Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CDizzeanSwarmControllerComponent.h"
#include "game/UnkTU1159.h"
#include "game/UnkTU2560.h"
#include "types.h"

using base::reflection::UnkReflObject;

// Returned through x8, so the original's result type is not trivially copyable.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkStruct_71007455dc;
struct UnkSwarmController_ovb6;
struct UnkSwarmGroup_ovb6;
UnkVec3Result fn_71007455dc(UnkStruct_71007455dc* self);
void fn_7100748234(UnkSwarmController_ovb6* self, UnkSwarmGroup_ovb6* group);

extern u8 g_unk_710164a9e8[];

struct UnkSwarmGroup_l74b680;

struct UnkSwarmUnit_l74b680 {
    u8 pad_0x0[0x10];
    UnkSwarmGroup_l74b680* group;  // 0x10
    u8 pad_0x18[0xc];
    bool active;  // 0x24
    bool field_0x25;
    u8 pad_0x26[0x3e];
    f32 field_0x64;
    u8 pad_0x68[0xec];
    f32 field_0x154;
    u8 pad_0x158[0x30];
    bool field_0x188;

    bool Accepts() { return fn_7100744e1c(reinterpret_cast<UnkSwarmMember_ovb6*>(this)); }
};
static_assert(__builtin_offsetof(UnkSwarmUnit_l74b680, field_0x154) == 0x154, "layout");
static_assert(__builtin_offsetof(UnkSwarmUnit_l74b680, field_0x188) == 0x188, "layout");

struct UnkSwarmGroup_l74b680 {
    u8 pad_0x0[0x24];
    bool active;  // 0x24
    u8 pad_0x25[0x93];
    UnkSwarmUnit_l74b680** members;  // 0xb8
    u32 count;  // 0xc0
    u8 pad_0xc4[0x1c];
    u32 users;  // 0xe0
    u8 pad_0xe4[0x64];

    UnkSwarmUnit_l74b680** begin() { return members; }
    UnkSwarmUnit_l74b680** end() { return members + count; }

    // The members that accept an alert.
    u32 CountAccepting() {
        u32 n = 0;
        for (UnkSwarmUnit_l74b680* m : *this)
            n += m->Accepts();
        return n;
    }

    // Lets one user go; true when none is left.
    bool Release() {
        if (users == 0)
            return true;
        return --users == 0;
    }
};
static_assert(__builtin_offsetof(UnkSwarmGroup_l74b680, users) == 0xe0, "layout");
static_assert(sizeof(UnkSwarmGroup_l74b680) == 0x148, "layout");

struct UnkSwarmGroups_l74b680 {
    u8 pad_0x0[0x10];
    UnkReflObject* owner;  // 0x10
    u8 pad_0x18[0x38];
    UnkSwarmGroup_l74b680* groups;  // 0x50
    u8 pad_0x58[0xb8];
    s32 count;  // 0x110

    UnkSwarmGroup_l74b680* Get(s64 i) {
        if (i < count && groups)
            return &groups[i];
        return nullptr;
    }
};
static_assert(__builtin_offsetof(UnkSwarmGroups_l74b680, count) == 0x110, "layout");

struct UnkSwarmDef_l74b680 {
    u8 pad_0x0[0x10];
    UnkSwarmGroups_l74b680* groups;  // 0x10
};

struct UnkSwarmKey_l74b680 {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkSwarmBillboard_l74b680 {
    u8 pad_0x0[0xd0];
    UnkSwarmKey_l74b680* effect;  // 0xd0
};

struct UnkSwarmTunables_l74b680 {
    u8 pad_0x0[0xf8];
    bool limit_by_users;  // 0xf8
    u32 max_alerted;  // 0xfc
    bool per_group;  // 0x100
};

// What alerted the member.
struct UnkSwarmAlert_l74b680 {
    u8 pad_0x0[0x6c];
    UnkVec3Result position;  // 0x6c
    u8 pad_0x78[0x1c];
    u32 field_0x94;
    u8 pad_0x98[0x1c];
    u32 flags;  // 0xb4

    bool Silent() { return (flags >> 16) & 1; }
};
static_assert(__builtin_offsetof(UnkSwarmAlert_l74b680, flags) == 0xb4, "layout");

struct UnkSwarmSum_l74b680 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkSwarmController_l74b680 {
    u8 pad_0x0[0xe8];
    UnkSwarmDef_l74b680* def;  // 0xe8
    u8 pad_0xf0[0xa0];
    UnkSwarmSum_l74b680 left_positions;  // 0x190
    u8 pad_0x19c[0x44];
    UnkSwarmTunables_l74b680* tunables;  // 0x1e0
    u8 pad_0x1e8[0x70];
    u32 left_count;  // 0x258

    UnkSwarmGroups_l74b680* Groups() { return def ? def->groups : nullptr; }
    void Spread(UnkSwarmUnit_l74b680* member, UnkSwarmGroup_l74b680* group, UnkSwarmAlert_l74b680* alert);
};
static_assert(__builtin_offsetof(UnkSwarmController_l74b680, left_positions) == 0x190, "layout");
static_assert(__builtin_offsetof(UnkSwarmController_l74b680, tunables) == 0x1e0, "layout");
static_assert(__builtin_offsetof(UnkSwarmController_l74b680, left_count) == 0x258, "layout");

void fn_710074b680(UnkSwarmController_l74b680* self, UnkSwarmUnit_l74b680* member, UnkSwarmAlert_l74b680* alert,
                   bool forced);

// Alerts the members of `group` that accept it.
inline void UnkSwarmController_l74b680::Spread(UnkSwarmUnit_l74b680* member, UnkSwarmGroup_l74b680* group,
                                               UnkSwarmAlert_l74b680* alert) {
    f32 timer = member->field_0x154;
    bool loud = alert ? !alert->Silent() : true;
    for (UnkSwarmUnit_l74b680* m : *group) {
        if (m->Accepts()) {
            if (timer <= 0.0f)
                m->field_0x154 = 0.0f;
            m->field_0x188 = loud;
            fn_710074b680(this, m, nullptr, true);
        }
    }
}

void fn_710074b680(UnkSwarmController_l74b680* self, UnkSwarmUnit_l74b680* member, UnkSwarmAlert_l74b680* alert,
                   bool forced) {
    // A forced alert, or one of this kind, makes the member leave at once.
    bool leave = forced;
    if (alert) {
        alert->position = fn_71007455dc(reinterpret_cast<UnkStruct_71007455dc*>(member));
        alert->field_0x94 |= 8;
        leave = forced || (alert->flags & 0x15401) == 1;
    }
    // Spread the alert, unless too many members would be alerted.
    if (!leave) {
        if (self->tunables->per_group) {
            UnkSwarmGroup_l74b680* group = member->group;
            if (group->users != 0) {
                bool few = self->tunables->limit_by_users && group->users <= self->tunables->max_alerted + 1;
                if (few || group->CountAccepting() <= self->tunables->max_alerted + 1) {
                    self->Spread(member, group, alert);
                    return;
                }
            }
        } else {
            u32 n = 0;
            for (s32 i = 0; i < self->def->groups->count; ++i) {
                UnkSwarmGroup_l74b680* group = self->Groups()->Get(i);
                if (group->active)
                    n += group->CountAccepting();
            }
            if (n <= self->tunables->max_alerted + 1) {
                for (s32 i = 0; i < self->def->groups->count; ++i) {
                    UnkSwarmGroup_l74b680* group = self->Groups()->Get(i);
                    if (group->active)
                        self->Spread(member, group, alert);
                }
                return;
            }
        }
    }

    ++self->left_count;
    UnkVec3Result pos = fn_71007455dc(reinterpret_cast<UnkStruct_71007455dc*>(member));
    self->left_positions.x += pos.x;
    self->left_positions.y += pos.y;
    self->left_positions.z += pos.z;

    UnkSwarmGroups_l74b680* groups = self->Groups();
    UnkReflObject* owner = groups ? groups->owner : nullptr;
    auto* billboard = reinterpret_cast<UnkSwarmBillboard_l74b680*>(
        owner && fn_710009d50c(owner->vf_0x0(), fn_710072be34()) ? owner : nullptr);
    void* manager = g_unk_71726da048;
    if (!manager) {
        manager = fn_71000839a0(0xe0, 0x3f);
        fn_7100e1bca4(manager);
        g_unk_71726da048 = manager;
    }
    u64 key = billboard->effect->key;
    void* effect = fn_7100e1cb50(manager, &key, g_unk_710164a9e8);
    if (effect && member->field_0x25) {
        member->field_0x64 = 1.0f;
        fn_7100745440(reinterpret_cast<UnkTarget*>(member), effect, 0);
        member->active = false;
        if (alert)
            member->field_0x188 = !alert->Silent();
        return;
    }
    UnkSwarmGroup_l74b680* group = member->group;
    member->active = false;
    member->field_0x25 = false;
    if (group->Release() && group->active)
        fn_7100748234(reinterpret_cast<UnkSwarmController_ovb6*>(self), reinterpret_cast<UnkSwarmGroup_ovb6*>(group));
}
