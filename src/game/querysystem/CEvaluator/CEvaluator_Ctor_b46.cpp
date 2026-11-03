// querysystem::CEvaluator's constructor (unit 378 of tools/tu_map.py, the name from reflection): the
// evaluators' base. Sets the vtable, names the evaluator "XXXX" (a placeholder the derived
// constructors replace), sets the weight-like value at +0x10 to 100 and copies the empty string id
// (g_unk_7101d470e0) to +0x18. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStrId;

extern char g_unk_7101a21230[];  // CEvaluator's vtable

// Signature as the derived constructors declare it.
struct UnkQueryParam;

struct UnkEvaluator_b46a1 {
    char* vtable;
    UnkStrId name;
    f32 field_0x10;
    CStrId field_0x18;
};

void fn_71002b7fb0(UnkQueryParam* param) {
    UnkEvaluator_b46a1* self = reinterpret_cast<UnkEvaluator_b46a1*>(param);
    self->vtable = g_unk_7101a21230 + 0x10;
    new (&self->name) UnkStrId("XXXX", true);
    self->field_0x10 = 100.0f;
    new (&self->field_0x18) CStrId(g_unk_7101d470e0);
}
