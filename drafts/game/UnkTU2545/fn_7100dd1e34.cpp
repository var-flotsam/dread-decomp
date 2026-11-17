// Unit 2545 of tools/tu_map.py: the boss rush result screen's constructor. It sets up its state machine
// (eight states, each with its enter, update and leave callbacks), enters state 1, and names its five time
// rows' description and value texts. Names and layouts are placeholders.
#include <stdio.h>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// The row constructor (src/game/UnkUnit2545/UnkUnit2545_IdPairCtor_b49.cpp).
struct UnkUnit2545IdPair {
    u64 field_0x0;
    u64 field_0x8;
    CStrId id_0x10;
    CStrId id_0x18;
};
void fn_7100dd26a0(UnkUnit2545IdPair* self);

struct UnkBossResultScreen;
struct UnkBossResultState;

struct UnkBossResultStateVtbl {
    void* slot_0x0;
    void* slot_0x8;
    void (*Enter)(UnkBossResultState* self, u32 previous);  // 0x10
    void (*Leave)(UnkBossResultState* self, u32 next);      // 0x18
};

struct UnkBossResultScreenVtbl {
    void* slot_0x0;
    void* slot_0x8;
    void* slot_0x10;
    void* slot_0x18;
    void (*OnEntered)(UnkBossResultScreen* self, UnkBossResultState* previous, UnkBossResultState* next);  // 0x20
    void (*OnLeaving)(UnkBossResultScreen* self, UnkBossResultState* current, UnkBossResultState* next);   // 0x28
};

// A callback bound to its object.
struct UnkBossResultCallback {
    void* object;
    void* function;

    template <class T, class F>
    void Bind(T* obj, F* fn) {
        object = obj;
        function = reinterpret_cast<void*>(fn);
    }
};

struct UnkBossResultState {
    const UnkBossResultStateVtbl* vtable;
    UnkBossResultScreen* owner;
    u32 id;
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    u32 field_0x2c;
    UnkBossResultCallback enter;   // 0x30
    UnkBossResultCallback leave;   // 0x40
    UnkBossResultCallback update;  // 0x50
    UnkBossResultCallback field_0x60;
};
static_assert(sizeof(UnkBossResultState) == 0x70, "size");

struct UnkBossResultQuad {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};

struct UnkBossResultRow {
    UnkUnit2545IdPair ids;  // value text at +0x10, description text at +0x18
    u32 field_0x20;
};

struct UnkBossResultScreen {
    const UnkBossResultScreenVtbl* vtable;
    UnkBossResultScreen* owner;
    UnkBossResultState* current;
    UnkBossResultState* previous;
    u32 count;
    u8 storage[16 * sizeof(UnkBossResultState*)];  // 0x24
    u32 pending;
    bool busy;
    bool field_0xa9;
    u8 pad_0xaa[6];
    f32 time;  // 0xb0
    f32 field_0xb4;
    u32 field_0xb8;
    u8 field_0xbc;
    u8 field_0xbd;
    u64 field_0xc0;
    u64 field_0xc8;
    u64 field_0xd0;
    bool field_0xd8;
    f32 field_0xdc;
    bool field_0xe0;
    UnkBossResultQuad field_0xe4[3];
    f32 field_0x114[3];
    u64 field_0x120[15];
    UnkBossResultRow rows[5];  // 0x198
    u32 row_count;
    u32 field_0x264;
    bool field_0x268;
    bool field_0x269;
    UnkBossResultState states[8];  // 0x270

    UnkBossResultState** items() { return reinterpret_cast<UnkBossResultState**>(storage); }

    UnkBossResultState* Find(u32 id) {
        UnkBossResultState** begin = items();
        UnkBossResultState** end = begin + count;
        for (UnkBossResultState** it = begin; it != end; ++it) {
            if ((*it)->id == id)
                return *it;
        }
        return nullptr;
    }

    void AddState(UnkBossResultState* state) {
        UnkBossResultState** begin = items();
        UnkBossResultState** end = begin + count;
        for (UnkBossResultState** it = begin; it != end; ++it) {
            if ((*it)->id == state->id)
                return;
        }
        state->owner = this;
        *end = state;
        count++;
    }

    void ChangeState(u32 id) {
        pending = id;
        if (busy)
            return;
        if (!current || current->id != id) {
            UnkBossResultState* next = Find(id);
            if (next) {
                if (current) {
                    current->vtable->Leave(current, id);
                    vtable->OnLeaving(this, current, next);
                }
                previous = current;
                current = next;
                next->vtable->Enter(next, previous ? previous->id : 9);
                vtable->OnEntered(this, previous, next);
            }
        }
        pending = 9;
    }
};
static_assert(__builtin_offsetof(UnkBossResultScreen, time) == 0xb0, "layout");
static_assert(__builtin_offsetof(UnkBossResultScreen, field_0xe4) == 0xe4, "layout");
static_assert(__builtin_offsetof(UnkBossResultScreen, rows) == 0x198, "layout");
static_assert(__builtin_offsetof(UnkBossResultScreen, states) == 0x270, "layout");
static_assert(sizeof(UnkBossResultScreen) == 0x5f0, "size");

extern char g_unk_7101aa9800[];
extern char g_unk_7101aa9750[];

struct UnkUnit2545_A6_b72;
void fn_7100dd8df4(UnkBossResultScreen* self);
void fn_7100dd8dfc(UnkBossResultScreen* self, f32 dt);
void fn_7100dd8e00(UnkBossResultScreen* self);
void fn_7100dd8e04(UnkBossResultScreen* self);
void fn_7100dd8e0c(UnkBossResultScreen* self, f32 dt);
void fn_7100dd8e1c(UnkBossResultScreen* self);
void fn_7100dd8e20(UnkBossResultScreen* self);
void fn_7100dd8e24(UnkBossResultScreen* self, f32 dt);
void fn_7100dd8e28(UnkBossResultScreen* self);
void fn_7100dd8e2c(UnkBossResultScreen* self);
void fn_7100dd8e30(UnkBossResultScreen* self, f32 dt);
void fn_7100dd8e34(UnkUnit2545_A6_b72* self);
void fn_7100dd8eb8(UnkBossResultScreen* self);
void fn_7100dd8ec0(UnkBossResultScreen* self, f32 dt);
void fn_7100dd8ec4(UnkBossResultScreen* self);
void fn_7100dd8ec8(UnkBossResultScreen* self);
void fn_7100dd8ecc(UnkBossResultScreen* self, f32 dt);
void fn_7100dd8ed0(UnkBossResultScreen* self);
void fn_7100dd8ed4(UnkBossResultScreen* self);
void fn_7100dd8ed8(UnkBossResultScreen* self, f32 dt);
void fn_7100dd8edc(UnkBossResultScreen* self);
void fn_7100dd8f18(UnkBossResultScreen* self);
void fn_7100dd8f4c(UnkBossResultScreen* self, f32 dt);
void fn_7100dd8f50(UnkBossResultScreen* self);

static inline void InitState(UnkBossResultState* state, u32 id) {
    state->vtable = reinterpret_cast<const UnkBossResultStateVtbl*>(g_unk_7101aa9750 + 0x10);
    state->owner = nullptr;
    state->id = id;
    state->field_0x18 = 0;
    state->field_0x20 = 0;
    state->field_0x28 = 2;
    state->field_0x2c = 1;
    state->enter = UnkBossResultCallback();
    state->leave = UnkBossResultCallback();
    state->update = UnkBossResultCallback();
    state->field_0x60 = UnkBossResultCallback();
}

template <class Enter, class Update, class Leave>
static inline void AddState(UnkBossResultScreen* self, UnkBossResultState* state, Enter* enter, Update* update,
                            Leave* leave) {
    self->AddState(state);
    state->enter.Bind(self, enter);
    state->update.Bind(self, update);
    state->leave.Bind(self, leave);
}

void fn_7100dd1e34(UnkBossResultScreen* self) {
    self->vtable = reinterpret_cast<const UnkBossResultScreenVtbl*>(g_unk_7101aa9800 + 0x10);
    self->owner = nullptr;
    self->current = nullptr;
    self->previous = nullptr;
    self->count = 0;
    self->pending = 9;
    self->busy = false;
    self->field_0xa9 = false;
    self->time = 0.0f;
    self->field_0xb4 = 0.0f;
    self->field_0xb8 = 0;
    self->field_0xbc = 0;
    self->field_0xbd = 0;
    self->field_0xc0 = 0;
    self->field_0xc8 = 0;
    self->field_0xd0 = 0;
    self->field_0xd8 = false;
    self->field_0xdc = -1.0f;
    self->field_0xe0 = false;
    for (s32 i = 0; i < 3; i++) {
        UnkBossResultQuad& quad = self->field_0xe4[i];
        quad.x = 0.0f;
        quad.y = 0.0f;
        quad.z = 0.0f;
        quad.w = 0.0f;
        self->field_0x114[i] = -1.0f;
    }
    for (u64& v : self->field_0x120)
        v = 0;
    for (UnkBossResultRow& row : self->rows)
        fn_7100dd26a0(&row.ids);
    self->row_count = 0;
    self->field_0x264 = 0;
    self->field_0x268 = false;
    self->field_0x269 = false;
    InitState(&self->states[0], 1);
    InitState(&self->states[1], 2);
    InitState(&self->states[2], 3);
    InitState(&self->states[3], 4);
    InitState(&self->states[4], 5);
    InitState(&self->states[5], 6);
    InitState(&self->states[6], 8);
    InitState(&self->states[7], 7);

    self->owner = self;
    AddState(self, &self->states[6], fn_7100dd8df4, fn_7100dd8dfc, fn_7100dd8e00);
    AddState(self, &self->states[0], fn_7100dd8e04, fn_7100dd8e0c, fn_7100dd8e1c);
    AddState(self, &self->states[1], fn_7100dd8e20, fn_7100dd8e24, fn_7100dd8e28);
    AddState(self, &self->states[2], fn_7100dd8e2c, fn_7100dd8e30, fn_7100dd8e34);
    AddState(self, &self->states[3], fn_7100dd8eb8, fn_7100dd8ec0, fn_7100dd8ec4);
    AddState(self, &self->states[4], fn_7100dd8ec8, fn_7100dd8ecc, fn_7100dd8ed0);
    AddState(self, &self->states[5], fn_7100dd8ed4, fn_7100dd8ed8, fn_7100dd8edc);
    AddState(self, &self->states[7], fn_7100dd8f18, fn_7100dd8f4c, fn_7100dd8f50);
    self->ChangeState(1);

    for (s32 i = 0; i < 5; i++) {
        UnkBossResultRow& row = self->rows[i];
        char text[0x80];
        sprintf(text, "BossRush_BossResultScreen_IncTimeDesc_%d", i);
        {
            CStrId desc(text, true);
            row.ids.id_0x18.ReleaseAndTake(desc);
        }
        sprintf(text, "BossRush_BossResultScreen_IncTimeValue_%d", i);
        {
            CStrId value(text, true);
            row.ids.id_0x10.ReleaseAndTake(value);
        }
        row.field_0x20 = 0;
    }
}
