// From unit 2547 of tools/tu_map.py (0x7100ddb4f0-0x7100ddbcec, no reflected class): helpers that walk the
// central-unit list fn_7100772c98 returns (as the darkness events do) and act on each entry's object
// (+0x10), skipping empty entries. Names and layouts are placeholders.
#include "game/CAudioComponent.h"
#include "game/UnkTU1176.h"
#include "types.h"

struct UnkCentralUnitState {
    u8 pad_0x0[0x10];
    s32 kind;
};

struct UnkCentralUnit {
    u8 pad_0x0[0x18];
    void* field_0x18;
    u8 pad_0x20[0x20];
    UnkCentralUnitState* state;  // 0x40
};

struct UnkCentralUnitEntry {
    u8 pad_0x0[0x10];
    UnkCentralUnit* object;
};

struct UnkCentralUnitList {
    UnkCentralUnitEntry** data;
    u32 count;
};

void fn_7100773018(UnkCentralUnit* unit, void* arg);
void fn_7100775918(UnkCentralUnit* unit, void* arg);
void fn_7100773284(UnkCentralUnit* unit, void* a, void* b, void* c, void* d, bool e);

struct UnkUnit2547 {
    u32 state;
};

void fn_7100ddb6f8(UnkUnit2547* self, void* arg) {
    UnkCentralUnitList* list = fn_7100772c98();
    for (UnkCentralUnitEntry** it = list->data, **end = list->data + list->count; it != end; ++it) {
        if (*it && (*it)->object)
            fn_7100773018((*it)->object, arg);
    }
}

UnkCentralUnit* fn_7100ddb768(UnkUnit2547* self) {
    UnkCentralUnitList* list = fn_7100772c98();
    for (UnkCentralUnitEntry** it = list->data, **end = list->data + list->count; it != end; ++it) {
        if (*it && (*it)->object)
            return (*it)->object;
    }
    return nullptr;
}

void fn_7100ddb7bc(UnkUnit2547* self, void* a, void* b, void* c, void* d, bool e) {
    UnkCentralUnitList* list = fn_7100772c98();
    for (UnkCentralUnitEntry** it = list->data, **end = list->data + list->count; it != end; ++it) {
        if (*it && (*it)->object)
            fn_7100773284((*it)->object, a, b, c, d, e);
    }
}

// Whether any unit's state has the given kind.
static inline bool AnyStateOfKind(s32 kind) {
    UnkCentralUnitList* list = fn_7100772c98();
    for (UnkCentralUnitEntry** it = list->data, **end = list->data + list->count; it != end; ++it) {
        if (*it && (*it)->object) {
            if (UnkCentralUnitState* s = (*it)->object->state) {
                if (s->kind == kind)
                    return true;
            }
        }
    }
    return false;
}

bool fn_7100ddb860(UnkUnit2547* self) { return AnyStateOfKind(4); }
bool fn_7100ddb8cc(UnkUnit2547* self) { return AnyStateOfKind(6); }

// Whether every unit has a state of kind 7.
bool fn_7100ddb938(UnkUnit2547* self) {
    UnkCentralUnitList* list = fn_7100772c98();
    for (UnkCentralUnitEntry** it = list->data, **end = list->data + list->count; it != end; ++it) {
        if (*it && (*it)->object) {
            UnkCentralUnitState* s = (*it)->object->state;
            if (!s || s->kind != 7)
                return false;
        }
    }
    return true;
}

void fn_7100ddb9a4(UnkUnit2547* self, void* arg) {
    UnkCentralUnitList* list = fn_7100772c98();
    for (UnkCentralUnitEntry** it = list->data, **end = list->data + list->count; it != end; ++it) {
        if (*it && (*it)->object)
            fn_7100775918((*it)->object, arg);
    }
}

void fn_71007724c4(UnkCentralUnit* unit, f32* value, u32* id);

// The highest value any unit reports (fn_71007724c4) and the id that came with it; both 0 without one.
void fn_7100ddba14(UnkUnit2547* self, f32* best, u32* best_id) {
    *best = 0;
    *best_id = 0;
    UnkCentralUnitList* list = fn_7100772c98();
    for (UnkCentralUnitEntry** it = list->data, **end = list->data + list->count; it != end; ++it) {
        if (*it) {
            if (UnkCentralUnit* unit = (*it)->object) {
                f32 value = 0.0f;
                u32 id = 0;
                fn_71007724c4(unit, &value, &id);
                if (value > *best) {
                    *best = value;
                    *best_id = id;
                }
            }
        }
    }
}

void* fn_7100d78954(void* object);
s32 fn_710010aad0(s32 index);

// Sends (value, 0) with the mask of bit fn_710010aad0(10) to every unit's target (its +0x18, through
// fn_7100d78954).
static inline void SendToTargets(f32 value) {
    UnkCentralUnitList* list = fn_7100772c98();
    for (UnkCentralUnitEntry** it = list->data, **end = list->data + list->count; it != end; ++it) {
        if (*it && (*it)->object && (*it)->object->field_0x18) {
            if (void* target = fn_7100d78954((*it)->object->field_0x18)) {
                alignas(8) u32 mask = 1u << fn_710010aad0(10);
                fn_710071a398(target, &mask, value, 0.0f);
            }
        }
    }
}

void fn_7100ddbabc(UnkUnit2547* self, u32 state, f32 value) {
    self->state = state;
    SendToTargets(value);
}

void fn_7100ddbb7c(UnkUnit2547* self, u32 expected) {
    if (self->state != expected)
        return;
    self->state = 2;
    SendToTargets(1.0f);
    self->state = 2;
}
