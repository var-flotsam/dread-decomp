// CIngameMenu (unit 1681): fills the menu's state table (0x380) with the three member functions
// of each of its ten states, in state order. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkMenuStates;

// The menu as its state functions see it.
struct UnkMenuStateMachine;
using UnkMenuStateFn = void (UnkMenuStateMachine::*)();

// One state's functions: enter, exit and one more, moved as raw bytes.
struct UnkMenuStateFnSet {
    UnkMenuStateFn enter;
    UnkMenuStateFn exit;
    UnkMenuStateFn field_0x20;

    UnkMenuStateFnSet(UnkMenuStateFn enter, UnkMenuStateFn exit, UnkMenuStateFn field_0x20)
        : enter(enter), exit(exit), field_0x20(field_0x20) {}
    UnkMenuStateFnSet(UnkMenuStateFnSet&& other) { __builtin_memcpy(this, &other, sizeof(*this)); }
};

struct UnkMenuStateMachine {
    u8 unk_0x0[0x380];
    base::global::CRntVector<UnkMenuStateFnSet> states;

    void fn_7100b2ceb0();
    void fn_7100b2ceb4();
    void fn_7100b2ceb8();
    void fn_7100b2cebc();
    void fn_7100b2d070();
    void fn_7100b2d274();
    void fn_7100b2d278();
    void fn_7100b2d3e8();
    void fn_7100b2d530();
    void fn_7100b2d534();
    void fn_7100b2d5c0();
    void fn_7100b2d628();
    void fn_7100b2d62c();
    void fn_7100b2d6d0();
    void fn_7100b2d750();
    void fn_7100b2d754();
    void fn_7100b2d7f8();
    void fn_7100b2d864();
    void fn_7100b2d868();
    void fn_7100b2da60();
    void fn_7100b2dacc();
    void fn_7100b2dad0();
    void fn_7100b2dcac();
    void fn_7100b2de44();
    void fn_7100b2de48();
    void fn_7100b2dfc4();
    void fn_7100b2e204();
    void fn_7100b2e208();
    void fn_7100b2e3b8();
    void fn_7100b2e548();
};

void fn_7100b2f6b0(UnkMenuStates* menu) {
    using M = UnkMenuStateMachine;
    using Fns = UnkMenuStateFnSet;
    M* self = reinterpret_cast<M*>(menu);
    self->states.Clear();
    self->states.EmplaceBack(Fns(&M::fn_7100b2ceb0, &M::fn_7100b2ceb4, &M::fn_7100b2ceb8));
    self->states.EmplaceBack(Fns(&M::fn_7100b2cebc, &M::fn_7100b2d070, &M::fn_7100b2d274));
    self->states.EmplaceBack(Fns(&M::fn_7100b2d278, &M::fn_7100b2d3e8, &M::fn_7100b2d530));
    self->states.EmplaceBack(Fns(&M::fn_7100b2d534, &M::fn_7100b2d5c0, &M::fn_7100b2d628));
    self->states.EmplaceBack(Fns(&M::fn_7100b2d62c, &M::fn_7100b2d6d0, &M::fn_7100b2d750));
    self->states.EmplaceBack(Fns(&M::fn_7100b2d754, &M::fn_7100b2d7f8, &M::fn_7100b2d864));
    self->states.EmplaceBack(Fns(&M::fn_7100b2d868, &M::fn_7100b2da60, &M::fn_7100b2dacc));
    self->states.EmplaceBack(Fns(&M::fn_7100b2dad0, &M::fn_7100b2dcac, &M::fn_7100b2de44));
    self->states.EmplaceBack(Fns(&M::fn_7100b2de48, &M::fn_7100b2dfc4, &M::fn_7100b2e204));
    self->states.EmplaceBack(Fns(&M::fn_7100b2e208, &M::fn_7100b2e3b8, &M::fn_7100b2e548));
}
