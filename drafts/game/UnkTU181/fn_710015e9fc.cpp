// Unit 181 (no reliable name): a sampler description's descriptor id. Descriptions are cached by a
// 64-bit key packed from their modes; a new one builds its NVN sampler and registers it in the
// sampler pool under the next free id. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

// A sampler description (0x68 bytes), also the cached value.
struct UnkSamplerDesc {
    u64 field_0x0;
    u32 field_0x8;
    s32 min_filter;    // 0xc
    s32 mag_filter;    // 0x10
    s32 wrap_s;        // 0x14
    s32 wrap_t;        // 0x18
    s32 wrap_r;        // 0x1c
    s32 compare;       // 0x20
    u32 border;        // 0x24
    f32 lod_bias;      // 0x28
    f32 lod_min;       // 0x2c
    f32 lod_max;       // 0x30
    f32 anisotropy;    // 0x34
    s32 nvn_wrap[3];   // 0x38
    f32 border_color[4];  // 0x44
    u32 id;            // 0x54
    u64 key;           // 0x58
    bool registered;   // 0x60
    u8 pad_0x61[0x7];
};
static_assert(sizeof(UnkSamplerDesc) == 0x68, "UnkSamplerDesc layout");

// The cache: values and keys in two arrays that share a count, a capacity and a growth step.
struct UnkSamplerCache {
    UnkSamplerDesc* values;
    u64* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkSamplerDesc* Find(u64 key) {
        for (u32 i = 0; i < count; i++) {
            if (keys[i] == key)
                return &values[i];
        }
        return &values[count];
    }

    void Grow(u32 new_capacity) {
        UnkSamplerDesc* new_values = static_cast<UnkSamplerDesc*>(
            allocator->vf_0x40((u64)new_capacity * sizeof(UnkSamplerDesc), 8, flags));
        u64* new_keys = static_cast<u64*>(allocator->vf_0x40((u64)new_capacity << 3, 8, flags));
        if (values) {
            UnkSamplerDesc* dst = new_values;
            UnkSamplerDesc* src = values;
            for (u32 n = count; n != 0; n--)
                *dst++ = *src++;
            allocator->vf_0x48(values);
        }
        if (keys) {
            __builtin_memcpy(new_keys, keys, (u64)count << 3);
            allocator->vf_0x48(keys);
        }
        values = new_values;
        keys = new_keys;
        capacity = new_capacity;
    }

    void Add(const u64& key, const UnkSamplerDesc& value) {
        if (count == capacity) {
            u32 new_capacity = count + step;
            if (count < new_capacity)
                Grow(new_capacity);
        }
        keys[count] = key;
        values[count] = value;
        count++;
    }
};

extern UnkSamplerCache g_unk_716e2a9708;
extern u32 g_unk_716e2a76e0;   // the next sampler id
extern u32 g_unk_716e2a76dc;   // the highest sampler id given out
extern u8 g_unk_716e2a76b8[];  // the sampler pool

void* fn_7100153de0();

// NVN entry points.
extern void (*g_unk_716e29b388)(void* builder);
extern void (*g_unk_716e29b390)(void* builder, s32 min, s32 mag);
extern void (*g_unk_716e29b398)(void* builder, s32 s, s32 t, s32 r);
extern void (*g_unk_716e29b3a0)(void* builder, f32 min, f32 max);
extern void (*g_unk_716e29b3a8)(void* builder, f32 bias);
extern void (*g_unk_716e29b3b8)(void* builder, const f32* color);
extern void (*g_unk_716e29b3d0)(void* builder, f32 anisotropy);
extern void (*g_unk_716e29b3d8)(void* builder, s32 reduction);
extern void (*g_unk_716e29b3b0)(void* builder, s32 mode, s32 func);
extern void (*g_unk_716e29b448)(void* sampler, void* builder);
extern void (*g_unk_716e29b008)(void* pool, u32 id, void* sampler);
extern void (*g_unk_716e29b450)(void* sampler);

static inline u64 SamplerKey(const UnkSamplerDesc* d) {
    return (u64)(d->min_filter & 7) | ((u64)(d->mag_filter & 1) << 3) |
           ((u64)(d->wrap_s & 3) << 4) | ((u64)(d->wrap_t & 3) << 6) |
           ((u64)(d->wrap_r & 3) << 8) | ((u64)(d->compare & 7) << 10) |
           ((u64)d->border << 13) | (((u64)(d->lod_bias + 15.0f) & 31) << 45) |
           (((u64)(d->lod_min + 15.0f) & 31) << 50) | (((u64)(d->lod_max + 15.0f) & 31) << 55) |
           ((u64)d->anisotropy << 60);
}

u32 fn_710015e9fc(UnkSamplerDesc* desc) {
    u64 key = SamplerKey(desc);
    desc->registered = false;
    desc->key = key;

    UnkSamplerCache& cache = g_unk_716e2a9708;
    UnkSamplerDesc* found = cache.Find(key);
    if (found != &cache.values[cache.count])
        return found->id;

    void* builder = fn_7100153de0();
    g_unk_716e29b388(builder);
    g_unk_716e29b390(builder, desc->min_filter, desc->mag_filter);
    g_unk_716e29b398(builder, desc->nvn_wrap[0], desc->nvn_wrap[1], desc->nvn_wrap[2]);
    g_unk_716e29b3a0(builder, desc->lod_min, desc->lod_max);
    g_unk_716e29b3a8(builder, desc->lod_bias);
    g_unk_716e29b3b8(builder, desc->border_color);
    g_unk_716e29b3d0(builder, desc->anisotropy);
    g_unk_716e29b3d8(builder, 0);
    if (desc->compare != 1) {  // 1: no depth compare
        s32 func;
        switch (desc->compare) {
        case 7: func = 7; break;
        case 6: func = 5; break;
        case 5: func = 4; break;
        case 4: func = 2; break;
        case 3: func = 6; break;
        case 2: func = 3; break;
        default: func = 8; break;
        }
        g_unk_716e29b3b0(builder, 1, func);
    }

    u8 sampler[0x60];
    g_unk_716e29b448(sampler, builder);
    u32 id = g_unk_716e2a76e0;
    desc->id = id;
    g_unk_716e29b008(g_unk_716e2a76b8, id, sampler);
    g_unk_716e29b450(sampler);

    cache.Add(desc->key, *desc);
    g_unk_716e2a76e0++;
    if (g_unk_716e2a76dc < desc->id)
        g_unk_716e2a76dc = desc->id;
    return desc->id;
}
