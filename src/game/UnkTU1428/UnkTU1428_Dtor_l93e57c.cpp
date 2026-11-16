// CMorphBallMovement's unit (hint only): the movement's destructor body. Drops its counted references,
// releases its string and file-path ids, frees its node list, resource and input handler, then runs the
// base destructor. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CEmmyProtoAIComponent.h"
#include "game/CInputComponent.h"
#include "game/UnkTU2689.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

extern void* g_unk_7101a841a8[];

// A string id of the pool *Pool, released in two steps.
template <UnkStringPool** Pool>
struct UnkBallId_l93e57c {
    UnkStringData* data;

    void Release() {
        if (UnkStringPool* pool = *Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = *Pool;
                }
            }
            data = &pool->empty;
        }
    }
};
using UnkBallStrId_l93e57c = UnkBallId_l93e57c<&g_unk_7101d47108>;
using UnkBallPathId_l93e57c = UnkBallId_l93e57c<&g_unk_7101d47200>;

// A counted reference (count at +0x8, vf_0x10 deletes).
struct UnkBallRefSlot_l93e57c {
    UnkRefObject* ptr;

    void Release() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

// Another counted object: its count is at +0x30 and vf_0x8 deletes it.
struct UnkBallShared_l93e57c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    u8 pad_0x8[0x28];
    s32 ref_count;
};

struct UnkBallNodeAllocator_l93e57c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void* Alloc(u64 size, u64 align, u32 flags);
    virtual void Free(void* p);
};

struct UnkBallNode_l93e57c {
    void* value;
    UnkBallNode_l93e57c* next;
};

// A list of nodes from an allocator.
struct UnkBallNodeList_l93e57c {
    void* vtable;
    u32 count;
    UnkBallNode_l93e57c* head;
    UnkBallNode_l93e57c* tail;
    UnkBallNodeAllocator_l93e57c* allocator;

    void Destroy() {
        vtable = g_unk_7101a46040 + 2;
        for (UnkBallNode_l93e57c* n = head; n;) {
            UnkBallNode_l93e57c* next = n->next;
            allocator->Free(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

// A string id, a file-path id and the resource the path resolves to.
struct UnkBallResource_l93e57c {
    UnkBallStrId_l93e57c id;
    UnkBallPathId_l93e57c path;
    UnkCountedResource* res;
};

struct UnkBallHandlerTarget_l93e57c {
    u8 pad_0x0[0x1d8];
    void* field_0x1d8;
};

struct UnkBallHandlerSource_l93e57c {
    u8 pad_0x0[0x30];
    UnkBallHandlerTarget_l93e57c* field_0x30;
};

struct UnkBallArrayElem_l93e57c {
    u8 data[0x10];
    ~UnkBallArrayElem_l93e57c() {}
};

// The input handler: unregisters itself, then frees its array and its name.
struct UnkBallHandler_l93e57c {
    void* vtable;
    base::global::CRntString name;
    u8 pad_0x38[0x8];
    UnkBallArrayElem_l93e57c* field_0x40;
    UnkBallHandlerSource_l93e57c* field_0x48;

    void Destroy() {
        vtable = g_unk_7101a22858 + 2;
        if (field_0x48 && field_0x48->field_0x30 && field_0x48->field_0x30->field_0x1d8)
            fn_7100872908(field_0x48->field_0x30->field_0x1d8, this);
        field_0x48 = nullptr;
        vtable = g_unk_7101a22878 + 2;
        if (field_0x40) {
            delete[] field_0x40;
            field_0x40 = nullptr;
        }
        fn_710007ecd4(&name);
    }
};

struct UnkBallMovement_l93e57c {
    void* vtable;
    u8 pad_0x8[0x2270];
    void* vtable2;
    u8 pad_0x2280[0x40];
    UnkBallHandler_l93e57c field_0x22c0;
    u8 pad_0x2310[0x60];
    UnkBallResource_l93e57c field_0x2370;
    u8 pad_0x2388[0x58];
    UnkBallNodeList_l93e57c field_0x23e0;
    u8 pad_0x2408[0xc8];
    UnkBallRefSlot_l93e57c field_0x24d0;
    UnkBallRefSlot_l93e57c field_0x24d8;
    u8 pad_0x24e0[0x80];
    UnkBallStrId_l93e57c field_0x2560[6];
    UnkBallPathId_l93e57c field_0x2590;
    UnkBallPathId_l93e57c field_0x2598;
    u8 pad_0x25a0[0x28];
    UnkBallStrId_l93e57c field_0x25c8;
    u8 pad_0x25d0[0x48];
    UnkBallStrId_l93e57c field_0x2618;
    u8 pad_0x2620[0x8];
    UnkBallShared_l93e57c* field_0x2628;
};
static_assert(__builtin_offsetof(UnkBallMovement_l93e57c, field_0x22c0) == 0x22c0, "layout");
static_assert(__builtin_offsetof(UnkBallMovement_l93e57c, field_0x23e0) == 0x23e0, "layout");
static_assert(__builtin_offsetof(UnkBallMovement_l93e57c, field_0x2560) == 0x2560, "layout");
static_assert(__builtin_offsetof(UnkBallMovement_l93e57c, field_0x2628) == 0x2628, "layout");

void fn_710093e57c(UnkBallMovement_l93e57c* self) {
    self->vtable = g_unk_7101a841a8 + 2;
    self->vtable2 = g_unk_7101a841a8 + 135;
    if (UnkBallShared_l93e57c* shared = self->field_0x2628) {
        if (--shared->ref_count == 0)
            shared->vf_0x8();
    }
    self->field_0x2618.Release();
    self->field_0x25c8.Release();
    self->field_0x2598.Release();
    self->field_0x2590.Release();
    for (int i = 6; i-- > 0;)
        self->field_0x2560[i].Release();
    self->field_0x24d8.Release();
    self->field_0x24d0.Release();
    self->field_0x23e0.Destroy();
    UnkBallResource_l93e57c& resource = self->field_0x2370;
    fn_7100027044(reinterpret_cast<UnkStrIdPathResource*>(&resource));
    if (UnkCountedResource* r = resource.res) {
        if (--r->ref_count == 0)
            fn_710001737c(r);
    }
    resource.path.Release();
    resource.id.Release();
    self->field_0x22c0.Destroy();
    fn_71010d7874(self);
}
