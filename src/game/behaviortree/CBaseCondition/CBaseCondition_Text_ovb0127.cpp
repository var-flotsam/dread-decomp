// behaviortree::CBaseCondition's unit (tools/tu_map.py): the condition's description. Names and layouts are
// placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;

// The names of the results a condition can map success and failure to (1 to 7).
extern const char* const g_unk_7101a26f80[] __attribute__((visibility("hidden")));

static inline const char* ResultName(s32 result) {
    if (result >= 1 && result <= 7)
        return g_unk_7101a26f80[result - 1];
    return g_unk_710162e2ed;
}

struct UnkBaseCondition_ovb0127 {
    void** vtable;
    u8 pad_0x8[0x24];
    bool negated;       // 0x2c
    u8 pad_0x2d[0x3];
    s32 on_success;     // 0x30: 5 when unchanged
    s32 on_failed;      // 0x34: 6 when unchanged

    CRntString vf_0xa8() {
        return reinterpret_cast<CRntString (*)(UnkBaseCondition_ovb0127*)>(vtable[0xa8 / 8])(this);
    }
};

// The condition's description: the base one, "NOT " in front when negated, then the results it
// maps success and failure to when they aren't the defaults.
CRntString fn_7100394588(UnkBaseCondition_ovb0127* self) {
    CRntString text = self->vf_0xa8();
    if (self->negated) {
        CRntString negated = fn_710007f050("NOT ", &text);
        fn_710007eaa0(&text, &negated);
    }
    if (self->on_success != 5) {
        fn_710007f564(&text, "(OnSuccess=");
        fn_710007f564(&text, ResultName(self->on_success));
        fn_710007f564(&text, ")");
    }
    if (self->on_failed != 6) {
        fn_710007f564(&text, "(OnFailed=");
        fn_710007f564(&text, ResultName(self->on_failed));
        fn_710007f564(&text, ")");
    }
    return text;
}
