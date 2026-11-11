// CRinkaUnitComponent unit (tools/tu_map.py unit 981): a reset. Clears +0x258; when the actor (+0x30) has
// the object at +0x280, hands it the two entries of the table g_unk_71726fd110 (fn_710114ffb8) and
// releases the handles at +0x268 and +0x270 (fn_710115596c); then sets slot 0 of the actor's target
// (+0x228, +0x50) to the value g_unk_7101647634 (fn_71001c2764). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CFXComponent.h"
#include "game/CRinkaUnitComponent.h"
#include "game/UnkSlots.h"
#include "types.h"

struct UnkRinkaEntry_ruc {
    u8 pad_0x0[0x30];
    void* field_0x30;
};

struct UnkRinkaControl_ruc;

extern const base::math::CVector4D g_unk_7101647634;

struct UnkRinkaTarget_ruc {
    u8 pad_0x0[0x138];
    UnkSlots* slots;  // 0x138
};

struct UnkRinkaLink_ruc {
    u8 pad_0x0[0x50];
    UnkRinkaTarget_ruc* target;  // 0x50
};

struct UnkRinkaActor_ruc {
    u8 pad_0x0[0x228];
    UnkRinkaLink_ruc* link;  // 0x228
    u8 pad_0x230[0x50];
    UnkRinkaControl_ruc* control;  // 0x280
};

struct UnkRinkaUnit_ruc {
    u8 pad_0x0[0x30];
    UnkRinkaActor_ruc* actor;  // 0x30
    u8 pad_0x38[0x220];
    u32 field_0x258;
    u8 pad_0x25c[0xc];
    void* handle_0x268;
    void* handle_0x270;
};

void fn_71006602a4(UnkRinkaUnit_ruc* self) {
    self->field_0x258 = 0;
    if (self->actor->control) {
        fn_710114ffb8(self->actor->control, g_unk_71726fd110[0]->field_0x30, 0, 0);
        fn_710115596c(self->actor->control, self->handle_0x268);
        self->handle_0x268 = nullptr;
        fn_710114ffb8(self->actor->control, g_unk_71726fd110[1]->field_0x30, 0, 0);
        fn_710115596c(self->actor->control, self->handle_0x270);
        self->handle_0x270 = nullptr;
    }
    if (UnkRinkaLink_ruc* link = self->actor->link) {
        if (UnkRinkaTarget_ruc* target = link->target)
            fn_71001c2764(target->slots, 0, 0, &g_unk_7101647634, 1, true);
    }
}
