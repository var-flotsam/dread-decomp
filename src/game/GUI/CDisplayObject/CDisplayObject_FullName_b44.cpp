// GUI::CDisplayObject unit (tools/tu_map.py, the name from reflection): the object's full name, its own
// id's text prefixed by each ancestor's (parent at +0x120, id at +0x130) and a dot. The ids' pooled data
// holds the text at +0x10 and a count of reads at +0x44, as in src/game/CGameManager.cpp. Names and
// layouts are placeholders.
#include "base/global/CRntString.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;

struct UnkPooledStrId;

struct UnkGuiIdData_b44a3 {
    u8 pad_0x0[0x10];
    CRntString text;
    u8 pad_0x40[0x4];
    // A union member: the original reloads the id's data pointer after the increment.
    union {
        s32 reads;
    };
};

struct UnkGuiNamedNode_b44a3 {
    u8 pad_0x0[0x120];
    UnkGuiNamedNode_b44a3* parent;
    u8 pad_0x128[0x8];
    UnkGuiIdData_b44a3* id;  // 0x130
};

// A string built from a string id out of line (fn_710007eb20) and destroyed as one.
struct UnkGuiIdText_b44a3 {
    union {
        CRntString str;
    };

    explicit UnkGuiIdText_b44a3(const void* id) { fn_710007eb20(&str, static_cast<const UnkPooledStrId*>(id)); }
    UnkGuiIdText_b44a3(const UnkGuiIdText_b44a3& other);  // never called: the result is built in place
    ~UnkGuiIdText_b44a3() { fn_710007ecd4(&str); }
};

UnkGuiIdText_b44a3 fn_71011cd4f8(const UnkGuiNamedNode_b44a3* self) {
    UnkGuiIdText_b44a3 name(&self->id);
    for (const UnkGuiNamedNode_b44a3* n = self->parent; n; n = n->parent) {
        ++n->id->reads;
        fn_710007eaa0(&name.str, &static_cast<const CRntString&>(
                                     fn_710007eee0(&static_cast<const CRntString&>(fn_710007ef90(&n->id->text, ".")),
                                                   &name.str)));
    }
    return name;
}
