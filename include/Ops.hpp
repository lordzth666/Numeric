#ifndef TOOLCHAIN_NUMERIC_OPS_HPP
#define TOOLCHAIN_NUMERIC_OPS_HPP

#include "Ops.h"

namespace Numeric {
    namespace Exceptions {
        class MathError {
        public:
            const char *what() const throw() {
                return "Math error!\n";
            }
        };
    }

    namespace Ops {
        template<typename T>
        T _tabs_(const T &rhs) {
            if (rhs > (T) 0)
                return rhs;
            else
                return -rhs;
        }

        template<typename T>
        T AddOp<T>::forward() const {
            return lhs + rhs;
        }

        template<typename T>
        std::pair<T, T> AddOp<T>::backward() const {
            std::pair<T, T> grad(1, 1);
            return grad;
        }

        template<typename T>
        T SubOp<T>::forward() const {
            return lhs - rhs;
        }

        template<typename T>
        std::pair<T, T> SubOp<T>::backward() const {
            std::pair<T, T> grad(1, -1);
            return grad;
        }

        template<typename T>
        T MulOp<T>::forward() const {
            return lhs * rhs;
        }

        template<typename T>
        std::pair<T, T> MulOp<T>::backward() const {
            std::pair<T, T> grad(rhs, lhs);
            return grad;
        }

        template<typename T>
        T DivOp<T>::forward() const {
            return lhs / rhs;
        }

        template<typename T>
        std::pair<T, T> DivOp<T>::backward() const {
            std::pair<T, T> grad(1 / rhs, -lhs / rhs / rhs);
            return grad;
        }

        template<typename T>
        T SinOp<T>::forward() const {
            return sin(lhs);
        }

        template<typename T>
        std::pair<T, T> SinOp<T>::backward() const {
            std::pair<T, T> grad(cos(lhs), 0);
            return grad;
        }

        template<typename T>
        T CosOp<T>::forward() const {
            return cos(lhs);
        }

        template<typename T>
        std::pair<T, T> CosOp<T>::backward() const {
            std::pair<T, T> grad(-sin(lhs), 0);
            return grad;
        }

        template<typename T>
        T LnOp<T>::forward() const {
            return log(lhs);
        }

        template<typename T>
        std::pair<T, T> LnOp<T>::backward() const {
            std::pair<T, T> grad(1. / lhs, 0);
            return grad;
        }

        template<typename T>
        T Log2Op<T>::forward() const {
            return log(lhs) / log(2);
        }

        template<typename T>
        std::pair<T, T> Log2Op<T>::backward() const {
            std::pair<T, T> grad(1. / lhs / log(2), 0);
            return grad;
        }

        template<typename T>
        T SqrtOp<T>::forward() const {
            return sqrt(lhs);
        }

        template<typename T>
        std::pair<T, T> SqrtOp<T>::backward() const {
            std::pair<T, T> grad(1. / 2 / sqrt(lhs), 0);
            return grad;
        }

        template<typename T>
        T PowOp<T>::forward() const {
            return pow(lhs, rhs);
        }

        template<typename T>
        std::pair<T, T> PowOp<T>::backward() const {
            std::pair<T, T> grad(rhs * pow(lhs, rhs - 1), pow(lhs, rhs) * log(rhs));
            return grad;
        }


        template<typename T>
        BaseOp<T> *CreateOp(const std::string &op_name) {
            if (op_name == "+") {
                BaseOp<T> *op = new AddOp<T>();
                return op;
            }
            if (op_name == "-") {
                BaseOp<T> *op = new SubOp<T>();
                return op;
            }
            if (op_name == "*") {
                BaseOp<T> *op = new MulOp<T>();
                return op;
            }
            if (op_name == "/") {
                BaseOp<T> *op = new DivOp<T>();
                return op;
            }
            if (op_name == "sin") {
                BaseOp<T> *op = new SinOp<T>();
                return op;
            }
            if (op_name == "cos") {
                BaseOp<T> *op = new CosOp<T>();
                return op;
            }
            if (op_name == "log2") {
                BaseOp<T> *op = new Log2Op<T>();
                return op;
            }
            if (op_name == "ln") {
                BaseOp<T> *op = new LnOp<T>();
                return op;
            }
            if (op_name == "pow") {
                BaseOp<T> *op = new PowOp<T>();
                return op;
            }


            return nullptr;
        }

        bool is_registered_op(const std::string &op_str) {
            return ((op_str == "+") || (op_str == "-") || (op_str == "*")
                    || (op_str == "/") || (op_str == "sin") || (op_str == "cos") ||
                    (op_str == "ln") || (op_str == "log2") || (op_str == "sqrt") || (op_str == "pow"));
        }

        bool is_unary_op(const std::string &op_str) {
            return (op_str == "sin") || (op_str == "cos") ||
                   (op_str == "ln") || (op_str == "log2")
                   || (op_str == "sqrt");
        }

        bool is_binary_op(const std::string &op_str) {
            return ((op_str == "+") || (op_str == "-") || (op_str == "*")
                    || (op_str == "/") || (op_str == "pow"));
        }

    }  // namespace Ops
}  // namespace Numeric
#endif
