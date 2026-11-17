// UnkTU2524 (tools/tu_map.py unit 2524): the update of a set of entities spread over up to five buckets.
// Only the bucket whose turn it is advances its entities' clocks to the limit; the others accumulate.
// Entities that no longer qualify return to the main list, the fullest bucket hands entities to the
// emptiest, and qualifying entities of the main list join the emptiest bucket. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntVector;

// An entity: the clock (+0x3a8) and the flags the buckets look at.
struct UnkBucketEntity {
    u8 pad_0x0[0x1b4];
    bool field_0x1b4;
    u8 pad_0x1b5[0x30c - 0x1b5];
    u32 field_0x30c;
    u8 pad_0x310[0x312 - 0x310];
    bool field_0x312;
    u8 pad_0x313[0x3a8 - 0x313];
    f32 field_0x3a8;
};

struct UnkBossOwner_b49;
struct UnkFactorEntity;
// As src/game/CBossAIComponent/CBossAIComponent_CanAct_b49.cpp declares it.
bool fn_7100d7b184(UnkBossOwner_b49* owner);
// As src/game/CEntity.cpp defines it: the entity's time factor.
f32 fn_7100d7ce40(UnkFactorEntity* self);
// As src/game/GUI/CModel3D/CModel3D_Entity_ovb0109.cpp declares it.
void fn_7100d7b1c0(void* entity, u32 index, f32 dt);

static inline bool IsActive(UnkBucketEntity* entity) {
    return fn_7100d7b184(reinterpret_cast<UnkBossOwner_b49*>(entity));
}

static inline f32 TimeFactor(UnkBucketEntity* entity) {
    return fn_7100d7ce40(reinterpret_cast<UnkFactorEntity*>(entity));
}

// A bucket of entities.
struct UnkEntityBucket : CRntVector<UnkBucketEntity*> {
    UnkEntityBucket() : CRntVector<UnkBucketEntity*>(0xf) {}
};

// Up to N elements in place, after their count.
template <class T, u32 N>
struct UnkFixedVector {
    u32 count;
    u8 storage[N * sizeof(T)];

    T* begin() { return reinterpret_cast<T*>(storage); }
    T* end() { return begin() + count; }

    // The element at `index`, constructing the ones up to it first.
    T& At(u32 index) {
        u32 n = index + 1;
        if (n > count) {
            for (u32 i = count; i < n; ++i)
                new (&begin()[i]) T();
            count = n;
        }
        return begin()[index];
    }
};

struct UnkEntityBuckets {
    u32 field_0x0;
    f32 limit;  // 0x4
    CRntVector<UnkBucketEntity*> entities;  // 0x8
    UnkFixedVector<UnkEntityBucket, 5> buckets;  // 0x30
    u32 turn;  // 0xfc: the bucket whose entities advance this update
};
static_assert(__builtin_offsetof(UnkEntityBuckets, turn) == 0xfc, "UnkEntityBuckets layout");

// Removes the element at `it` by moving the last one into its place; returns the element now there.
template <class T>
static inline T* EraseUnordered(CRntVector<T>& vector, T* it) {
    u32 index = it - vector.data;
    if (index < vector.count) {
        u32 last = vector.count - 1;
        if (index < last)
            vector.data[index] = vector.data[last];
        vector.count = last;
    }
    return vector.data + index;
}

// Advances the entity's clock by `dt`, up to the limit.
static inline void Advance(UnkEntityBuckets* self, UnkBucketEntity* entity, f32 dt) {
    if (entity->field_0x1b4) {
        f32 time = TimeFactor(entity) * dt + entity->field_0x3a8;
        f32 limit = self->limit;
        fn_7100d7b1c0(entity, 0, limit < time ? limit : time);
    }
}

// Grows or shrinks the array to `n` elements, clearing the new ones.
template <class T>
static inline void Resize(CRntVector<T>& vector, u32 n) {
    u32 old = vector.count;
    if (old < n) {
        T* p = vector.data;
        if (vector.capacity < n) {
            p = static_cast<T*>(vector.allocator->vf_0x40((u64)n * sizeof(T), 8, vector.flags));
            if (vector.data) {
                __builtin_memcpy(p, vector.data, (u64)vector.count * sizeof(T));
                vector.allocator->vf_0x48(vector.data);
            }
            vector.data = p;
            vector.capacity = n;
            old = vector.count;
        }
        __builtin_memset(p + old, 0, (u64)(n - old) * sizeof(T));
    } else if (old == n) {
        return;
    }
    vector.count = n;
}

// The bucket holding the fewest entities (the first of them).
static inline UnkEntityBucket* Emptiest(UnkEntityBuckets* self) {
    UnkEntityBucket* emptiest = self->buckets.begin();
    u32 fewest = emptiest->count;
    for (UnkEntityBucket* bucket = emptiest + 1; bucket != self->buckets.end(); ++bucket) {
        if (bucket->count < fewest) {
            emptiest = bucket;
            fewest = bucket->count;
        }
    }
    return emptiest;
}

void fn_7100d8be38(void* self_, f32 dt) {
    UnkEntityBuckets* self = static_cast<UnkEntityBuckets*>(self_);

    if (self->buckets.count != 0) {
        UnkEntityBucket* emptiest = nullptr;
        UnkEntityBucket* fullest = nullptr;
        s32 fewest = 0x7fffffff;
        s32 most = -1;
        for (u32 i = 0; i != self->buckets.count; ++i) {
            UnkEntityBucket* bucket = &self->buckets.At(i);
            u32 turn = self->turn;
            for (UnkBucketEntity** it = bucket->begin(); bucket->end() != it;) {
                UnkBucketEntity* entity = *it;
                if (IsActive(entity) && entity->field_0x30c != 0 && !entity->field_0x312) {
                    if (i == turn)
                        Advance(self, entity, dt);
                    else
                        entity->field_0x3a8 += TimeFactor(entity) * dt;
                    ++it;
                } else {
                    self->entities.PushBack(entity);
                    it = EraseUnordered(*bucket, it);
                }
            }
            s32 size = bucket->count;
            if (size < fewest) {
                emptiest = bucket;
                fewest = size;
            }
            if (size > most) {
                fullest = bucket;
                most = size;
            }
        }

        if (fullest != emptiest) {
            while (fullest->count > emptiest->count + 1) {
                UnkBucketEntity* last = *(fullest->end() - 1);
                emptiest->PushBack(last);
                Resize(*fullest, fullest->count - 1);
            }
        }
        self->turn = (self->turn + 1) % self->buckets.count;
    }

    for (UnkBucketEntity** it = self->entities.begin(); self->entities.end() != it;) {
        UnkBucketEntity* entity = *it;
        if (self->buckets.count != 0 && IsActive(entity) && entity->field_0x30c >= 6 && !entity->field_0x312) {
            UnkEntityBucket* bucket = Emptiest(self);
            bucket->PushBack(entity);
            entity->field_0x3a8 += TimeFactor(entity) * dt;
            it = EraseUnordered(self->entities, it);
        } else {
            Advance(self, entity, dt);
            ++it;
        }
    }
}
