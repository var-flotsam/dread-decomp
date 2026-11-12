// behaviortree::CXParasiteTransformAction's unit (tools/tu_map.py, unit 1097): fn_71006e3fd8 resolves the
// action's target name (+0x30) and looks the entity up in the game manager by its hash. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflObject;

// As src/flag_id/fn_71004dd360.cpp declares it.
struct UnkStruct_0x68;
// As src/game/CCharClassDissolveFanMudEvent/CCharClassDissolveFanMudEvent_Execute_A6_b63.cpp declares them.
struct UnkWeakTarget;

// A string with 0x100 bytes of inline storage (0x148 bytes), as in
// src/game/behaviortree/CStringEqualsCondition/CStringEqualsCondition_Check_A2_b72.cpp.
struct UnkInlineString_ovb0130 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkInlineString_ovb0130() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
    }
    ~UnkInlineString_ovb0130() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

// An id built from a string out of line, released like CStrId.
struct UnkXParasiteBuiltId_ovb0130 {
    union {
        CStrId id;
    };

    explicit UnkXParasiteBuiltId_ovb0130(const CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkXParasiteBuiltId_ovb0130() { id.~CStrId(); }
};

// Declared with a void result in
// src/game/behaviortree/CStringEqualsCondition/CStringEqualsCondition_Check_A2_b72.cpp; here its result
// (whether the id resolved) is read.
struct UnkStringEqualsCondition_A2_b72;
struct UnkInlineString_A2_b72;
bool fn_7100395044(UnkStringEqualsCondition_A2_b72* self, CStrId* id, UnkInlineString_A2_b72* out);
using UnkResolveFn_ovb0130 = bool (*)(UnkStringEqualsCondition_A2_b72*, CStrId*, UnkInlineString_A2_b72*);

struct UnkXParasiteOwner_ovb0130 {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;  // 0x240
};

struct UnkXParasiteTransform_ovb0130 {
    u8 pad_0x0[0x18];
    UnkXParasiteOwner_ovb0130* owner;  // 0x18
    u8 pad_0x20[0x10];
    CStrId target;  // 0x30
};

struct UnkStrData_ovb0130 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

void fn_71006e3fd8(UnkXParasiteTransform_ovb0130* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    UnkReflObject* o = self->owner->ai;
    if (!o || !fn_710009d50c(o->vf_0x0(), fn_71006e5f28()))
        return;
    u64 key;
    alignas(8) u32 kind;
    UnkInlineString_ovb0130 str;
    if (self->target.data != &g_unk_7101d47108->empty &&
        reinterpret_cast<UnkResolveFn_ovb0130>(fn_7100395044)(
            reinterpret_cast<UnkStringEqualsCondition_A2_b72*>(self), &self->target,
            reinterpret_cast<UnkInlineString_A2_b72*>(&str))) {
        UnkGameManager* manager = g_unk_7101d47310;
        UnkXParasiteBuiltId_ovb0130 built(*str.Str());
        u32 kind_value = g_unk_71727bebb8;
        key = reinterpret_cast<UnkStrData_ovb0130*>(built.id.data)->hash;
        kind = kind_value;
        fn_7100debbc8(manager, &key, &kind);
    }
}
