// Unit 2566 (CMinimapManager::CTunableMinimapManager): reads a rectangle from the game manager's
// blackboard (the section g_unk_71727cc990, under the sub-id of `id` within a named definition built
// on first use); empty (inverted) bounds when it is not there. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU106.h"
#include "game/UnkTU2566.h"
#include "game/UnkSubId_7100fc4b34.h"
#include "types.h"

using base::global::CStrId;

typedef u32 a_u32;

// A rectangle returned in memory, copied word by word (as in src/game/CMinimapManager.cpp).
struct UnkMinimapRect {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
    UnkMinimapRect() {}
    UnkMinimapRect(f32 x0, f32 y0, f32 x1, f32 y1) : min_x(x0), min_y(y0), max_x(x1), max_y(y1) {}
    UnkMinimapRect(const UnkMinimapRect& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        ((a_u32*)this)[2] = ((const a_u32*)&o)[2];
        ((a_u32*)this)[3] = ((const a_u32*)&o)[3];
    }
    UnkMinimapRect& operator=(const UnkMinimapRect& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        ((a_u32*)this)[2] = ((const a_u32*)&o)[2];
        ((a_u32*)this)[3] = ((const a_u32*)&o)[3];
        return *this;
    }
};

struct UnkMapNamedDef_b49 : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedName name;
};

static inline UnkSubId_7100fc4b34 SubId(UnkMapNamedDef_b49& def, const CStrId& name) {
    if (!def.name.str || !def.name.str[0])
        fn_7100ae9d7c(&def.name, &def, base::global::CStrId::Empty());
    return fn_71003327a4(&def.name, &name);
}

// The rectangle's reflected type.
struct UnkMinimapRectTag;
template <>
struct base::reflection::UnkFieldTraits<UnkMinimapRectTag> {
    static const CType* type() { return reinterpret_cast<const CType*>(g_unk_7108b6d530); }
};

UnkMinimapRect fn_7100e984a4(UnkMinimapOwner* /* self */, const CStrId* id) {
    CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
    UnkMinimapRect bounds(__FLT_MAX__, __FLT_MAX__, -__FLT_MAX__, -__FLT_MAX__);
    UnkSubId_7100fc4b34 name = SubId(g_unk_71727c9e00, *id);
    if (CBlackboard::CSection* section = fn_7101047270(blackboard, g_unk_71727cc990))
        base::refl_GetKeyedValue<UnkMinimapRectTag>(reinterpret_cast<char*>(section),
                                                    reinterpret_cast<const base::UnkStrIdMember*>(&name.id), &bounds);
    return bounds;
}
