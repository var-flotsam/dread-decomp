// CPickableComponent unit (unit 1492 of tools/tu_map.py): plays the pickable's effect (its path id at +0x58)
// once at the owner's position, unrotated and at full scale, through the model manager. Names and layouts
// are placeholders.
#include "base/global/CRntVector.h"
#include "game/CAutclastChargeAttack.h"
#include "game/UnkActor.h"
#include "game/UnkModelManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU134.h"
#include "game/UnkTU2739.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkFxObjectVtable_ovb0041 {
    void* unk_0x0[2];
    void (*set_position)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x10
    void (*set_rotation)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x18
    void (*set_scale)(UnkFxObject_ovb0041* self, const UnkVec3f* v);     // 0x20
    void* unk_0x28[13];
    void (*set_loop)(UnkFxObject_ovb0041* self, bool loop);               // 0x90
    void (*set_speed)(UnkFxObject_ovb0041* self, f32 speed);              // 0x98
};
struct UnkFxObject_ovb0041 {
    UnkFxObjectVtable_ovb0041* vtable;
};

struct UnkPickable_l9ce17c {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x20];
    u8 effect[0x8];  // 0x58, the effect's path id
};

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71009ce17c(UnkPickable_l9ce17c* self) {
    UnkModelManager* manager = ModelManager();
    UnkVec3f position(self->owner->field_0x44, self->owner->field_0x54, self->owner->field_0x64);
    UnkVec3f rotation(0.0f, 0.0f, 0.0f);
    UnkVec3f scale(1.0f, 1.0f, 1.0f);
    if (UnkFxObject_ovb0041* fx = fn_71003b7db0(manager, self->effect, false, true)) {
        fx->vtable->set_position(fx, &position);
        fx->vtable->set_rotation(fx, &rotation);
        fx->vtable->set_scale(fx, &scale);
        fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(fx), true);
        fx->vtable->set_loop(fx, false);
        fx->vtable->set_speed(fx, 1.0f);
        fn_71011a9bb4(manager, fx);
    }
}
