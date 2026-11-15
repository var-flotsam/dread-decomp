// nn::atk unit: checks the main object and every object in its list with fn_71012068bc; true when all pass.
// Names and layouts are placeholders.
#include "types.h"

struct UnkAtkChecked;
u32 fn_71012068bc(UnkAtkChecked* obj);

struct UnkAtkCheckNode {
    UnkAtkCheckNode* prev;
    UnkAtkCheckNode* next;
    u8 pad_0x10[8];
    UnkAtkChecked* object;
};

struct UnkAtkCheckOwner {
    UnkAtkChecked* main;
    u8 pad_0x8[8];
    UnkAtkCheckNode* listPrev;
    UnkAtkCheckNode* listNext;
};

bool fn_7101211328(UnkAtkCheckOwner* self) {
    u32 result = 0;
    if (self->main) {
    u32 mainOk = fn_71012068bc(self->main);
    UnkAtkCheckNode* end = reinterpret_cast<UnkAtkCheckNode*>(&self->listPrev);
    u32 allOk = 1;
    for (UnkAtkCheckNode* n = self->listNext; end != n; n = n->next)
        allOk = allOk & fn_71012068bc(n->object);
    result = mainOk & allOk;
    }
    return result & 1;
}
