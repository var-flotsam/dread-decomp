// GUI::CMapMenuController's unit (tools/tu_map.py, the name from reflection): two handlers that query
// the object under +0x10 of +0xf0 (fn_7100a8f34c, kind 0) into a pair of string ids (built empty by
// fn_7100a8e07c, src/game_s7/fn_7100a8e07c.cpp; released when done), then refresh the controller
// (vf_0x20). fn_7100b477c4 reports the event handled. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU1600.h"
#include "types.h"

using base::global::CStrId;

// The pair (as fn_7100a8e07c.cpp has it): its constructor out of line; the second id is released in
// two steps (CStrId::Reset), then the first by its destructor.
struct UnkIdPair_7100a8e07c;

struct UnkIdPair_b49 {
    union {
        CStrId first;
    };
    union {
        CStrId second;
    };

    UnkIdPair_b49() {
        __builtin_memset(this, 0, sizeof(*this));
        fn_7100a8e07c(reinterpret_cast<UnkIdPair_7100a8e07c*>(this));
    }
    ~UnkIdPair_b49() {
        second.Reset();
        first.~CStrId();
    }
};

struct UnkMapMenuSource {
    u8 pad_0x0[0x10];
    void* object;
};

struct UnkMapMenuControllerQ {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();  // refreshes
    u8 pad_0x8[0xe8];
    UnkMapMenuSource* source;  // 0xf0
};

__attribute__((always_inline)) static inline void Query(UnkMapMenuControllerQ* self) {
    void* object = self->source ? self->source->object : nullptr;
    UnkIdPair_b49 pair;
    fn_7100a8f34c(object, 0, &pair);
}

bool fn_7100b477c4(UnkMapMenuControllerQ* self) {
    Query(self);
    self->vf_0x20();
    return true;
}

void fn_7100b47190(UnkMapMenuControllerQ* self) {
    Query(self);
    self->vf_0x20();
}
