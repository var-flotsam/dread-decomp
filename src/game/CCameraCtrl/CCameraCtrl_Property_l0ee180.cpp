// CCameraCtrl's unit (tools/tu_map.py, unit 2693): a camera controller's property handler for its
// position, destination position, direction and destination direction ("o" vectors): writes the value
// out or reads it in; returns -1 for any other property. Names and layouts are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/global/CRntString.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CRntString;
using unk_prop::UnkProp;

struct UnkCameraVec_l0ee180 {
    f32 x;
    f32 y;
    f32 z;

    UnkCameraVec_l0ee180& operator=(const UnkCameraVec_l0ee180& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

struct UnkCameraCtrlProps_l0ee180 {
    u8 pad_0x0[0x38];
    UnkCameraVec_l0ee180 pos;  // 0x38
    UnkCameraVec_l0ee180 dir;  // 0x44
    u8 pad_0x50[0x68 - 0x50];
    UnkCameraVec_l0ee180 dest_pos;  // 0x68
    UnkCameraVec_l0ee180 dest_dir;  // 0x74
};
static_assert(__builtin_offsetof(UnkCameraCtrlProps_l0ee180, dest_dir) == 0x74, "layout");

// Reads a vector from the property's stream into `out`: 1 when it read one, else 0.
static inline s32 ReadVec(UnkProp* p, UnkCameraVec_l0ee180* out) {
    UnkCameraVec_l0ee180* value;
    if (CRntFile_ReadFormat(p->stream, "o", &value) != 8)
        return 0;
    *out = *value;
    return 1;
}

s32 CCameraCtrl_ReadWritePosDirProperty(UnkCameraCtrlProps_l0ee180* self, UnkProp* p) {
    if (unk_prop::IsName(p, CRntString("Pos")) && !p->set)
        return CRntFile_WriteFormat(p->stream, "o", &self->pos) == 8;
    if (unk_prop::IsName(p, CRntString("Pos")) && p->set)
        return ReadVec(p, &self->pos);
    if (unk_prop::IsName(p, CRntString("DestPos")) && !p->set)
        return CRntFile_WriteFormat(p->stream, "o", &self->dest_pos) == 8;
    if (unk_prop::IsName(p, CRntString("DestPos")) && p->set)
        return ReadVec(p, &self->dest_pos);
    if (unk_prop::IsName(p, CRntString("Dir")) && !p->set)
        return CRntFile_WriteFormat(p->stream, "o", &self->dir) == 8;
    if (unk_prop::IsName(p, CRntString("Dir")) && p->set)
        return ReadVec(p, &self->dir);
    if (unk_prop::IsName(p, CRntString("DestDir")) && !p->set)
        return CRntFile_WriteFormat(p->stream, "o", &self->dest_dir) == 8;
    if (unk_prop::IsName(p, CRntString("DestDir")) && p->set)
        return ReadVec(p, &self->dest_dir);
    return -1;
}
