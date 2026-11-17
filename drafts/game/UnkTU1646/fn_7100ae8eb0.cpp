// Unit 1646: a blackboard upgrade step: unless the blackboard already has the section that area names,
// adds that area to the list of areas the "MINIMAP" section stores. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/CCollectionType.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2564.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

extern UnkPropertyEx<> g_unk_71727ca150;

static inline const UnkQualifiedId& DefId_lae8eb0(UnkPropertyEx<>& def) {
    UnkQualifiedId& qid = ItemQualifiedId(def);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(def), g_unk_7101d470e0);
    return qid;
}

// A section's property values, and the end of them.
static inline UnkReflValue* Values_lae8eb0(UnkPropertyArrays& properties) {
    return static_cast<UnkReflValue*>(properties.values);
}
static inline UnkReflValue* End_lae8eb0(UnkPropertyArrays& properties) {
    return &Values_lae8eb0(properties)[properties.count];
}

// The value stored under `name` in a section's properties, or the end of them.
static inline UnkReflValue* FindValue_lae8eb0(UnkPropertyArrays& properties, const CStrId& name) {
    for (u64 i = 0; i < properties.count; ++i)
        if (properties.ids[i].data == name.data)
            return &Values_lae8eb0(properties)[i];
    return End_lae8eb0(properties);
}

// Reads the list of ids `def` names in the blackboard section `section` into `out`; false when the section,
// the property or its value is missing.
static inline bool ReadIds_lae8eb0(CBlackboard* blackboard, const CStrId& section, UnkPropertyEx<>& def,
                                   CRntVector<CStrId>* out) {
    const UnkQualifiedId& name = DefId_lae8eb0(def);
    CBlackboard::CSection* s = fn_7101047270(blackboard, section);
    if (!s)
        return false;
    UnkPropertyArrays& properties = s->data.properties;
    UnkReflValue* v = FindValue_lae8eb0(properties, name.id);
    if (v == End_lae8eb0(properties))
        return false;
    const char* type = v->type;
    if (!type)
        return false;
    void* p = v->field_0x14 == 1 || reinterpret_cast<const CType*>(type)->m_size > 8 ? v->field_0x0 : v;
    if (!p)
        return false;
    UnkTypedRef dst{&g_unk_7101d5c998, out};
    UnkTypedRef src;
    fn_71000a16e0(&src, type, p);
    return fn_710009d7a8(&g_unk_7101d5c998, &dst, &src);
}

// Stores the list of ids `value` points to under `name` in the section `section`, unless the blackboard
// is locked.
static inline void SetIds_lae8eb0(CBlackboard* blackboard, const CStrId& section, const UnkQualifiedId& name,
                                  CRntVector<CStrId>* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = &g_unk_7101d5c998;
            v.value = value;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)), &v);
        }
    }
}

bool fn_7100ae8eb0(CBlackboard* blackboard) {
    if (fn_7101047208(blackboard, static_cast<const CStrId&>(g_unk_71727c8018)))
        return true;

    CRntVector<CStrId> areas(0xf);
    bool ok = ReadIds_lae8eb0(blackboard, CStrId("MINIMAP", true), g_unk_71727ca150, &areas);
    if (ok) {
        areas.PushBackCopy(g_unk_71727c83c0);
        SetIds_lae8eb0(blackboard, CStrId("MINIMAP", true), DefId_lae8eb0(g_unk_71727ca150), &areas);
    }
    areas.Free();
    return ok;
}
