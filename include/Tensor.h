#ifndef TOOLCHAIN_NUMERIC_TENSOR_H
#define TOOLCHAIN_NUMERIC_TENSOR_H

#include <iostream>
#include <string>

#define LR  0.01

#include "Optimizer.hpp"

namespace Numeric {
    namespace Tensor {
        template<typename T>
        class Tensor {
        private:
            T data;
            T _data_const_;
            T gradient;
            bool freeze;
            bool gradient_ready;
            bool data_ready;
            bool const_expr;
            std::string name;
            Optimizer::Optimizer<T> *optimizer;

        public:
            Tensor(const std::string &name_, const T &data_) :
                    data(data_),
                    _data_const_(0),
                    gradient(0),
                    freeze(false),
                    gradient_ready(false),
                    data_ready(false),
                    const_expr(false),
                    name(name_),
                    optimizer(new Optimizer::MomentumOptimizer<T>(LR, .9)) {}

            Tensor(const std::string &name_, const T &data_, bool const_expr_) :
                    data(data_),
                    _data_const_(0),
                    gradient(0),
                    freeze(true),
                    gradient_ready(false),
                    data_ready(const_expr_),
                    const_expr(const_expr_),
                    name(name_),
                    optimizer(new Optimizer::MomentumOptimizer<T>(LR, .9)) {}

            void FeedData(const T &data_);

            void AccGradient(const T &grad_);

            void FeedGradient(const T &grad_);

            T FetchGradient();

            void apply_gradient();

            T _compute_l1_gradient();

            ~Tensor() = default;

            std::string _name_() { return name; };

            T Fetch();

            T eval();

            bool is_const_tensor() const;

            void clear();

            void clear_gradient();
        };
    }  // namespace Tensor
}  // namespace Numeric
#endif
