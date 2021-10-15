/**
 * @copyright
 * Copyright (C) 2021 Assured Information Security, Inc.
 *
 * @copyright
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * @copyright
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * @copyright
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef KVM_CPUID_ENTRY2_HPP
#define KVM_CPUID_ENTRY2_HPP

#include <bsl/array.hpp>
#include <bsl/convert.hpp>
#include <bsl/cstdint.hpp>

namespace shim
{
    /** @brief  see /include/uapi/linux/kvm.h in Linux for more details. */
    constexpr auto KVM_CPUID_FLAG_SIGNIFCANT_INDEX{0x1_u32};
    /** @brief  see /include/uapi/linux/kvm.h in Linux for more details. */
    constexpr auto KVM_CPUID_FLAG_STATEFUL_FUNC{0x10_u32};
    /** @brief  see /include/uapi/linux/kvm.h in Linux for more details. */
    constexpr auto KVM_CPUID_FLAG_STATE_READ_NEXT{0x100_u32};

#pragma pack(push, 1)

    /**
     * @struct kvm_cpuid_entry2
     *
     * <!-- description -->
     *   @brief see /include/uapi/linux/kvm.h in Linux for more details.
     */
    struct kvm_cpuid_entry2 final
    {
        /** @brief the eax value used to obtain the entry */
        bsl::uint32 function;
        /** @brief the ecx value used to obtain the entry (for entries that are affected by ecx) */
        bsl::uint32 index;
        /** @brief an OR of zero or more of the following:
         *      KVM_CPUID_FLAG_SIGNIFCANT_INDEX: if the index field is valid
         *      KVM_CPUID_FLAG_STATEFUL_FUNC: if cpuid for this function
         *      returns different values for successive invocations; there will
         *      be several entries with the same function, all with this flag
         *      set
         *      KVM_CPUID_FLAG_STATE_READ_NEXT: for
         *      KVM_CPUID_FLAG_STATEFUL_FUNC entries, set if this entry if the
         *      first entry to be read by a cpu
         */
        bsl::uint32 flags;
        /** @brief the eax value returned by the cpuid instruction for this function/index combination */
        bsl::uint32 eax;
        /** @brief the ebx value returned by the cpuid instruction for this function/index combination */
        bsl::uint32 ebx;
        /** @brief the ecx value returned by the cpuid instruction for this function/index combination */
        bsl::uint32 ecx;
        /** @brief the edx value returned by the cpuid instruction for this function/index combination */
        bsl::uint32 edx;
        /** @brief padding for alignment */
        bsl::array<bsl::uint32, 3> padding;
    };
}

#pragma pack(pop)

#endif
