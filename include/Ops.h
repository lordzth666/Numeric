#ifndef TOOLCHAIN_NUMERIC_OPS_H
#define TOOLCHAIN_NUMERIC_OPS_H

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include <exception>


namespace Numeric {
    namespace Ops {
        template<typename T>
        class BaseOp {
        public:
            virtual void FeedData(const T &lhs_const, const T &rhs_const) = 0;

            virtual T forward() const = 0;

            virtual std::pair<T, T> backward() const = 0;

            virtual std::string what() const = 0;

            virtual ~BaseOp() = default;
        };

        template<typename T>
        class AddOp : public BaseOp<T> {
        private:
            T lhs;
            T rhs;

        public:
            AddOp() : lhs(0), rhs(0) {}

            AddOp(const T &lexpr, const T &rexpr) : lhs(lexpr), rhs(rexpr) {}

            virtual void FeedData(const T &lhs_const, const T &rhs_const) {
                lhs = lhs_const;
                rhs = rhs_const;
            }

            virtual T forward() const;

            virtual std::pair<T, T> backward() const;

            virtual std::string what() const { return "AddOp"; }

            virtual ~AddOp() = default;
        };

        template<typename T>
        class SubOp : public BaseOp<T> {
        private:
            T lhs;
            T rhs;

        public:
            SubOp() : lhs(0), rhs(0) {}

            SubOp(const T &lexpr, const T &rexpr) : lhs(lexpr), rhs(rexpr) {}

            virtual void FeedData(const T &lhs_const, const T &rhs_const) {
                lhs = lhs_const;
                rhs = rhs_const;
            }

            virtual T forward() const;

            virtual std::pair<T, T> backward() const;

            virtual std::string what() const { return "SubOp"; }

            virtual ~SubOp() = default;
        };

        template<typename T>
        class MulOp : public BaseOp<T> {
        private:
            T lhs;
            T rhs;
        public:
            MulOp() : lhs(0), rhs(0) {}

            MulOp(const T &lexpr, const T &rexpr) : lhs(lexpr), rhs(rexpr) {}

            virtual void FeedData(const T &lhs_const, const T &rhs_const) {
                lhs = lhs_const;
                rhs = rhs_const;
            }

            virtual T forward() const;

            virtual std::pair<T, T> backward() const;

            virtual std::string what() const { return "MulOp"; }

            virtual ~MulOp() = default;
        };

        template<typename T>
        class DivOp : public BaseOp<T> {
        private:
            T lhs;
            T rhs;

        public:
            DivOp() : lhs(0), rhs(0) {}

            DivOp(const T &lexpr, const T &rexpr) : lhs(lexpr), rhs(rexpr) {}

            virtual void FeedData(const T &lhs_const, const T &rhs_const) {
                lhs = lhs_const;
                rhs = rhs_const;
            }

            virtual T forward() const;

            virtual std::pair<T, T> backward() const;

            virtual std::string what() const { return "DivOp"; }

            virtual ~DivOp() = default;
        };

        template<typename T>
        class SinOp : public BaseOp<T> {
        private:
            T lhs;
            T rhs;
        public:
            SinOp() : lhs(0), rhs(0) {}

            SinOp(const T &lexpr, const T &rexpr) : lhs(lexpr), rhs(rexpr) {}

            virtual void FeedData(const T &lhs_const, const T &rhs_const) {
                lhs = lhs_const;
                rhs = rhs_const;
            }

            virtual T forward() const;

            virtual std::pair<T, T> backward() const;

            virtual std::string what() const { return "SinOp"; }

            virtual ~SinOp() = default;
        };

        template<typename T>
        class CosOp : public BaseOp<T> {
        private:
            T lhs;
            T rhs;
        public:
            CosOp() : lhs(0), rhs(0) {}

            CosOp(const T &lexpr, const T &rexpr) : lhs(lexpr), rhs(rexpr) {}

            virtual void FeedData(const T &lhs_const, const T &rhs_const) {
                lhs = lhs_const;
                rhs = rhs_const;
            }

            virtual T forward() const;

            virtual std::pair<T, T> backward() const;

            virtual std::string what() const { return "CosOp"; }

            virtual ~CosOp() = default;
        };

        template<typename T>
        class LnOp : public BaseOp<T> {
        private:
            T lhs;
            T rhs;

        public:
            LnOp() : lhs(0), rhs(0) {}

            LnOp(const T &lexpr, const T &rexpr) : lhs(lexpr), rhs(rexpr) {}

            virtual void FeedData(const T &lhs_const, const T &rhs_const) {
                lhs = lhs_const;
                rhs = rhs_const;
            }

            virtual T forward() const;

            virtual std::pair<T, T> backward() const;

            virtual std::string what() const { return "LnOp"; }

            virtual ~LnOp() = default;
        };

        template<typename T>
        class Log2Op : public BaseOp<T> {
        private:
            T lhs;
            T rhs;

        public:
            Log2Op() : lhs(0), rhs(0) {}

            Log2Op(const T &lexpr, const T &rexpr) : lhs(lexpr), rhs(rexpr) {}

            virtual void FeedData(const T &lhs_const, const T &rhs_const) {
                lhs = lhs_const;
                rhs = rhs_const;
            }

            virtual T forward() const;

            virtual std::pair<T, T> backward() const;

            virtual std::string what() const { return "Log2Op"; }

            virtual ~Log2Op() = default;
        };

        template<typename T>
        class SqrtOp : public BaseOp<T> {
        private:
            T lhs;
            T rhs;

        public:
            SqrtOp() : lhs(0), rhs(0) {}

            SqrtOp(const T &lexpr, const T &rexpr) : lhs(lexpr), rhs(rexpr) {}

            virtual void FeedData(const T &lhs_const, const T &rhs_const) {
                lhs = lhs_const;
                rhs = rhs_const;
            }

            virtual T forward() const;

            virtual std::pair<T, T> backward() const;

            virtual std::string what() const { return "Log2Op"; }

            virtual ~SqrtOp() = default;
        };

        template<typename T>
        class PowOp : public BaseOp<T> {
        private:
            T lhs;
            T rhs;

        public:
            PowOp() : lhs(0), rhs(0) {}

            PowOp(const T &lexpr, const T &rexpr) : lhs(lexpr), rhs(rexpr) {}

            virtual void FeedData(const T &lhs_const, const T &rhs_const) {
                lhs = lhs_const;
                rhs = rhs_const;
            }

            virtual T forward() const;

            virtual std::pair<T, T> backward() const;

            virtual std::string what() const { return "Log2Op"; }

            virtual ~PowOp() = default;
        };


        template<typename T>
        BaseOp<T> *CreateOp(const std::string &op_name);

        bool is_registered_op(const std::string &op_str);

        bool is_unary_op(const std::string &op_str);

        bool is_binary_op(const std::string &op_str);

    }  // namespace Ops
}  // namespace Numeric
#endif


