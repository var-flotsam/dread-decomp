// CMorphBallLauncherComponent's unit: the base component's setup, caches the launcher class cast of the
// object at +0x10, binds the component's callbacks and registers four handler entries (once each) in the
// registry at +0x208. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkComponent.h"
#include "types.h"

// The handler is defined in src/game/UnkStateMachineSet_b60.cpp.
template <u32 Offset>
struct UnkSmOwner_b60;
bool fn_7100889040(UnkSmOwner_b60<0x208>* self, const s32* message);
extern void fn_7100889160();
extern void fn_7100889178();
extern void fn_710088917c();
extern void fn_710088918c();
extern void fn_7100889190();
extern void fn_71008891f0();
extern void fn_71008891f4();
extern void fn_71008891f8();
extern void fn_710088927c();
extern void fn_7100889280();
extern void fn_71008892f4();

struct UnkMbl_Cb {
    void* owner;
    void* fn;
};

struct UnkMbl_Entry;

struct UnkMbl_Registry {
    u8 pad_0x0[0x8];
    void* owner;
    u8 pad_0x10[0x10];
    u32 count;
    struct __attribute__((packed)) Slot {
        UnkMbl_Entry* entry;
    } slots[1];
};

struct UnkMbl_Entry {
    u8 pad_0x0[0x8];
    UnkMbl_Registry* registry;
    u32 id;
};

struct UnkMbl_Obj {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* field_0x10;
    u8 pad_0x18[0x30];
    UnkMbl_Entry e0;
    u8 pad_0x5c[0x1c];
    UnkMbl_Cb cb0;
    UnkMbl_Cb cb1;
};

struct __attribute__((packed)) UnkMbl_Slot {
    UnkMbl_Entry* entry;
};

static inline __attribute__((always_inline)) void Add(char* self, UnkMbl_Entry* e) {
    UnkMbl_Slot* slots = reinterpret_cast<UnkMbl_Slot*>(self + 0x22c);
    u32 n = *reinterpret_cast<u32*>(self + 0x228);
    UnkMbl_Slot* end = slots + n;
    for (UnkMbl_Slot* p = slots; p != end; ++p)
        if (p->entry->id == e->id)
            return;
    e->registry = reinterpret_cast<UnkMbl_Registry*>(self + 0x208);
    end->entry = e;
    *reinterpret_cast<u32*>(self + 0x228) = n + 1;
}

bool fn_71008866b8(char* self, void* arg) {
    fn_71010c41bc(reinterpret_cast<UnkBaseComponent*>(self), arg);
    auto at = [&](u32 off) { return self + off; };
    auto* link = *reinterpret_cast<base::reflection::UnkReflObject**>(at(0x10));
    base::reflection::UnkReflObject* cast = nullptr;
    if (link && fn_710009d50c(link->vf_0x0(), fn_71008849e4()))
        cast = link;
    *reinterpret_cast<void**>(at(0x308)) = cast;
    auto cb = [&](u32 off, void (*fn)()) {
        *reinterpret_cast<void**>(at(off)) = self;
        *reinterpret_cast<void**>(at(off + 8)) = reinterpret_cast<void*>(fn);
    };
    *reinterpret_cast<void**>(at(0x350)) = self;
    *reinterpret_cast<void**>(at(0x370)) = self;
    *reinterpret_cast<void**>(at(0x358)) = reinterpret_cast<void*>(fn_7100889040);
    *reinterpret_cast<void**>(at(0x378)) = reinterpret_cast<void*>(fn_7100889160);
    auto* reg = reinterpret_cast<UnkMbl_Registry*>(at(0x208));
    cb(0x78, fn_7100889178);
    reg->owner = self;
    cb(0x88, fn_710088917c);
    Add(self, reinterpret_cast<UnkMbl_Entry*>(at(0x48)));
    cb(0xe8, fn_710088918c);
    cb(0xf8, fn_7100889190);
    Add(self, reinterpret_cast<UnkMbl_Entry*>(at(0xb8)));
    cb(0x158, fn_71008891f0);
    cb(0x178, fn_71008891f4);
    cb(0x168, fn_71008891f8);
    Add(self, reinterpret_cast<UnkMbl_Entry*>(at(0x128)));
    cb(0x1c8, fn_710088927c);
    cb(0x1e8, fn_7100889280);
    cb(0x1d8, fn_71008892f4);
    Add(self, reinterpret_cast<UnkMbl_Entry*>(at(0x198)));
    return true;
}
