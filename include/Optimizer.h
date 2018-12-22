
#ifndef TOOLCHAIN_NUMERIC_OPTIMIZER_H
#define TOOLCHAIN_NUMERIC_OPTIMIZER_H


namespace Numeric {
    namespace Optimizer {
        template<typename T>
        class Optimizer {
        public:
            virtual T apply_gradient(T &t, const T &grad) = 0;

            virtual T _compute_l1_gradient() = 0;

            virtual ~Optimizer() = default;
        };

        template<typename T>
        class GradientDescentOptimizer : public Optimizer<T> {
        private:
            T lr;
            T gradient;
        public:
            explicit GradientDescentOptimizer(const T &lr_) {
                lr = lr_;
            }

            virtual T apply_gradient(T &t, const T &grad);

            virtual T _compute_l1_gradient();
        };

        template<typename T>
        class MomentumOptimizer : public Optimizer<T> {
        private:
            T lr;
            T gradient;
            T velocity;
            T momentum;
        public:
            MomentumOptimizer(const T &lr_, const T &momentum_) {
                lr = lr_;
                momentum = momentum_;
                velocity = 0;
                gradient = 0;
            }

            virtual T apply_gradient(T &t, const T &grad);

            virtual T _compute_l1_gradient();
        };

    }

}

#endif //TOOLCHAIN_NUMERIC_OPTIMIZER_H
