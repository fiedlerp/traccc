/** TRACCC library, part of the ACTS project (R&D line)
 *
 * (c) 2022-2024 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

#pragma once

// Project include(s).
#include "traccc/definitions/qualifiers.hpp"
#include "traccc/edm/track_state.hpp"

namespace traccc {

/// Type unrolling functor to update the fitting quality
template <typename algebra_t>
struct statistics_updater {

    using scalar_type = detray::dscalar<algebra_t>;

    /// Update track fitting qualities (NDoF and Chi2)
    ///
    /// @param mask_group mask group that contains the mask of the current
    /// surface
    /// @param index mask index of the current surface
    /// @param fit_res fitting information such as NDoF or Chi2
    /// @param trk_state track state of the current surface
    template <typename mask_group_t, typename index_t>
    TRACCC_HOST_DEVICE inline void operator()(
        const mask_group_t& /*mask_group*/, const index_t& /*index*/,
        fitting_result<algebra_t>& fit_res,
        const track_state<algebra_t>& trk_state,
        const bool use_backward_filter) {

        if (!trk_state.is_hole) {

            // Measurement dimension
            const unsigned int D = trk_state.get_measurement().meas_dim;

            // NDoF = NDoF + number of coordinates per measurement
            fit_res.ndf += static_cast<scalar_type>(D);

            // total_chi2 = total_chi2 + chi2
            if (use_backward_filter) {
                fit_res.chi2 += trk_state.backward_chi2();
            } else {
                fit_res.chi2 += trk_state.filtered_chi2();
            }
        }
    }
};

}  // namespace traccc
