// behaviortree::CBoolCondition's unit (tools/tu_map.py unit 514): describes the condition as text: the blackboard
// property's name (+0x40) followed by "?" when one is set, otherwise the constant (+0x38) as "true" or "false".
// Names and layouts are placeholders.
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;

// A blackboard property name: its text at +0x10 and a use count at +0x44.
struct UnkBoolPropertyName_l39b274 {
    u8 unk_0x0[0x10];
    CRntString text;
    u8 unk_0x40[0x4];
    u32 uses;
};

struct UnkBoolCondition_l39b274 {
    u8 unk_0x0[0x38];
    bool value;  // 0x38
    u8 unk_0x39[0x7];
    UnkBoolPropertyName_l39b274* property;  // 0x40
};

struct UnkText_l39b274 {
    u8 unk_0x0[0x30];
    UnkText_l39b274() {}
};

UnkText_l39b274 fn_710039b274(UnkBoolCondition_l39b274* self) {
    UnkText_l39b274 text;
    CRntString_ConstructFromCString(reinterpret_cast<base::UnkNamedEntry*>(&text), g_unk_710162e2ed);
    if (self->property != reinterpret_cast<UnkBoolPropertyName_l39b274*>(&g_unk_7101d47108->empty)) {
        ++self->property->uses;
        CRntString_Append(reinterpret_cast<CRntString*>(&text), &self->property->text);
        CRntString_AppendCString(&text, "?");
    } else {
        const char* name = self->value ? "true" : "false";
        CRntString_AppendCString(&text, name);
    }
    return text;
}
