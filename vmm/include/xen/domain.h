//
// Copyright (C) 2019 Assured Information Security, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef MICROV_XEN_DOMAIN_H
#define MICROV_XEN_DOMAIN_H

#include "../ring.h"
#include "../domain/domain.h"
#include "types.h"

#include <page.h>
#include <public/domctl.h>
#include <public/arch-x86/xen.h>
#include <public/arch-x86/hvm/save.h>
#include <public/hvm/save.h>
#include <public/sysctl.h>
#include <public/platform.h>

namespace microv {

constexpr xen_domid_t DOMID_ROOTVM = DOMID_FIRST_RESERVED - 1;

xen_domid_t create_xen_domain(microv_domain *uv_dom);
xen_domain *get_xen_domain(xen_domid_t id) noexcept;
void put_xen_domain(xen_domid_t id) noexcept;
void destroy_xen_domain(xen_domid_t id);

/* syctls */
bool xen_domain_numainfo(xen_vcpu *vcpu, struct xen_sysctl *ctl);
bool xen_domain_cputopoinfo(xen_vcpu *vcpu, struct xen_sysctl *ctl);
bool xen_domain_getinfolist(xen_vcpu *vcpu, struct xen_sysctl *ctl);
bool xen_domain_get_cpu_featureset(xen_vcpu *vcpu, struct xen_sysctl *ctl);
bool xen_domain_sched_id(xen_vcpu *vcpu, struct xen_sysctl *ctl);

/* domctls */
bool xen_domain_createdomain(xen_vcpu *vcpu, struct xen_domctl *ctl);
bool xen_domain_destroydomain(xen_vcpu *vcpu, struct xen_domctl *ctl);
bool xen_domain_unpausedomain(xen_vcpu *vcpu, struct xen_domctl *ctl);
bool xen_domain_pausedomain(xen_vcpu *vcpu, struct xen_domctl *ctl);
bool xen_domain_getdomaininfo(xen_vcpu *vcpu, struct xen_domctl *ctl);
bool xen_domain_gethvmcontext(xen_vcpu *vcpu, struct xen_domctl *ctl);
bool xen_domain_sethvmcontext(xen_vcpu *vcpu, struct xen_domctl *ctl);
bool xen_domain_setvcpuaffinity(xen_vcpu *vcpu, struct xen_domctl *ctl);
bool xen_domain_max_mem(xen_vcpu *vcpu, struct xen_domctl *ctl);
bool xen_domain_set_tsc_info(xen_vcpu *vcpu, struct xen_domctl *ctl);
bool xen_domain_shadow_op(xen_vcpu *vcpu, struct xen_domctl *ctl);
bool xen_domain_getvcpuextstate(xen_vcpu *vcpu, struct xen_domctl *ctl);
bool xen_domain_set_cpuid(xen_vcpu *vcpu, struct xen_domctl *ctl);
bool xen_domain_ioport_perm(xen_vcpu *vcpu, struct xen_domctl *ctl);
bool xen_domain_iomem_perm(xen_vcpu *vcpu, struct xen_domctl *ctl);
bool xen_domain_assign_device(xen_vcpu *vcpu, struct xen_domctl *ctl);

/**
 * xen_domain
 *
 * This is the primary structure that contains information specific
 * to xen domains. There are two paths from which a xen domain can
 * be created:
 *   - domain_op__create_domain hypercall from a bareflank root vcpu
 *   - domctl::createdomain hypercall from a xen guest dom0
 *
 * TODO: add locks to protect readers (e.g. get_info) from updates
 * (e.g. move_cpupool). This isn't strictly necessary until we have
 * multiple domains reading and writing domain data.
 */
class xen_domain {
public:
    xen_domain(microv_domain *domain);
    ~xen_domain();

    void add_root_page(uintptr_t this_gpa,
                       uintptr_t hpa,
                       uint32_t perm,
                       uint32_t mtype);

    int set_timer_mode(uint64_t mode);
    void queue_virq(int virq);
    xen_vcpuid_t add_vcpu(xen_vcpu *xen);
    void get_info(struct xen_domctl_getdomaininfo *info);
    uint64_t runstate_time(int state);
    uint32_t nr_online_vcpus();
    xen_vcpuid_t max_vcpu_id();

    size_t hvc_rx_put(const gsl::span<char> &span);
    size_t hvc_rx_get(const gsl::span<char> &span);
    size_t hvc_tx_put(const gsl::span<char> &span);
    size_t hvc_tx_get(const gsl::span<char> &span);

    void set_upcall_pending(xen_vcpuid_t vcpuid);
    int acquire_gnttab_pages(xen_mem_acquire_resource_t *res,
                             gsl::span<class page *> pages);

    /* Hypercalls from xl create path */
    bool numainfo(xen_vcpu *v, struct xen_sysctl_numainfo *numa);
    bool cputopoinfo(xen_vcpu *v, struct xen_sysctl_cputopoinfo *topo);
    bool setvcpuaffinity(xen_vcpu *v, struct xen_domctl_vcpuaffinity *aff);
    bool gethvmcontext(xen_vcpu *v, struct xen_domctl_hvmcontext *ctx);
    bool sethvmcontext(xen_vcpu *v, struct xen_domctl_hvmcontext *ctx);
    bool set_max_mem(xen_vcpu *v, struct xen_domctl_max_mem *max);
    bool set_tsc_info(xen_vcpu *v, struct xen_domctl_tsc_info *info);
    bool shadow_op(xen_vcpu *v, struct xen_domctl_shadow_op *shadow);
    bool getvcpuextstate(xen_vcpu *v, struct xen_domctl_vcpuextstate *ext);
    bool set_cpuid(xen_vcpu *v, struct xen_domctl_cpuid *cpuid);
    bool ioport_perm(xen_vcpu *v, struct xen_domctl_ioport_permission *perm);
    bool iomem_perm(xen_vcpu *v, struct xen_domctl_iomem_permission *perm);
    bool assign_device(xen_vcpu *v, struct xen_domctl_assign_device *perm);
    bool unpause(xen_vcpu *v);
    bool pause(xen_vcpu *v);

    bool readconsole(xen_vcpu *v, struct xen_sysctl *ctl);
    bool physinfo(xen_vcpu *v, struct xen_sysctl *ctl);
    bool move_cpupool(xen_vcpu *v, struct xen_sysctl *ctl);

    /* TODO: move to memory.cpp */
    bool get_sharing_freed_pages(xen_vcpu *v);
    bool get_sharing_shared_pages(xen_vcpu *v);

    /* Hypercalls from vcpu boot path */
    uint64_t init_shared_info(xen_vcpu *v, uintptr_t shinfo_gpfn);
    void update_wallclock(xen_vcpu *v,
                          const struct xenpf_settime64 *time) noexcept;

    class xen_vcpu *get_xen_vcpu(xen_vcpuid_t id = 0) noexcept;
    void put_xen_vcpu(xen_vcpuid_t id = 0) noexcept;

    size_t constexpr max_nr_vcpus()
    {
        return m_uvv_id.size();
    }

private:
    friend class xen_evtchn;

    /* TODO: fix assumed id = 0 call sites */
    void set_uv_dom_ctx(struct hvm_hw_cpu *cpu);
    void init_from_domctl() noexcept;
    void init_from_uvctl() noexcept;
    void init_from_root() noexcept;

public:
    microv::domain_info *m_uv_info{};
    microv_domain *m_uv_dom{};

    /*
     * Domain-wide upcall vector. Used for each vcpu unless the guest
     * sets a per-vcpu vector with HVMOP_set_evtchn_upcall_vector
     */
    uint32_t m_upcall_vector{};

    /*
     * We use this limit given by Xen as this is the max number of
     * vcpus that a given struct shared_info can store
     */
    std::array<microv_vcpuid, XEN_LEGACY_MAX_VCPUS> m_uvv_id{};
    size_t m_nr_vcpus{};

    xen_domid_t m_id{};
    xen_uuid_t m_uuid{};
    uint32_t m_ssid{}; /* flask id */

    /* Tunables */
    uint32_t m_max_pcpus{};
    uint32_t m_max_vcpus{};
    uint32_t m_max_evtchns{};
    uint32_t m_max_evtchn_port{};
    uint32_t m_max_grant_frames{};
    uint32_t m_max_maptrack_frames{};

    /* Memory */
    uint64_t m_total_ram{};
    uint32_t m_total_pages{}; /* nr pages possessed */
    uint32_t m_max_pages{};   /* max value for total_pages */
    uint32_t m_free_pages{};
    uint32_t m_max_mfn{};
    uint32_t m_shr_pages{};   /* nr shared pages */
    int32_t m_out_pages{};    /* nr claimed-but-not-possessed pages */
    uint32_t m_paged_pages{}; /* nr paged-out pages */

    /* Scheduling */
    xen_cpupoolid_t m_cpupool_id{};
    bool m_returned_new{};

    bool m_has_passthrough_dev{};
    uint32_t m_flags{}; /* DOMINF_ flags, used for {sys,dom}ctls */
    struct xen_arch_domainconfig m_arch_config {};

    /* Console IO */
    std::unique_ptr<microv::ring<HVC_RX_SIZE>> m_hvc_rx_ring{};
    std::unique_ptr<microv::ring<HVC_TX_SIZE>> m_hvc_tx_ring{};

    /* Console Ring Buffer */
    bool m_is_console_eof{false};

    /* Shared info page */
    std::unique_ptr<uint8_t[]> m_shinfo_page{};
    unique_map<uint8_t> m_shinfo_map{};
    struct shared_info *m_shinfo{};
    uintptr_t m_shinfo_gpfn{};

    std::unique_ptr<xen_memory> m_memory{};
    std::unique_ptr<xen_evtchn> m_evtchn{};
    std::unique_ptr<xen_gnttab> m_gnttab{};
    std::unique_ptr<xen_hvm> m_hvm{};

    /* TSC params */
    uint64_t m_tsc_khz;
    uint64_t m_tsc_mul;
    uint64_t m_tsc_shift;
    uint64_t m_timer_mode;

    /* NUMA parms */
    uint32_t m_numa_nodes;

    /* Assigned PCI devices and IO regions */
    struct io_region {
        static constexpr auto pmio_t = 0;
        static constexpr auto mmio_t = 1;
        uint64_t base;
        uint64_t size;
        int type;
    };

    std::list<struct io_region> m_assigned_pmio{};
    std::list<struct io_region> m_assigned_mmio{};
    std::list<uint32_t> m_assigned_devs{};

public:
    xen_domain(xen_domain &&) = delete;
    xen_domain(const xen_domain &) = delete;
    xen_domain &operator=(xen_domain &&) = delete;
    xen_domain &operator=(const xen_domain &) = delete;
};

}

#endif