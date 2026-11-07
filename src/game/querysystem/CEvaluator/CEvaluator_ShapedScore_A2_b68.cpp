// querysystem::CEvaluator's unit (unit 378 of tools/tu_map.py, the name from reflection): an evaluator's
// score, shaped by curve modifier letters. Takes the raw value from vtable slot 0x20; when the string id at +0x18
// isn't the empty one, bumps a counter in the id's string data (+0x44) and reshapes the value by the id's text
// (fn_7100076020); then scales it by the weight at +0x10. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

f32 fn_7100076020(f32, const char*);

struct UnkShapedStrData_A2_b68 {
    u8 pad_0x0[0x10];
    const char* text;
    u8 pad_0x18[0x44 - 0x18];
    union {
        u32 use_count;
    };
};

struct UnkShapedEvaluator_A2_b68 {
    void** vtable;
    u8 pad_0x8[0x8];
    f32 weight;
    u8 pad_0x14[0x4];
    union {
        UnkShapedStrData_A2_b68* modifiers;
    };

    f32 vf_0x20(void* a, void* b) {
        return reinterpret_cast<f32 (*)(UnkShapedEvaluator_A2_b68*, void*, void*)>(vtable[4])(this, a, b);
    }
};

// `a` and `b` (x1, x2) reach the raw value's virtual untouched in the original; the caller sets x2
// (tools/forwarded_args.py).
f32 fn_71002b80ec(UnkShapedEvaluator_A2_b68* self, void* a, void* b) {
    f32 t = self->vf_0x20(a, b);
    if (reinterpret_cast<void*>(self->modifiers) != &g_unk_7101d47108->empty) {
        ++self->modifiers->use_count;
        const char* text = self->modifiers->text;
        t = fn_7100076020(t, text ? text : g_unk_710162e2ed);
    }
    return t * self->weight;
}
