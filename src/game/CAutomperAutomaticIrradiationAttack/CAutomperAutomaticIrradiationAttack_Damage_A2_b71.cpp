// CAutomperAutomaticIrradiationAttack's unit (tools/tu_map.py unit 564): the attack's damage set-up for a
// request whose source (+0x60) has the id (+0x20) at 0x1d9 of fn_7100031894's string id table: the
// request's id at +0xb8 becomes the unit's static id, its value (fn_71008c1538) is what fn_7100789f5c gives
// for that id and the word at +0xc8 is set to 1. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_71726be008 __attribute__((visibility("hidden")));

struct UnkRequest;
void fn_71008c1538(UnkRequest* request, f32 value);
f32 fn_7100789f5c(const CStrId* id);

struct UnkIrradiationSource_A2_b71 {
    u8 pad_0x0[0x20];
    CStrId id;  // 0x20
};

struct UnkIrradiationRequest_A2_b71 {
    u8 pad_0x0[0x60];
    UnkIrradiationSource_A2_b71* source;  // 0x60
    u8 pad_0x68[0x50];
    CStrId id;  // 0xb8
    u8 pad_0xc0[0x8];
    u32 field_0xc8;
};

void fn_71003be028(void* self, void* unused, UnkIrradiationRequest_A2_b71* request) {
    UnkIrradiationSource_A2_b71* source = request->source;
    if (!source)
        return;
    const CStrId& id = base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x1d9];
    if (source->id.data != id.data)
        return;
    request->id = g_unk_71726be008;
    fn_71008c1538(reinterpret_cast<UnkRequest*>(request), fn_7100789f5c(&g_unk_71726be008));
    request->field_0xc8 = 1;
}
