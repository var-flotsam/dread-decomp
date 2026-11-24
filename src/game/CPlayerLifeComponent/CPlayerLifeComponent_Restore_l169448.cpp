// CPlayerLifeComponent's unit (tools/tu_map.py unit 2716): restores the player's life component after the
// base reset (fn_710115fd10): the life shards from the inventory, the tunable life override, the owner's
// sub-objects, the track's request, the position and angles of the actor the handle at +0x240 points at,
// and empties the owner's node list at +0x320. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CInputComponent.h"
#include "game/CInventoryComponent.h"
#include "game/CLifeComponent.h"
#include "game/CPlayerLifeComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "game/UnkPlayerLifeTunable.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkInventory;
struct UnkInputComponentState;
struct UnkAim_7100343ca8;
struct UnkLifeComponentFull_ovb0000;
struct UnkPlayerLife_ovb0049;
void fn_7100343ca8(UnkAim_7100343ca8* self, bool request);
void fn_71011697a0(UnkPlayerLife_ovb0049* self, f32 shards);

struct UnkLifeHashHolder_l169448 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkLifeTrack_l169448 {
    u8 pad_0x0[0x2b8];
    UnkLifeHashHolder_l169448* field_0x2b8;
};

struct UnkLifeTarget_l169448 {
    void** vtable;
    void vf_0x1e8(bool value) {
        reinterpret_cast<void (*)(UnkLifeTarget_l169448*, bool)>(vtable[0x1e8 / 8])(this, value);
    }
};

// A sub-object with two flags whose changes it is told about.
struct UnkLifeWatcher_l169448 {
    void** vtable;
    u8 pad_0x8[0xf1];
    bool field_0xf9;
    u8 pad_0xfa[0x2ae];
    bool field_0x3a8;

    void vf_0x260() { reinterpret_cast<void (*)(UnkLifeWatcher_l169448*)>(vtable[0x260 / 8])(this); }
    void vf_0x2b8() { reinterpret_cast<void (*)(UnkLifeWatcher_l169448*)>(vtable[0x2b8 / 8])(this); }

    void SetField3a8(bool value) {
        if (field_0x3a8 != value) {
            field_0x3a8 = value;
            vf_0x2b8();
        }
    }
    void SetFieldf9(bool value) {
        if (field_0xf9 != value) {
            field_0xf9 = value;
            vf_0x260();
        }
    }
};
static_assert(__builtin_offsetof(UnkLifeWatcher_l169448, field_0x3a8) == 0x3a8, "layout");

struct UnkLifeNode_l169448 {
    u8 pad_0x0[0x8];
    UnkLifeNode_l169448* next;
};

struct UnkLifeAllocator_l169448 {
    void** vtable;
    void Free(UnkLifeNode_l169448* node) {
        reinterpret_cast<void (*)(UnkLifeAllocator_l169448*, UnkLifeNode_l169448*)>(vtable[0x48 / 8])(this, node);
    }
};

// A singly linked list whose nodes come from an allocator.
struct UnkLifeNodeList_l169448 {
    u32 count;
    u8 pad_0x4[0x4];
    UnkLifeNode_l169448* head;
    UnkLifeNode_l169448* tail;
    UnkLifeAllocator_l169448* allocator;

    void Clear() {
        for (UnkLifeNode_l169448* node = head; node;) {
            UnkLifeNode_l169448* next = node->next;
            allocator->Free(node);
            node = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkLifeOwner_l169448 {
    void** vtable;
    u8 pad_0x8[0x1d0];
    UnkInputComponentState* input;  // 0x1d8
    u8 pad_0x1e0[0x20];
    UnkLifeTarget_l169448* field_0x200;
    UnkLifeWatcher_l169448* field_0x208;
    u8 pad_0x210[0x10];
    UnkLifeTrack_l169448* track;  // 0x220
    u8 pad_0x228[0x48];
    UnkInventory* inventory;  // 0x270
    u8 pad_0x278[0x38];
    UnkAim_7100343ca8* aim;  // 0x2b0
    u8 pad_0x2b8[0x68];
    UnkLifeNodeList_l169448 nodes;  // 0x320

    // vtable slot 0xd0: places the actor at `pos`.
    void vf_0xd0(const UnkVec3f& pos, bool flag) {
        reinterpret_cast<void (*)(UnkLifeOwner_l169448*, const UnkVec3f&, bool)>(vtable[0xd0 / 8])(this, pos, flag);
    }
    // vtable slot 0xd8: turns the actor to `angle`.
    void vf_0xd8(const UnkVec3f& angle, bool flag) {
        reinterpret_cast<void (*)(UnkLifeOwner_l169448*, const UnkVec3f&, bool)>(vtable[0xd8 / 8])(this, angle, flag);
    }
};
static_assert(__builtin_offsetof(UnkLifeOwner_l169448, aim) == 0x2b0, "layout");
static_assert(__builtin_offsetof(UnkLifeOwner_l169448, nodes) == 0x320, "layout");

// The actor a weak handle points at, with its transform.
struct UnkLifeActor_l169448 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkPlayerLife_l169448 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkLifeOwner_l169448* owner;  // 0x30
    u8 pad_0x38[0x18];
    f32 max_life;  // 0x50
    u8 pad_0x54[0x1ec];
    UnkWeakHandle* handle;  // 0x240
    u8 pad_0x248[0xd8];
    f32 field_0x320;

    void vf_0x168(bool flag, f32 value) {
        reinterpret_cast<void (*)(UnkPlayerLife_l169448*, bool, f32)>(vtable[0x168 / 8])(this, flag, value);
    }
    void vf_0x170(f32 value) {
        reinterpret_cast<void (*)(UnkPlayerLife_l169448*, f32)>(vtable[0x170 / 8])(this, value);
    }

    UnkLifeActor_l169448* Target() {
        return handle ? reinterpret_cast<UnkLifeActor_l169448*>(handle->target) : nullptr;
    }
};
static_assert(__builtin_offsetof(UnkPlayerLife_l169448, field_0x320) == 0x320, "layout");

// Places the owner at the target's position and turns it to the target's angles.
static inline void MoveOwnerToTarget(UnkPlayerLife_l169448* self) {
    self->owner->vf_0xd0(self->Target()->xform.GetTranslation(), true);
    self->owner->vf_0xd8(self->Target()->xform.GetAngle(), true);
}

static inline const UnkQualifiedId& ItemId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), base::global::CStrId::Empty());
    return qid;
}

void fn_7101169448(UnkPlayerLife_l169448* self) {
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
    self->field_0x320 = 0.25f;
    fn_710115fd10(reinterpret_cast<UnkLifeComponentFull_ovb0000*>(self));
    if (UnkInventory* inventory = self->owner->inventory)
        fn_71011697a0(reinterpret_cast<UnkPlayerLife_ovb0049*>(self),
                      fn_7101142988(inventory, ItemId(g_unk_717275a098), 0.0f));
    f32 max_life = g_unk_7172a0a0a8.fPlayerMaxLifeOverride;
    if (max_life > 0.0f) {
        self->vf_0x170(max_life);
        self->vf_0x168(true, self->max_life);
    }
    if (UnkLifeTarget_l169448* target = self->owner->field_0x200)
        target->vf_0x1e8(true);
    if (UnkInputComponentState* input = self->owner->input)
        fn_7100871d7c(input);
    if (UnkLifeWatcher_l169448* watcher = self->owner->field_0x208) {
        watcher->SetField3a8(true);
        watcher->SetFieldf9(false);
    }
    if (UnkAim_7100343ca8* aim = self->owner->aim)
        fn_7100343ca8(aim, true);
    if (UnkLifeTrack_l169448* track = self->owner->track) {
        u64 hash = track->field_0x2b8->hash;
        mode.id = 1;
        mask.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(track, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
    }
    if (self->Target()) {
        MoveOwnerToTarget(self);
        ResetRef(&self->handle);
    }
    self->owner->nodes.Clear();
}
