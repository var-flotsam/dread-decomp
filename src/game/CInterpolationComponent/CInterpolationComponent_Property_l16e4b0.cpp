// CInterpolationComponent's unit (tools/tu_map.py unit 2718): the property handler for the position
// (+0x40) and angles (+0x4c) ("o" vectors): writes the value out or reads it in; any other property
// goes to the base component's handler (fn_71010c4250). Names and layouts are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/global/CRntString.h"
#include "game/UnkComponent.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CRntString;
using unk_prop::UnkProp;

struct UnkInterpolationVec_l16e4b0 {
    f32 x;
    f32 y;
    f32 z;

    UnkInterpolationVec_l16e4b0& operator=(const UnkInterpolationVec_l16e4b0& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

struct UnkInterpolationProps_l16e4b0 {
    u8 pad_0x0[0x40];
    UnkInterpolationVec_l16e4b0 pos;  // 0x40
    UnkInterpolationVec_l16e4b0 ang;  // 0x4c
};

// Reads a vector from the property's stream into `out`: 1 when it read one, else 0.
static inline u32 ReadVec(UnkProp* p, UnkInterpolationVec_l16e4b0* out) {
    UnkInterpolationVec_l16e4b0* value;
    if (CRntFile_ReadFormat(p->stream, "o", &value) != 8)
        return 0;
    *out = *value;
    return 1;
}

u32 CInterpolationComponent_HandlePosAngProperty(UnkInterpolationProps_l16e4b0* self, UnkProp* p) {
    if (unk_prop::IsName(p, CRntString("Pos")) && !p->set)
        return CRntFile_WriteFormat(p->stream, "o", &self->pos) == 8;
    if (unk_prop::IsName(p, CRntString("Pos")) && p->set)
        return ReadVec(p, &self->pos);
    if (unk_prop::IsName(p, CRntString("Ang")) && !p->set)
        return CRntFile_WriteFormat(p->stream, "o", &self->ang) == 8;
    if (unk_prop::IsName(p, CRntString("Ang")) && p->set)
        return ReadVec(p, &self->ang);
    return fn_71010c4250(reinterpret_cast<UnkBaseComponent*>(self), reinterpret_cast<UnkComponentProp*>(p));
}
