// CCharClassAIComponent's unit (tools/tu_map.py, unit 616): more of the string-id kind setters of
// src/game/CCharClassAIComponent/UnkStrIdKindSetters_b43.cpp. The door kinds (fn_71004159d0: the text split at
// "|", "OneWayDoor" giving bit 1 and "All" every bit) and the setter of +0x230 with its kind at +0x238
// (fn_71004149d8). Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/UnkNodeOwner.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// A string id's data as the text reads see it: the text at +0x10 and a use count at +0x44 (a union
// member: the original reloads the id after the increment).
struct UnkStrIdText_ovb0113 {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[4];
    union {
        s32 field_0x44;
    };
};

static inline UnkStrIdText_ovb0113* Text(const CStrId& id) {
    return reinterpret_cast<UnkStrIdText_ovb0113*>(id.data);
}

static inline void InitStrList_ovb0113(UnkStrList30* list) {
    list->field_0x8 = 0;
    list->field_0x10 = nullptr;
    list->field_0x18 = nullptr;
    list->field_0x20 = fn_7100083990();
    *reinterpret_cast<u32*>(list->pad_0x28) = 0x13;
    list->vtable = g_unk_7101a1a4a8 + 2;
    list->field_0x30 = g_unk_7101a1a4c8 + 0x10;
    list->field_0x38 = nullptr;
    fn_710008377c(&list->field_0x40);
    *reinterpret_cast<u32*>(list->pad_0x68) = 0;
    list->field_0x6c = 0;
}

using UnkSetupStrList = UnkStrList30;
void fn_71000730a0(const char* str, const char* separators, UnkSetupStrList* out);

static inline s32 DoorPartKind(const CRntString* part) {
    if (fn_710007f140(part, "OneWayDoor"))
        return 1;
    return fn_710007f140(part, "All") ? -1 : 0;
}

s32 fn_71004159d0(const char* text) {
    UnkStrList30 parts;
    InitStrList_ovb0113(&parts);
    fn_71000730a0(text, "|", &parts);
    s32 kinds = 0;
    for (UnkListNode<0x30>* node = parts.field_0x10; node; node = node->next) {
        const CRntString* part = reinterpret_cast<const CRntString*>(node);
        if (fn_710007f140(part, "All"))
            kinds = -1;
        else
            kinds |= DoorPartKind(part);
    }
    fn_71000732d4(&parts);
    return kinds;
}

static inline bool NameIs(const CStrId* id, const char* name) {
    CRntString s(name);
    ++Text(*id)->field_0x44;
    return Text(*id)->str.key == s.key;
}

static inline s32 DoorKindByName(const CStrId* id) {
    if (NameIs(id, "OneWayDoor"))
        return 1;
    return NameIs(id, "All") ? -1 : 0;
}

template <u32 Offset, auto Parse, auto ByName>
static inline void SetKindFromId(char* self, const CStrId& id) {
    *reinterpret_cast<CStrId*>(self + Offset) = id;
    s32 kind;
    if (id.data == &g_unk_7101d47108->empty) {
        kind = 0;
    } else {
        ++Text(id)->field_0x44;
        if (id.data != &g_unk_7101d47108->empty && fn_710007fb9c(&Text(id)->str, "|")) {
            ++Text(id)->field_0x44;
            const char* s = Text(id)->str.data;
            kind = Parse(s ? s : g_unk_710162e2ed);
        } else {
            kind = ByName(&id);
        }
    }
    *reinterpret_cast<s32*>(self + Offset + 8) = kind;
}

void fn_71004149d8(char* self, const CStrId& id) { SetKindFromId<0x230, fn_71004159d0, DoorKindByName>(self, id); }
