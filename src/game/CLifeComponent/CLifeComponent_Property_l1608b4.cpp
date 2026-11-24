// CLifeComponent's unit (tools/tu_map.py unit 2715): the property handler. Writes out the life (+0x50), a
// flag for state 2, the shield (+0x54) and the shield's share of the life; reads three floats, a name and
// runs a refresh. Other properties go to the base handler (fn_71010c4250). Names and layouts are
// placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/global/CRntString.h"
#include "game/CLifeComponent.h"
#include "game/UnkComponent.h"
#include "types.h"

using namespace unk_prop;

struct UnkLifeComp_l1608b4 : UnkBaseComponent {
    u8 pad_0x40[0x10];
    f32 life;  // 0x50
    f32 shield;  // 0x54
    u8 pad_0x58[0x18];
    s32 state;  // 0x70
    u8 pad_0x74[0xc];
    base::global::CRntString name;  // 0x80
    u8 pad_0x80_[0xe4 - 0x80 - sizeof(base::global::CRntString)];
    f32 field_0xe4;
    f32 field_0xe8;
    f32 field_0xec;
};
static_assert(__builtin_offsetof(UnkLifeComp_l1608b4, name) == 0x80, "layout");
static_assert(__builtin_offsetof(UnkLifeComp_l1608b4, field_0xe4) == 0xe4, "layout");

// The parameter type is the one src/game/CPlayerLifeComponent/CPlayerLifeComponent_c2_1.cpp declares.
struct UnkObj_7101165a40;

u32 fn_71011608b4(UnkObj_7101165a40* component, UnkProp* p) {
    UnkLifeComp_l1608b4* self = reinterpret_cast<UnkLifeComp_l1608b4*>(component);
    if (!p->set && Is(p, Key(0x1720)))
        return CRntFile_WriteFormat(p->stream, "f", (f64)self->life) != 0;
    if (!p->set && Is(p, Key(0x1510)))
        return CRntFile_WriteFormat(p->stream, "b", self->state == 2) != 0;
    if (!p->set && Is(p, Key(0xcd0)))
        return CRntFile_WriteFormat(p->stream, "f", (f64)self->shield) != 0;
    if (!p->set && Is(p, Key(0xcd8)))
        return CRntFile_WriteFormat(p->stream, "f", self->life > 1.1920929e-07f ? (f64)(self->shield / self->life) : 0.0) != 0;
    if (p->set && Is(p, Key(0x1fb8)))
        return CRntFile_ReadFormat(p->stream, "fff", &self->field_0xe4, &self->field_0xe8, &self->field_0xec) != 0;
    if (Is(p, Key(0x1fc0))) {
        char buf[0x80];
        CRntFile_ReadFormat(p->stream, "s", buf);
        CRntString_AssignCString(&self->name, buf);
        return 1;
    }
    if (Is(p, Key(0x1470))) {
        fn_710115fd10(reinterpret_cast<UnkLifeComponentFull_ovb0000*>(self));
        return 1;
    }
    return fn_71010c4250(self, reinterpret_cast<UnkComponentProp*>(p));
}
