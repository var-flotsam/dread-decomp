// CTunableMenuHelperClass' unit (tools/tu_map.py): takes an id's text after its last '|', and when it starts
// with the first of two global ids, sets `out` to the rest, without the second id when it follows. Names and
// layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// The two prefixes a menu name may start with.
__attribute__((visibility("hidden"))) extern CStrId g_unk_71727586c0[2];

// A string with 0x100 bytes of inline storage (0x148 bytes).
struct UnkInlineString_ladd810 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    const CRntString* Str() const { return reinterpret_cast<const CRntString*>(str); }
    UnkInlineString_ladd810() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        field_0x138 = 0;
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x140 = 0;
    }
    UnkInlineString_ladd810& operator=(const UnkInlineString_ladd810& other) {
        fn_710007eaa0(Str(), other.Str());
        return *this;
    }
    ~UnkInlineString_ladd810() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

// Room for a string id that the out-of-line constructor (fn_7100000850) builds; it is left without a
// destructor, since its string is always handed on.
union UnkStrIdSlot_ladd810 {
    CStrId id;
    UnkStrIdSlot_ladd810() {}
    ~UnkStrIdSlot_ladd810() {}
};

// `count` characters of `text` from `start` on (-1: to the end).
inline UnkInlineString_ladd810 Mid_ladd810(const CRntString* text, u32 start, u32 count = 0xffffffff) {
    u32 length = count == 0xffffffff ? text->length - start : count;
    const char* from = text->data + start;
    UnkInlineString_ladd810 part;
    fn_710007e7f8(part.Str(), from, length);
    return part;
}

// The id's string, counting the read.
inline const CRntString& Read_ladd810(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

// Whether two strings have the same text, by their keys.
inline bool SameText_ladd810(const CRntString& a, const CRntString& b) { return a.key == b.key; }

bool fn_7100add810(const CStrId* id, void* unused, CStrId* out) {
    UnkInlineString_ladd810 text;
    fn_710007eb6c(text.Str(), id);
    s32 bar = fn_710007fb10(text.Str(), '|', -1);
    if (bar >= 0)
        text = Mid_ladd810(text.Str(), bar + 1);

    const CStrId& first = g_unk_71727586c0[0];
    if (text.Str()->length <= first.data->String().length)
        return false;
    if (!SameText_ladd810(*Mid_ladd810(text.Str(), 0, first.data->String().length).Str(), Read_ladd810(first)))
        return false;
    text = Mid_ladd810(text.Str(), first.data->String().length);

    const CStrId& second = g_unk_71727586c0[1];
    if (SameText_ladd810(*Mid_ladd810(text.Str(), 0, second.data->String().length).Str(), Read_ladd810(second)))
        text = Mid_ladd810(text.Str(), second.data->String().length);

    UnkStrIdSlot_ladd810 slot;
    CStrId_ConstructFromRntString(&slot.id, *text.Str(), true);
    out->ReleaseAndTake(slot.id);
    return true;
}
