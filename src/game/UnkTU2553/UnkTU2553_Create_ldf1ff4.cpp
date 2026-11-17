// CGameManager::CTunableGameManager's unit (tools/tu_map.py unit 2553): creates the object kept under a string
// id in the map at +0x2290, deleting and erasing any object already there, and returns it. Names and layouts
// are placeholders.
#include <new>

#include "base/global/CRntDictionary.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

// The object created (0x50 bytes; fn_7101051530 constructs it).
struct UnkIdObject_df1ff4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    u8 pad_0x8[0x8];
    CStrId id;  // 0x10
    u8 pad_0x18[0x38];
};
static_assert(sizeof(UnkIdObject_df1ff4) == 0x50, "layout");

void fn_7101051530(UnkIdObject_df1ff4* self);

// A hash map by string id whose nodes are also kept in a list.
using UnkIdMap_df1ff4 = base::global::CRntDictionary<CStrId, UnkIdObject_df1ff4*>;

// Erases a node (its key) from the map (generated: src/clones/).
UnkIdMap_df1ff4::TNode* fn_7100df2284(UnkIdMap_df1ff4* map, const CStrId* key);

// The game manager's members this uses (game/UnkGameManager.h leaves them as padding).
struct UnkGameManagerIds_df1ff4 {
    u8 pad_0x0[0x2290];
    UnkIdMap_df1ff4 field_0x2290;
};

void* fn_7100df1ff4(UnkGameManager* manager, const CStrId& id) {
    auto* self = reinterpret_cast<UnkGameManagerIds_df1ff4*>(manager);
    if (UnkIdMap_df1ff4::TNode* node = self->field_0x2290.FindNode(id)) {
        if (node->value)
            node->value->vf_0x10();
        fn_7100df2284(&self->field_0x2290, &node->key);
    }
    auto* object = static_cast<UnkIdObject_df1ff4*>(fn_71000839a0(sizeof(UnkIdObject_df1ff4), 5));
    fn_7101051530(object);
    if (object) {
        object->vf_0x18();
        object->id = id;
        self->field_0x2290.Insert(id, object);
    }
    return object;
}
