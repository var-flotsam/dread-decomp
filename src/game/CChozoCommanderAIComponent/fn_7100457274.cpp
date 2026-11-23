// Unit 665 (CChozoCommanderAIComponent's neighbours): calls a bound handler with a copy of a message
// (a word, a string id and a list of string ids) passed by value. Names and layouts are placeholders.
#include <new>

#include "base/UnkPooledStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// A growable list of string ids (CRntVector's layout) whose growth moves the ids over, releasing the
// moved-from ones in two steps.
struct UnkIdList_l457274 {
    CStrId* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkIdList_l457274(const UnkIdList_l457274& other)
        : data(nullptr), count(0), capacity(0), step(other.step), field_0x14(other.field_0x14),
          flags(other.flags), allocator(other.allocator) {
        Reserve(other.capacity);
        if (other.count != 0) {
            const CStrId* end = other.data + other.count;
            for (const CStrId* p = other.data; p != end; ++p)
                PushBack(*p);
        }
    }
    ~UnkIdList_l457274() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].Reset();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }

    void Reserve(u32 n) {
        if (capacity < n) {
            CStrId* p = static_cast<CStrId*>(allocator->vf_0x40((u64)n * sizeof(CStrId), 8, flags));
            if (CStrId* old = data) {
                u64 j = 0;
                for (u32 i = 0; i < count; ++i, ++j, ++old) {
                    CStrId::MoveConstructReloading(&p[j], *old);
                    old->Reset();
                }
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
    }
    void PushBack(const CStrId& id) {
        u32 new_capacity = capacity;
        u32 n = count;
        if (new_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        Reserve(new_capacity);
        new (&data[count]) CStrId(id);
        ++count;
    }
};

struct UnkMsg_l457274 {
    u32 field_0x0;
    base::UnkPooledStrId<g_unk_7101d47108> id;  // 0x8
    UnkIdList_l457274 ids;                      // 0x10
};
static_assert(sizeof(UnkMsg_l457274) == 0x38, "UnkMsg_l457274 layout");

struct UnkMsgHandler_l457274 {
    void* context;
    u32 (*fn)(void* context, UnkMsg_l457274 msg);
};

u32 fn_7100457274(UnkMsgHandler_l457274* handler, const UnkMsg_l457274& msg) {
    return handler->fn(handler->context, msg);
}
