// CEvacuationCountDown unit (tools/tu_map.py unit 1246). fn_71007d9e34: a destructor of a component built
// on the activatable component: the vtable, the array of 0x40-byte polymorphic entries at +0xc8 (each
// destroyed through its vtable, then the array freed), the keyed ids at +0x88 (fn_71000df388), the
// counted reference at +0x80 dropped, then the activatable component's destructor (fn_71007d70d8,
// CDemolitionBlockComponent_Dtor_b44.cpp). Names and layouts are placeholders.
#include "base/UnkKeyedStrIds.h"
#include "base/UnkVector.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a72ba8[];

template <u32 IdOffset>
struct UnkActivatable_b44a2;
void fn_71007d70d8(UnkActivatable_b44a2<0x48>* self);

struct UnkCountDownEntry_b49a3 {
    virtual ~UnkCountDownEntry_b49a3();
    u8 pad_0x8[0x38];
};

struct UnkEvacuationCountDown_b49a3 {
    void** vtable;
    u8 pad_0x8[0x78];
    UnkRefObject* ref;  // 0x80
    UnkKeyedStrIds ids;  // 0x88
    u8 pad_0xb0[0x18];
    base::UnkVector<UnkCountDownEntry_b49a3> entries;  // 0xc8
};

void fn_71007d9e34(UnkEvacuationCountDown_b49a3* self) {
    self->vtable = g_unk_7101a72ba8 + 2;
    self->entries.Free();
    fn_71000df388(&self->ids);
    if (UnkRefObject* p = self->ref) {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
    fn_71007d70d8(reinterpret_cast<UnkActivatable_b44a2<0x48>*>(self));
}
