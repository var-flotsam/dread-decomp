// Unit 2744 (gui/scripts/guimanager.lua): the root's skin whose name equals `name`, compared
// case-insensitively (both lower-cased into strings with inline storage); null when none.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkPtrList_A6;

// A string with 0x100 bytes of inline storage (0x148 bytes).
struct UnkInlineString_l1b90a8 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    explicit UnkInlineString_l1b90a8(const char* text) {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
        fn_710007e77c(Str(), text);
    }
    ~UnkInlineString_l1b90a8() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

// A skin: its name first.
struct UnkSkin_l1b90a8 {
    CStrId name;
};

// The root's skins (base::global::CRntVector<GUI::CSkin*>).
struct UnkSkinVector_l1b90a8 {
    UnkSkin_l1b90a8** data;
    u32 count;
};
UnkSkin_l1b90a8** fn_7100b79514(UnkSkinVector_l1b90a8* self, u32 index);  // CRntVector<GUI::CSkin*>::GrowAt

// The root as this reads it: its skins at +0x38.
struct UnkGuiRootSkins_l1b90a8 {
    u8 pad_0x0[0x38];
    UnkSkinVector_l1b90a8* skins;
};

// Copies `src` lower-cased (ASCII) into `dst`.
static inline void ToLower(char* dst, const char* src) {
    dst[0] = '\0';
    u32 length = __builtin_strlen(src);
    if (length) {
        for (u32 i = 0; i < length; ++i) {
            char c = src[i];
            dst[i] = (u8)(c - 'A') < 26 ? c + ('a' - 'A') : c;
        }
        dst[length] = '\0';
    }
}

// The text of a string id, counting the read.
static inline const char* Text(const CStrId& id) {
    ++id.data->reads;
    return id.data->String().data;
}

UnkPtrList_A6* fn_71011b90a8(UnkGuiRoot* root, const CStrId* name) {
    UnkGuiRootSkins_l1b90a8* r = reinterpret_cast<UnkGuiRootSkins_l1b90a8*>(root);
    for (u32 i = 0; i < r->skins->count; ++i) {
        UnkSkin_l1b90a8* skin = *fn_7100b79514(r->skins, i);
        bool equal;
        {
            char buffer[0x100];
            ToLower(buffer, Text(skin->name));
            UnkInlineString_l1b90a8 a(buffer);
            ToLower(buffer, Text(*name));
            UnkInlineString_l1b90a8 b(buffer);
            equal = fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(a.Str()),
                                  reinterpret_cast<const UnkCompared_0x30*>(b.Str()));
        }
        if (equal)
            return reinterpret_cast<UnkPtrList_A6*>(skin);
    }
    return nullptr;
}
