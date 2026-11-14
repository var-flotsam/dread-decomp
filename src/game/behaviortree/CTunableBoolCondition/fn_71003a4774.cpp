// behaviortree::CTunableBoolCondition: builds the text describing the condition, either "true"/"false"
// or "<first>:<second>?" from two string ids. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"
#include "game/UnkTU57.h"
#include "base/global/CFilePathStrId.h"

using base::global::CRntString;


// A pooled string entry: a string at 0x10 and a reference count at 0x44.
struct UnkTextEntry_l3a4774 {
    u8 pad_0x0[0x10];
    CRntString text;
    u8 pad_0x40[0x4];
    union {
        s32 ref_count;
    };
};
static_assert(__builtin_offsetof(UnkTextEntry_l3a4774, ref_count) == 0x44, "ref_count offset");

struct UnkTunableBoolCondition_l3a4774 {
    u8 pad_0x0[0x38];
    bool field_0x38;
    u8 pad_0x39[7];
    UnkTextEntry_l3a4774* field_0x40;
    UnkTextEntry_l3a4774* field_0x48;
};


// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
CRntString fn_71003a4774(UnkTunableBoolCondition_l3a4774* self) {
    CRntString result(g_unk_710162e2ed);
    UnkTextEntry_l3a4774* first = self->field_0x40;
    void* empty = &g_unk_7101d47108->empty;
    if (first == empty || self->field_0x48 == empty) {
        const char* word = self->field_0x38 == false ? "false" : "true";
        fn_710007f564(&result, word);
        return result;
    }
    first->ref_count++;
    fn_710007f3b4(&result, &self->field_0x40->text);
    fn_710007f564(&result, ":");
    self->field_0x48->ref_count++;
    fn_710007f3b4(&result, &self->field_0x48->text);
    fn_710007f564(&result, "?");
    return result;
}
