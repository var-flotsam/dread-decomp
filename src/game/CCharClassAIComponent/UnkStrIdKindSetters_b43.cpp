// Four setters in CCharClassAIComponent's unit (tools/tu_map.py unit 616): each assigns a string id
// to a member (+0x1f0, +0x200, +0x210, +0x220) and stores the kind derived from it in the word
// 8 bytes after: 0 for the empty id; when the id's text contains "|" the kind its text parses to
// (fn_7100415050 or fn_710041536c); otherwise the kind matched by name (fn_710041518c and its
// copies). Reading the text bumps the id's use count. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/UnkNodeOwner.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// A string id's data as the text reads see it: the text at +0x10 and a use count at +0x44 (a union
// member: the original reloads the id after the increment).
struct UnkStrIdText_b43a1 {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[4];
    union {
        s32 field_0x44;
    };
};

s32 fn_7100415050(const char* text);
s32 fn_710041518c(const CStrId* id);
u32 fn_710026d2e4(const char* text);  // as src/game/navmesh/CNavMesh.cpp declares it
s32 fn_710041527c(const CStrId* id);
s32 fn_710041536c(const char* text);
s32 fn_710041545c(const CStrId* id);

static inline UnkStrIdText_b43a1* Text(const CStrId& id) {
    return reinterpret_cast<UnkStrIdText_b43a1*>(id.data);
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
            kind = Parse(s ? s : "");
        } else {
            kind = ByName(&id);
        }
    }
    *reinterpret_cast<s32*>(self + Offset + 8) = kind;
}

void fn_71004145a8(char* self, const CStrId& id) { SetKindFromId<0x1f0, fn_7100415050, fn_710041518c>(self, id); }
void fn_71004146b4(char* self, const CStrId& id) { SetKindFromId<0x200, fn_710026d2e4, fn_710041527c>(self, id); }
void fn_71004147c0(char* self, const CStrId& id) { SetKindFromId<0x210, fn_710041536c, fn_710041545c>(self, id); }
void fn_71004148cc(char* self, const CStrId& id) { SetKindFromId<0x220, fn_710041536c, fn_710041545c>(self, id); }

// The kinds matched by name (fn_710041518c, fn_710041527c, fn_710041545c): each name is built as a
// string, compared with the id's text by key, and gives its bit; "All" gives every bit, anything
// else 0.
static inline bool NameIs(const CStrId* id, const char* name) {
    CRntString s(name);
    ++Text(*id)->field_0x44;
    return Text(*id)->str.key == s.key;
}

s32 fn_710041518c(const CStrId* id) {
    if (NameIs(id, "Poly"))
        return 1;
    if (NameIs(id, "Edge"))
        return 2;
    return NameIs(id, "All") ? -1 : 0;
}

s32 fn_710041527c(const CStrId* id) {
    if (NameIs(id, "Tunnel"))
        return 1;
    if (NameIs(id, "Water"))
        return 2;
    return NameIs(id, "All") ? -1 : 0;
}

s32 fn_710041545c(const CStrId* id) {
    if (NameIs(id, "StaticItems"))
        return 1;
    if (NameIs(id, "DynamicItems"))
        return 2;
    if (NameIs(id, "BreakableTilesPowerBeam"))
        return 4;
    if (NameIs(id, "BreakableTilesBomb"))
        return 8;
    if (NameIs(id, "BreakableTilesMissile"))
        return 0x10;
    if (NameIs(id, "BreakableTilesSuperMissile"))
        return 0x20;
    if (NameIs(id, "BreakableTilesPowerBomb"))
        return 0x40;
    if (NameIs(id, "BreakableTilesScrewAttack"))
        return 0x80;
    if (NameIs(id, "BreakableTilesWeight"))
        return 0x100;
    if (NameIs(id, "BreakableTilesBabyHatchling"))
        return 0x200;
    if (NameIs(id, "BreakableTilesSpeedBooster"))
        return 0x400;
    if (NameIs(id, "DestructibleItems"))
        return 0x800;
    return NameIs(id, "All") ? -1 : 0;
}

// The kinds parsed from text (fn_7100414070, fn_7100415050, fn_710041536c): the text is split at "|" into a
// string list (fn_71000730a0; constructed inline as in src/game/UnkSubareaSetupState.cpp,
// destroyed by fn_71000732d4) and the kinds of the parts are or-ed together; a part "All" sets
// every bit.
static inline void InitStrList_b43a1(UnkStrList30* list) {
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
// The out-of-line part kinds, declared as src/lifted/ defines them.
u32 fn_7100414bec(char* part);
u32 fn_71004157f4(char* part);

static inline s32 LiftedKind_7100414bec(const CRntString* part) {
    return fn_7100414bec(reinterpret_cast<char*>(const_cast<CRntString*>(part)));
}
static inline s32 LiftedKind_71004157f4(const CRntString* part) {
    return fn_71004157f4(reinterpret_cast<char*>(const_cast<CRntString*>(part)));
}

template <auto PartKind>
static inline s32 ParseKinds(const char* text) {
    UnkStrList30 parts;
    InitStrList_b43a1(&parts);
    fn_71000730a0(text, "|", &parts);
    s32 kinds = 0;
    for (UnkListNode<0x30>* node = parts.field_0x10; node; node = node->next) {
        const CRntString* part = reinterpret_cast<const CRntString*>(node);
        if (fn_710007f140(part, "All"))
            kinds = -1;
        else
            kinds |= PartKind(part);
    }
    fn_71000732d4(&parts);
    return kinds;
}

static inline s32 PolyEdgeKind(const CRntString* part) {
    if (fn_710007f140(part, "Poly"))
        return 1;
    if (fn_710007f140(part, "Edge"))
        return 2;
    return fn_710007f140(part, "All") ? -1 : 0;
}

s32 fn_7100415050(const char* text) { return ParseKinds<PolyEdgeKind>(text); }
u32 fn_7100414070(const char* text) { return ParseKinds<LiftedKind_7100414bec>(text); }
s32 fn_710041536c(const char* text) { return ParseKinds<LiftedKind_71004157f4>(text); }
