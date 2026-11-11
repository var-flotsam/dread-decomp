// Unit 2640: move-assigns a two-mode (inline or heap-held) string-id + node-list holder.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkNode {
    u8 pad_0x0[0x38];
    UnkNode* next;
};

struct UnkNodeList {
    u8 pad_0x0[0x48];
    UnkNode* first;
};

struct UnkHolder {
    union {
        struct {
            CStrId id;
            UnkNodeList list;
        } inl;
        struct {
            CStrId* id;
            UnkNodeList* list;
        } heap;
    };
    s32 mode;

    CStrId* Id() { return mode == 2 ? heap.id : &inl.id; }
    UnkNodeList* List() { return mode == 2 ? heap.list : &inl.list; }
};
static_assert(__builtin_offsetof(UnkHolder, mode) == 0x58);

void fn_7101005fcc(void* dest, UnkNode* node, void* key);

#define MOVE_BODY                                                                              {                                                                                              CStrId* dstId = self->Id();                                                                CStrId* srcId = other->Id();                                                               dstId->ReleaseAndTake(*srcId);                                                            UnkNodeList* dst = self->List();                                                           UnkNodeList* src = other->List();                                                          if (dst != src) {                                                                              for (UnkNode* n = src->first; n; n = n->next)                                                  fn_7101005fcc(reinterpret_cast<u8*>(dst) + 8, n, reinterpret_cast<u8*>(n) + 8);         }                                                                                      }

UnkHolder* fn_71010041a0(UnkHolder* self, UnkHolder* other) {
    switch (other->mode) {
    case 1:
        MOVE_BODY
        break;
    case 2:
        MOVE_BODY
        break;
    }
    return self;
}
