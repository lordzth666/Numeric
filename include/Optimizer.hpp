
#ifndef TOOLCHAIN_NUMERIC_OPTIMIZER_HPP
#define TOOLCHAIN_NUMERIC_OPTIMIZER_HPP

#include "Optimizer.h"
#include "Ops.hpp"

namespace Numeric {
    namespace Optimizer {
        template<typename T>
        T GradientDescentOptimizer<T>::apply_gradient(T &t, const T &grad) {
            gradient = grad;
            t -= lr * gradient;
            return t;
        }

        template<typename T>
        T GradientDescentOptimizer<T>::_compute_l1_gradient() {
            return (T) Ops::_tabs_<T>(gradient);
        }

        template<typename T>
        T MomentumOptimizer<T>::apply_gradient(T &t, const T &grad) {
            gradient = grad;
            velocity = velocity * momentum + lr * gradient;
            gradient = velocity;
            t -= velocity;
            return t;
        }

        template<typename T>
        T MomentumOptimizer<T>::_compute_l1_gradient() {
            return (T) Ops::_tabs_<T>(gradient / lr);
        }


    }
}


#endif //TOOLCHAIN_NUMERIC_OPTIMIZER_HPP
