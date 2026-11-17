// Unit 392 (query system): resets the owner's entries, picks for each the first filter that passes, scores
// the entries no filter took with every evaluator, sorts them by total score and returns the best one's
// subject. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkTU377.h"
#include "game/querysystem/CFilter.h"
#include "types.h"

struct UnkUnit392Filter_b69;
struct UnkShapedEvaluator_A2_b68;
template <class T>
struct UnkFrameVector;

f32 fn_71002b80ec(UnkShapedEvaluator_A2_b68* self, void* a, void* b);
f32* fn_71000d4a50(UnkFrameVector<f32>* v, u32 i);

struct UnkTU392Source {
    u8 pad_0x0[0x10];
    void* subject;  // +0x10
};

struct UnkTU392Entry {
    UnkTU392Source* source;               // +0x0
    UnkUnit392Filter_b69* filter;         // +0x8
    base::global::CRntVector<f32> scores;  // +0x10
    f32 total;                            // +0x38
    u8 pad_0x3c[0x4];

    UnkFrameVector<f32>* Scores() { return reinterpret_cast<UnkFrameVector<f32>*>(&scores); }
};
static_assert(sizeof(UnkTU392Entry) == 0x40, "UnkTU392Entry");

struct UnkUnit392Entry;
void fn_71002c5c00(UnkTU392Entry** first, UnkTU392Entry** last);

struct UnkTU392EntryList {
    UnkTU392Entry* data;
    u32 count;

    UnkTU392Entry* begin() { return data; }
    UnkTU392Entry* end() { return data + count; }
};

struct UnkTU392Rules {
    u8 pad_0x0[0x8];
    UnkUnit392Filter_b69** filters;         // +0x8
    u32 filter_count;                       // +0x10
    u8 pad_0x14[0x30 - 0x14];
    UnkShapedEvaluator_A2_b68** evaluators;  // +0x30
    u32 evaluator_count;                    // +0x38
};

struct UnkTU392Ranker {
    u8 pad_0x0[0x8];
    UnkTU392Rules* rules;                          // +0x8
    void* context;                                 // +0x10
    UnkTU392EntryList entries;                     // +0x18
    u8 pad_0x28[0x40 - 0x28];
    base::global::CRntVector<UnkTU392Entry*> ranked;  // +0x40

    void ClearEntries() {
        for (UnkTU392Entry& e : entries)
            fn_71002b6920(reinterpret_cast<UnkUnit392Entry*>(&e));
        ranked.count = 0;
    }

    UnkUnit392Filter_b69* FindPassing(void* arg) {
        for (UnkUnit392Filter_b69** it = rules->filters; it != rules->filters + rules->filter_count; ++it) {
            UnkUnit392Filter_b69* f = *it;
            if (f && fn_71002bee84(f, context, arg) == 1)
                return f;
        }
        return nullptr;
    }
};

void* fn_71002c5514(UnkTU392Ranker* self) {
    if (!self->context)
        return nullptr;
    self->ClearEntries();
    for (UnkTU392Entry& entry : self->entries) {
        UnkTU392Entry* e = &entry;
        void* subject = e->source ? e->source->subject : nullptr;
        if (UnkUnit392Filter_b69* f = self->FindPassing(subject))
            e->filter = f;
    }
    for (UnkTU392Entry& entry : self->entries) {
        UnkTU392Entry* e = &entry;
        if (e->filter)
            continue;
        f32 total = 0.0f;
        for (u32 i = 0; i < self->rules->evaluator_count; ++i) {
            UnkShapedEvaluator_A2_b68* ev = self->rules->evaluators[i];
            if (ev) {
                f32 score = fn_71002b80ec(ev, self->context, e->source ? e->source->subject : nullptr);
                *fn_71000d4a50(e->Scores(), i) = score;
                total += score;
            }
        }
        e->total = total;
        self->ranked.PushBack(e);
    }
    fn_71002c5c00(self->ranked.begin(), self->ranked.end());
    if (self->ranked.count != 0 && self->ranked[0]->source)
        return self->ranked[0]->source->subject;
    return nullptr;
}
