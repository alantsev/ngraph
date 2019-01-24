//*****************************************************************************
// Copyright 2017-2019 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//*****************************************************************************

#include "ngraph/op/experimental/quantized_avg_pool.hpp"
#include "ngraph/op/constant.hpp"
#include "ngraph/runtime/cpu/cpu_builder.hpp"
#include "ngraph/runtime/cpu/mkldnn_invoke.hpp"
#include "ngraph/runtime/cpu/mkldnn_utils.hpp"

using namespace std;
using namespace ngraph;

namespace ngraph
{
    namespace runtime
    {
        namespace cpu
        {
            template <>
            void Builder::BUILDER_DECL(ngraph::op::QuantizedAvgPool)
            {
                if (runtime::cpu::mkldnn_utils::use_mkldnn_kernel(node))
                {
                    auto qavg_pool = static_cast<const ngraph::op::QuantizedAvgPool*>(node);

                    auto& functors = external_function->get_functors();

                    auto& arg_tensor = external_function->get_tensor_data(args[0].get_name());
                    auto& out_tensor = external_function->get_tensor_data(out[0].get_name());

                    auto window_shape = qavg_pool->get_window_shape();
                    auto window_movement_strides = qavg_pool->get_window_movement_strides();
                    auto padding_below = qavg_pool->get_padding_below();
                    auto padding_above = qavg_pool->get_padding_above();
                    auto include_padding_in_avg_computation =
                        qavg_pool->get_include_padding_in_avg_computation();

                    auto& mkldnn_emitter = external_function->get_mkldnn_emitter();
                    auto input_desc = mkldnn_utils::get_input_mkldnn_md(node, 0);
                    auto result_desc = mkldnn_utils::get_output_mkldnn_md(node, 0);

                    size_t qavg_pool_index = mkldnn_emitter->primitive_init(3);
                    auto& deps = mkldnn_emitter->get_primitive_deps(qavg_pool_index);

                    auto functor = [&,
                                    include_padding_in_avg_computation,
                                    input_desc,
                                    result_desc,
                                    window_movement_strides,
                                    window_shape,
                                    padding_below,
                                    padding_above,
                                    qavg_pool_index](CPURuntimeContext* ctx,
                                                     CPUExecutionContext* ectx) {
                        if (ctx->first_iteration)
                        {
                            mkldnn_emitter->pooling_forward(
                                (include_padding_in_avg_computation
                                     ? mkldnn::algorithm::pooling_avg_include_padding
                                     : mkldnn::algorithm::pooling_avg_exclude_padding),
                                input_desc,
                                result_desc,
                                window_movement_strides,
                                window_shape,
                                padding_below,
                                padding_above,
                                qavg_pool_index);
                        }
                        cpu::mkldnn_utils::set_memory_ptr(ctx, deps[0], arg_tensor);
                        cpu::mkldnn_utils::set_memory_ptr(ctx, deps[1], out_tensor);
                        cpu::mkldnn_utils::mkldnn_invoke_primitive(ctx, qavg_pool_index);
                    };
                    functors.emplace_back(functor);
                }
                else
                {
                    throw ngraph_error("unsupported parameters for QuantizedAvgPool via DEX");
                }
            }
            REGISTER_OP_BUILDER(QuantizedAvgPool);
        }
    }
}
