#pragma once

// What the check programs read from a SPIR-V module themselves, as the independent reference the
// manifest and the shim are compared with: the descriptor bindings, and the stage's user inputs and
// outputs with their component type. Opcodes and enumerants from the SPIR-V specification
// (Khronos, unified); only what tools/bshdat.py --install lets through is expected (uniform blocks,
// combined image-samplers, arrays of constant length, scalar and vector interface variables).

#include <cstdint>
#include <cstring>
#include <map>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

namespace dread::nvn::check {

// (set, binding) -> (is a uniform block, descriptor count).
using SpirvBindings = std::map<std::pair<std::uint32_t, std::uint32_t>, std::pair<bool, std::uint32_t>>;

enum class ScalarKind { Float, Int, Uint, Other };

struct InterfaceVar {
    std::uint32_t location;
    ScalarKind kind;
    std::uint32_t components;  // 1-4
    std::uint32_t locations;   // >1 for an array or a matrix (columns)
};

struct SpirvScan {
    SpirvBindings bindings;
    std::vector<InterfaceVar> inputs, outputs;  // user variables with a Location; built-ins left out
};

inline bool ScanSpirv(const std::vector<unsigned char>& data, SpirvScan* out) {
    if (data.size() < 20 || data.size() % 4)
        return false;
    std::vector<std::uint32_t> w(data.size() / 4);
    std::memcpy(w.data(), data.data(), data.size());
    if (w[0] != 0x07230203)
        return false;
    std::map<std::uint32_t, std::uint32_t> set, binding, location, constant;
    std::set<std::uint32_t> block_types, sampled_image_types, builtins;
    std::map<std::uint32_t, std::pair<ScalarKind, std::uint32_t>> scalars;    // id -> (kind, 1)
    std::map<std::uint32_t, std::pair<std::uint32_t, std::uint32_t>> vectors;  // id -> (component, n)
    std::map<std::uint32_t, std::pair<std::uint32_t, std::uint32_t>> matrices; // id -> (column, n)
    std::map<std::uint32_t, std::pair<std::uint32_t, std::uint32_t>> arrays;   // id -> (element, length id)
    std::map<std::uint32_t, std::uint32_t> pointers;                           // id -> pointee
    std::vector<std::tuple<std::uint32_t, std::uint32_t, std::uint32_t>> vars;   // (pointer type, id, storage)
    bool entry_point = false, function = false;
    for (std::size_t i = 5; i < w.size();) {
        const std::uint32_t count = w[i] >> 16, op = w[i] & 0xFFFF;
        if (count == 0 || i + count > w.size())
            return false;
        const std::uint32_t* a = &w[i + 1];
        if (op == 15)  // OpEntryPoint
            entry_point = true;
        else if (op == 54)  // OpFunction
            function = true;
        if (op == 71 && count >= 3) {  // OpDecorate target decoration [value]
            if (a[1] == 34 && count >= 4)
                set[a[0]] = a[2];
            else if (a[1] == 33 && count >= 4)
                binding[a[0]] = a[2];
            else if (a[1] == 30 && count >= 4)
                location[a[0]] = a[2];
            else if (a[1] == 11)
                builtins.insert(a[0]);
            else if (a[1] == 2)
                block_types.insert(a[0]);
        } else if (op == 72 && count >= 4 && w[i + 3] == 11) {  // OpMemberDecorate ... BuiltIn: a gl_PerVertex block
            builtins.insert(a[0]);
        } else if (op == 21 && count >= 4) {  // OpTypeInt result width signedness
            scalars[a[0]] = {a[2] ? ScalarKind::Int : ScalarKind::Uint, 1};
        } else if (op == 22 && count >= 3) {  // OpTypeFloat
            scalars[a[0]] = {ScalarKind::Float, 1};
        } else if (op == 20) {  // OpTypeBool
            scalars[a[0]] = {ScalarKind::Other, 1};
        } else if (op == 23 && count >= 4) {  // OpTypeVector result component count
            vectors[a[0]] = {a[1], a[2]};
        } else if (op == 24 && count >= 4) {  // OpTypeMatrix result column count
            matrices[a[0]] = {a[1], a[2]};
        } else if (op == 27 && count >= 2) {  // OpTypeSampledImage
            sampled_image_types.insert(a[0]);
        } else if (op == 28 && count >= 4) {  // OpTypeArray result element length
            arrays[a[0]] = {a[1], a[2]};
        } else if (op == 32 && count >= 4) {  // OpTypePointer result storage type
            pointers[a[0]] = a[2];
        } else if (op == 43 && count == 4) {  // OpConstant type result value (32-bit)
            constant[a[1]] = a[2];
        } else if (op == 59 && count >= 4) {  // OpVariable type result storage
            vars.emplace_back(a[0], a[1], a[2]);
        }
        i += count;
    }
    if (!entry_point || !function)  // a truncated module can end on an instruction boundary
        return false;
    for (const auto& [ptr, id, storage] : vars) {
        std::uint32_t type = pointers.count(ptr) ? pointers[ptr] : 0, n = 1;
        if (arrays.count(type)) {
            n = constant.count(arrays[type].second) ? constant[arrays[type].second] : 0;
            type = arrays[type].first;
        }
        if (storage == 1 || storage == 3) {  // Input, Output
            if (builtins.count(id) || builtins.count(type) || !location.count(id))
                continue;
            std::uint32_t columns = 1;
            if (matrices.count(type)) {
                columns = matrices[type].second;
                type = matrices[type].first;
            }
            InterfaceVar v{location[id], ScalarKind::Other, 1, n * columns};
            if (vectors.count(type)) {
                v.components = vectors[type].second;
                type = vectors[type].first;
            }
            if (scalars.count(type))
                v.kind = scalars[type].first;
            (storage == 1 ? out->inputs : out->outputs).push_back(v);
            continue;
        }
        if (!binding.count(id) && !set.count(id))
            continue;
        const bool block = storage == 2 && block_types.count(type);
        const bool sampler = storage == 0 && sampled_image_types.count(type);
        if ((!block && !sampler) || n == 0)
            return false;
        if (!out->bindings.emplace(std::make_pair(set.count(id) ? set[id] : 0, binding.count(id) ? binding[id] : 0),
                                   std::make_pair(block, n)).second)
            return false;
    }
    return true;
}

}  // namespace dread::nvn::check
