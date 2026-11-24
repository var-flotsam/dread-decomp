#pragma once

#include "game/sound/CMusicManager/CTunableMusicManager.h"

// The two types the declaration in CTunableMusicManager_Requests_ovb0019.cpp names.
struct UnkNodeList_b0019 {
    char* vtable;
    union {
        u32 count;  // a union member: the original reloads the links after updating it
    };
    UnkNode_0x68* head;
    UnkNode_0x68* tail;
    u32 flags;
    base::IAllocator* allocator;
    void* field_0x30;

    void Destroy(UnkNode_0x68* node) {
        fn_7100fde6c4(node);
        allocator->vf_0x48(node);
    }
};
