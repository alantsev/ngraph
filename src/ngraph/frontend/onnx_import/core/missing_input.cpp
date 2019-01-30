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

#include "missing_input.hpp"
#include "ngraph/node.hpp"
#include "ngraph/node_vector.hpp"

namespace ngraph
{
    namespace onnx_import
    {
        missing_input::missing_input(const std::shared_ptr<Node>& arg)
            : ngraph::Node("Missing input", {}, 0)
        {
            m_name = "";
        }

        void missing_input::validate_and_infer_types()
        {
        }

        std::shared_ptr<Node> missing_input::copy_with_new_args(const NodeVector& new_args) const
        {
            check_new_args_count(this, new_args);
            return std::make_shared<missing_input>(new_args.at(0));
        }
    } // namespace onnx_import
} // namespace ngraph