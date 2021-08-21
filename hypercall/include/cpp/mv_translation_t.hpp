/// @copyright
/// Copyright (C) 2020 Assured Information Security, Inc.
///
/// @copyright
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// @copyright
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// @copyright
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.

#ifndef MV_TRANSLATION_T_HPP
#define MV_TRANSLATION_T_HPP

#include <bsl/safe_integral.hpp>

namespace hypercall
{
    /// <!-- description -->
    ///   @brief Provides the results of a translation. This can be used for
    ///     both first and second level paging. If first level paging is used,
    ///     vaddr is a GVA, laddr is a GLA, and paddr is a GPA. flags refers
    ///     to how the GLA is mapped to the GPA. If the translation was a GLA
    ///     to a GPA, vaddr is set to 0. If the translation was a GVA to a GLA,
    ///     paddr, and flags are all 0. If second level paging is used, vaddr
    ///     is always 0 (as second level paging does not support segmentation).
    ///     laddr is a GPA and paddr is an SPA. flags refers to how the GPA is
    ///     mapped to the SPA. In all cases, if is_valid is 0, the values of
    ///     the rest of the structure are undefined. Flags do not include
    ///     caching flags.
    ///
    struct mv_translation_t final
    {
        /// @brief stores the translation's virtual address (input)
        bsl::safe_uint64 vaddr;
        /// @brief stores the translation's linear address (input/output)
        bsl::safe_uint64 laddr;
        /// @brief stores the translation's physical address (output)
        bsl::safe_uint64 paddr;
        /// @brief stores the flags associated with the translation (output)
        bsl::safe_uint64 flags;
        /// @brief stores whether or not the translation is valid (output)
        bool is_valid;
    };
}

#endif
