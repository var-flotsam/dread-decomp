// behaviortree::CStorePosInBlcAction's unit (tools/tu_map.py, unit 463): the node's debug text: the source
// ("Owner" without a property id at +0x38, else the blackboard value it names through fn_7100395044, or
// "Unknown" when that fails), " Position -> ", then the name of the target at +0x30 (its pooled data's text,
// counting the read). Names and layouts are placeholders.
#include "../../../game_aca57c/UnkBufferString.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

using UnkDebugBuffer_ovb0128 = UnkBufferString<0x100, g_unk_7101a194b8>;

CRntString fn_710007ef90(const CRntString* a, const char* b);  // concatenation

// Signature as other files declare it (the original returns whether the value was found).
struct UnkStringEqualsCondition_A2_b72;
struct UnkInlineString_A2_b72;
void fn_7100395044(UnkStringEqualsCondition_A2_b72* self, CStrId* id, UnkInlineString_A2_b72* out);

struct UnkBlcName_ovb0128 {
    u8 pad_0x0[0x10];
    CRntString text;  // 0x10
    u8 pad_0x40[0x4];
    // A union member: the original reloads the name's pointer after the increment.
    union {
        s32 reads;  // 0x44
    };
};

struct UnkStorePosInBlc_ovb0128 {
    u8 pad_0x0[0x30];
    UnkBlcName_ovb0128* target;  // 0x30
    CStrId source;  // 0x38
};

// The result, moved in from a string (fn_710007eab4) and destroyed as one.
struct UnkDebugText_ovb0128 {
    union {
        CRntString str;
    };

    explicit UnkDebugText_ovb0128(CRntString* from) { fn_710007eab4(&str, from); }
    UnkDebugText_ovb0128(const UnkDebugText_ovb0128& other);  // never called: the result is built in place
    ~UnkDebugText_ovb0128() { fn_710007ecd4(&str); }
};

UnkDebugText_ovb0128 fn_7100376104(UnkStorePosInBlc_ovb0128* self) {
    UnkDebugBuffer_ovb0128 text(g_unk_710162e2ed);
    UnkDebugBuffer_ovb0128 source;
    if (self->source.data == &g_unk_7101d47108->empty) {
        fn_710007e77c(source.Str(), "Owner");
    } else if (!reinterpret_cast<bool (*)(UnkStringEqualsCondition_A2_b72*, CStrId*, UnkInlineString_A2_b72*)>(
                   fn_7100395044)(reinterpret_cast<UnkStringEqualsCondition_A2_b72*>(self), &self->source,
                                  reinterpret_cast<UnkInlineString_A2_b72*>(&source))) {
        fn_710007e77c(source.Str(), "Unknown");
    }
    fn_710007f3b4(text.Str(), &static_cast<const CRntString&>(fn_710007ef90(source.Str(), " Position -> ")));
    ++self->target->reads;
    fn_710007f3b4(text.Str(), &self->target->text);
    return UnkDebugText_ovb0128(text.Str());
}
