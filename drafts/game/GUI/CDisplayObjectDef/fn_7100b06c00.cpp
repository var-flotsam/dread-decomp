// GUI::CDisplayObjectDef's unit (tools/tu_map.py unit 1669): fn_7100b06c00 searches the tree under a
// definition for the one named `name` (ignoring case), resuming among the items from the cursor at +0x88
// and clearing it when the items run out. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU57.h"
#include "types.h"

#include "../../../../src/game_aca57c/UnkBufferString.h"

struct UnkCompared_0x30;

struct UnkDisplayDef_lb06c00 {
    void* vtable;
    base::global::CRntString name;                           // 0x8
    base::global::CRntVector<void*> children;                // 0x38
    base::global::CRntVector<UnkDisplayDef_lb06c00*> items;  // 0x60
    UnkDisplayDef_lb06c00** cursor;                          // 0x88: where the search resumes
};
static_assert(sizeof(UnkDisplayDef_lb06c00) == 0x90, "UnkDisplayDef_lb06c00 layout");

using UnkDisplayName_lb06c00 = UnkBufferString<0x100, g_unk_7101a194b8>;

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

// The definition named `name` (ignoring case) in the tree under this one, this one included.
UnkDisplayDef_lb06c00* fn_7100b06c00(UnkDisplayDef_lb06c00* self, const base::global::CRntString& name) {
    bool equal;
    {
        char buffer[0x100];
        ToLower(buffer, self->name.data);
        UnkDisplayName_lb06c00 a(buffer);
        ToLower(buffer, name.data);
        UnkDisplayName_lb06c00 b(buffer);
        equal = fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(a.Str()),
                              reinterpret_cast<const UnkCompared_0x30*>(b.Str()));
    }
    if (equal)
        return self;
    base::global::CRntVector<UnkDisplayDef_lb06c00*>& items = self->items;
    if (!self->cursor)
        self->cursor = items.data;
    for (; self->cursor != items.data + items.count; ++self->cursor) {
        if (UnkDisplayDef_lb06c00* found = fn_7100b06c00(*self->cursor, name))
            return found;
    }
    self->cursor = nullptr;
    return nullptr;
}
