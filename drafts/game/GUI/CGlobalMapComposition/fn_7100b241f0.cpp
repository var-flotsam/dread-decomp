// GUI::CGlobalMapComposition's unit (tools/tu_map.py unit 1680): sets up the composition's state machine:
// registers its six states (each once, by id), binds each state's three callbacks to the composition and
// requests state 0. Names and layouts are placeholders.
#include "types.h"

struct UnkGlobalMapComposition;

void fn_7100b2b864();
void fn_7100b2b868(UnkGlobalMapComposition* self);
void fn_7100b2b900();
void fn_7100b2b904();
void fn_7100b2b918();
void fn_7100b2b91c();
void fn_7100b2b920();
void fn_7100b2b924();
void fn_7100b2b928();
void fn_7100b2b92c();
void fn_7100b2b930();
void fn_7100b2b934();
void fn_7100b2b938();
void fn_7100b2b93c(UnkGlobalMapComposition* self);
void fn_7100b2b9d4(char* self);
void fn_7100b2ba84();
void fn_7100b2ba94();
void fn_7100b2ba98();

struct UnkGlobalMapStateMachine;

struct UnkGlobalMapDelegate {
    void* object;
    void (*function)();
};

struct UnkGlobalMapState {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Enter(s32 previous);
    virtual void Leave(s32 next);

    UnkGlobalMapStateMachine* machine;
    u32 id;
    u8 pad_0x14[0x1c];
    UnkGlobalMapDelegate field_0x30;
    UnkGlobalMapDelegate field_0x40;
    UnkGlobalMapDelegate field_0x50;
    u8 pad_0x60[0x10];
};
static_assert(sizeof(UnkGlobalMapState) == 0x70, "UnkGlobalMapState layout");

typedef UnkGlobalMapState* __attribute__((aligned(4))) UnkGlobalMapStatePtr;

struct UnkGlobalMapStateMachine {
    static const s32 kNone = 0x7fffffff;

    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void OnEntered(UnkGlobalMapState* previous, UnkGlobalMapState* next);
    virtual void OnLeft(UnkGlobalMapState* current, UnkGlobalMapState* next);

    void* owner;
    UnkGlobalMapState* current;
    UnkGlobalMapState* previous;
    u32 count;
    UnkGlobalMapStatePtr states[16];
    u32 pending;
    bool busy;

    void Add(UnkGlobalMapState* state) {
        UnkGlobalMapStatePtr* begin = states;
        UnkGlobalMapStatePtr* end = begin + count;
        for (UnkGlobalMapStatePtr* p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }

    // The registered state with this id, or null.
    UnkGlobalMapState* Find(u32 id) {
        UnkGlobalMapStatePtr* end = states + count;
        for (UnkGlobalMapStatePtr* p = states; p != end; ++p)
            if ((*p)->id == id)
                return *p;
        return nullptr;
    }

    // Moves to state `id` (none: leaves the current one) unless a switch is under way.
    void Request(u32 id) {
        pending = id;
        if (busy)
            return;
        if (!current || current->id != id) {
            UnkGlobalMapState* next = Find(id);
            if (next || id == 0) {
                if (current) {
                    current->Leave(id);
                    OnLeft(current, next);
                }
                previous = current;
                current = next;
                if (next) {
                    next->Enter(previous ? previous->id : kNone);
                    OnEntered(previous, next);
                }
            }
        }
        pending = kNone;
    }
};

struct UnkGlobalMapView {
    u8 pad_0x0[0x238];
    UnkGlobalMapStateMachine machine;  // 0x238
    UnkGlobalMapState states[6];  // 0x2e8
};
static_assert(__builtin_offsetof(UnkGlobalMapView, states) == 0x2e8, "layout");

static inline void Bind(UnkGlobalMapView* self, UnkGlobalMapState* state, void (*a)(), void (*b)(), void (*c)()) {
    self->machine.Add(state);
    state->field_0x30.object = self;
    state->field_0x30.function = a;
    state->field_0x50.object = self;
    state->field_0x50.function = b;
    state->field_0x40.object = self;
    state->field_0x40.function = c;
}

void fn_7100b241f0(UnkGlobalMapView* self) {
    self->machine.owner = self;
    Bind(self, &self->states[0], fn_7100b2b864, reinterpret_cast<void (*)()>(fn_7100b2b868), fn_7100b2b900);
    Bind(self, &self->states[1], fn_7100b2b904, fn_7100b2b918, fn_7100b2b91c);
    Bind(self, &self->states[2], fn_7100b2b920, fn_7100b2b924, fn_7100b2b928);
    Bind(self, &self->states[3], fn_7100b2b92c, fn_7100b2b930, fn_7100b2b934);
    Bind(self, &self->states[4], fn_7100b2b938, reinterpret_cast<void (*)()>(fn_7100b2b93c),
         reinterpret_cast<void (*)()>(fn_7100b2b9d4));
    Bind(self, &self->states[5], fn_7100b2ba84, fn_7100b2ba94, fn_7100b2ba98);
    self->machine.Request(0);
}
