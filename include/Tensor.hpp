#ifndef TOOLCHAIN_NUMERIC_TENSOR_HPP
#define TOOLCHAIN_NUMERIC_TENSOR_HPP

#include "Tensor.h"

namespace Numeric {
    namespace Tensor {

        template<typename T>
        T Tensor<T>::eval() {
            if (data_ready) {
                return data;
            } else {
                std::cerr << "Unitialized Tensor: " << _name_() << std::endl;
                std::cerr << "Warning: Tensor value has not been computed." << std::endl;
                return 0;
            }
        }

        template<typename T>
        T Tensor<T>::Fetch() {
            if (data_ready) {
                return data;
            } else {
                std::cerr << "Unitialized Tensor: " << _name_() << std::endl;
                std::cerr << "Warning: Tensor value cannot be fetched." << std::endl;
                return 0;
            }
        }


        template<typename T>
        bool Tensor<T>::is_const_tensor() const {
            return const_expr;
        }

        template<typename T>
        void Tensor<T>::FeedData(const T &data_) {
            if (const_expr) {
                std::cerr << "Error! Attempt to feed a const tensor!" << std::endl;
                exit(EXIT_FAILURE);
            }
            data = data_;
            _data_const_ = data_;
            data_ready = true;
        }

        template<typename T>
        void Tensor<T>::AccGradient(const T &grad_) {
            if (!freeze) {
                gradient += grad_;
                gradient_ready = true;
            }
        }

        template<typename T>
        void Tensor<T>::FeedGradient(const T &grad_) {
            if (!freeze) {
                gradient = grad_;
                gradient_ready = true;
            }
        }

        template<typename T>
        T Tensor<T>::FetchGradient() {
            if (gradient_ready) {
                return gradient;
            } else {
                return 0;
            }
        }

        template<typename T>
        T Tensor<T>::_compute_l1_gradient() {
            return optimizer->_compute_l1_gradient();
        }

        template<typename T>
        void Tensor<T>::apply_gradient() {
            data = optimizer->apply_gradient(data, gradient);
        }

        template<typename T>
        void Tensor<T>::clear() {
            if (!is_const_tensor()) {
                data = (T) 0.;
                gradient = (T) 0.;
                data_ready = false;
                gradient_ready = false;
            }
        }

        template<typename T>
        void Tensor<T>::clear_gradient() {
            gradient = 0;
            gradient_ready = false;
        }


    }  // namespace Tensor
}  // namespace Numeric

#endif
