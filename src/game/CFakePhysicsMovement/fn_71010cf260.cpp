// Destructors (complete and deleting) of an object with a node list at 0x238: restore the
// vtables, free the nodes and run the base destructor (fn_71010d092c); the deleting one then
// frees the object.
#include "base/global/CRntVector.h"
#include "game/CGooplotAIComponent.h"
#include "types.h"
#include "base/UnkVtableList.h"

struct UnkBase_0x238 {
    void** vtable;
    u8 unk_0x8[0x230];
    ~UnkBase_0x238();  // fn_71010d092c
};

struct UnkNode_0x18 {
    u64 field_0x0;
    u64 field_0x8;
    UnkNode_0x18* next;
};

extern void* g_unk_7101cb0740[];

struct UnkNodeList_0x28 : base::UnkVtableList<UnkNode_0x18> {
    ~UnkNodeList_0x28() {
        vtable = &g_unk_7101a4d8b8[2];
        Clear();
    }
};

struct UnkObject_71010cf260 : UnkBase_0x238 {
    UnkNodeList_0x28 list;

    ~UnkObject_71010cf260() { vtable = &g_unk_7101cb0740[2]; }
};

void fn_71010cf1e8(UnkObject_71010cf260& self) {
    self.~UnkObject_71010cf260();
}

void fn_71010cf260(UnkObject_71010cf260& self) {
    delete &self;
}
