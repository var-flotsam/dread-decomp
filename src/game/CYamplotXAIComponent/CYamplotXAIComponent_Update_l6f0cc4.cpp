// CYamplotXAIComponent's unit (tools/tu_map.py unit 1109): a step of the Yamplot X while its movement is in
// state 4 (riding a smart link on an air track: lands it, or leaves it when the link is gone) or, otherwise,
// when the animation's track is one of two ids; then the base step. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU1444.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU376.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

__attribute__((visibility("hidden"))) extern base::global::CStrId g_unk_717270c498;

// A copied string id whose destructor does not test for a null id.
struct UnkYamplotId_l6f1ed8 {
    UnkStringData* data;
    UnkYamplotId_l6f1ed8(const UnkYamplotId_l6f1ed8& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkYamplotId_l6f1ed8() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data != &pool->empty && --data->ref_count == 0)
                fn_71000803a4(pool, data);
        }
    }
};

struct UnkStringEntry_l6f0cc4 {
    u8 pad_0x0[0x30];
    u64 hash;
};

// The owner's movement at +0x218.
struct UnkYamplotMovement_l6f0cc4 {
    void** vtable;
    u8 pad_0x8[0xf1];
    bool active;  // 0xf9
    u8 pad_0xfa[0x17a0 - 0xfa];
    u8 event[0xd0];             // 0x17a0
    UnkObject_0x100 link;       // 0x1870
    bool has_link;              // 0x1970

    void SetActive() {
        if (active != true) {
            active = true;
            reinterpret_cast<void (*)(UnkYamplotMovement_l6f0cc4*)>(vtable[0x260 / 8])(this);
        }
    }
    void vf_0x220() { reinterpret_cast<void (*)(UnkYamplotMovement_l6f0cc4*)>(vtable[0x220 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkYamplotMovement_l6f0cc4, has_link) == 0x1970, "movement layout");

struct UnkYamplotSink_l6f0cc4 {
    void** vtable;

    void vf_0x2b0(UnkCollisionObject* object) {
        reinterpret_cast<void (*)(UnkYamplotSink_l6f0cc4*, UnkCollisionObject*)>(vtable[0x2b0 / 8])(this, object);
    }
};

struct UnkYamplotOwner_l6f0cc4 {
    u8 pad_0x0[0x1f8];
    UnkYamplotSink_l6f0cc4* field_0x1f8;
    u8 pad_0x200[0x18];
    UnkYamplotMovement_l6f0cc4* movement;  // 0x218
    UnkAnimationComponent* animation;      // 0x220
    u8 pad_0x228[0x78];
    UnkCollisionContainer* collision;  // 0x2a0
};
static_assert(__builtin_offsetof(UnkYamplotOwner_l6f0cc4, collision) == 0x2a0, "owner layout");

struct UnkYamplotX_l6f0cc4 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkYamplotOwner_l6f0cc4* owner;  // 0x30
    u8 pad_0x38[0x14760 - 0x38];
    bool field_0x14760;
    u8 pad_0x14761;
    bool field_0x14762;
    u8 pad_0x14763[5];
    bool field_0x14768;
    bool field_0x14769;
    u8 pad_0x1476a[0x147b8 - 0x1476a];
    bool field_0x147b8;
    u8 pad_0x147b9[0x147c4 - 0x147b9];
    u32 field_0x147c4;
    UnkObject_0x100 link;  // 0x147c8

    void vf_0x3d8() { reinterpret_cast<void (*)(UnkYamplotX_l6f0cc4*)>(vtable[0x3d8 / 8])(this); }
    void vf_0x760() { reinterpret_cast<void (*)(UnkYamplotX_l6f0cc4*)>(vtable[0x760 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkYamplotX_l6f0cc4, link) == 0x147c8, "layout");

bool fn_71006efe18(UnkYamplotX_l6f0cc4* self);

void fn_71006f0cc4(UnkYamplotX_l6f0cc4* self, void* arg) {
    UnkYamplotMovement_l6f0cc4* movement = self->owner->movement;
    u32 state = fn_71010cc3a4(movement);
    UnkAnimationComponent* animation = self->owner->animation;
    if (state == 4) {
        UnkUnit1444_Event* event = reinterpret_cast<UnkUnit1444_Event*>(movement->event);
        if (!fn_710117b348(animation, fn_7100c4f918())) {
            self->vf_0x3d8();
        } else {
            self->field_0x14769 = false;
            if (movement->has_link)
                fn_71002b5c90(&self->link, &movement->link);
            self->field_0x14760 = true;
            movement->SetActive();
            movement->vf_0x220();
            if (fn_71006efe18(self)) {
                self->field_0x147b8 = false;
                if (self->field_0x147c4 == 0) {
                    self->field_0x147c4 = 1;
                    UnkCollisionObject* object = fn_71008640a4(self->owner->collision, &g_unk_717270c498);
                    self->owner->field_0x1f8->vf_0x2b0(object);
                }
            } else {
                self->field_0x147b8 = true;
            }
        }
        fn_7100974798(event, 1);
    } else if (animation) {
        UnkYamplotId_l6f1ed8 id(*reinterpret_cast<const UnkYamplotId_l6f1ed8*>(fn_7101177ae8(animation, 0xf4061ff25a2163d6)));
        u64 hash = reinterpret_cast<const UnkStringEntry_l6f0cc4*>(id.data)->hash;
        if (hash == 0xeacdeb01ece85851 || hash == 0xad9215f288161046) {
            movement->SetActive();
            self->vf_0x760();
            self->field_0x14768 = false;
            self->field_0x14762 = true;
        }
    }
    fn_7101108124(reinterpret_cast<UnkQuetzoaAIComponent*>(self), arg);
}
