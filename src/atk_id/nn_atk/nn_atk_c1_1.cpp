// nn::atk optional parameters: bit 9's slot is an offset (from the record) to a reference, whose
// target is returned; without it, a default object.
#include "nn_atk_c1_params.h"

extern const char unk_71016539f0[] __attribute__((visibility("hidden")));  // the default

struct UnkAtkC1ParamsOwner4 {
    u32 field_0x0;
    UnkAtkC1Params params;  // 0x4
};
const char* fn_7101230ae4(const UnkAtkC1ParamsOwner4* self) {
    const UnkAtkC1Params* params = &self->params;
    if (!params->Has(9))
        return unk_71016539f0;
    u32 off = *reinterpret_cast<const u32*>(params->Slot(9));
    return reinterpret_cast<const UnkAtkC1Ref*>(reinterpret_cast<const char*>(self) + off)->Target();
}

// The same with the flag word at +0 (offsets from it).
const char* fn_710123093c(const UnkAtkC1Params* params) {
    if (!params->Has(9))
        return unk_71016539f0;
    u32 off = *reinterpret_cast<const u32*>(params->Slot(9));
    return reinterpret_cast<const UnkAtkC1Ref*>(reinterpret_cast<const char*>(params) + off)->Target();
}
