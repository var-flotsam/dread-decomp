// Unit 118 (no reflected class): sets a path on the object at +0x128 (fn_71001bb9c8, src/base/UnkUnit246/
// UnkUnit246_SetMaterial_b40.cpp) through a copy of the path id. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkObject246;
bool fn_71001bb9c8(UnkObject246* self, const CFilePathStrId* path);

struct UnkOwner118 {
    u8 pad_0x0[0x128];
    UnkObject246* field_0x128;
};

bool fn_7100141b64(UnkOwner118* self, const CFilePathStrId* path) {
    CFilePathStrId copy(*path);
    return fn_71001bb9c8(self->field_0x128, &copy);
}
