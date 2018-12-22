#ifndef TOOLCHAIN_NUMERIC_MODEL_H
#define TOOLCHAIN_NUMERIC_MODEL_H

#include <queue>
#include <set>
#include <stack>
#include <vector>
#include <map>

#include "Ops.hpp"
#include "Tensor.hpp"
#include "utilities.hpp"

namespace Numeric {
    namespace Model {
        template<typename T>
        class Model {
        private:
            std::vector<Ops::BaseOp<T> *> ops;
            std::vector<Tensor::Tensor<T> *> lhs;
            std::vector<Tensor::Tensor<T> *> rhs;
            std::vector<Tensor::Tensor<T> *> dst;
            std::vector<Tensor::Tensor<T> *> tensors;
            std::set<std::string> tensor_names;

            std::map<std::string, size_t> name_map;
            std::vector<Tensor::Tensor<T> *> trainable_tensors;

            Tensor::Tensor<T> *func;

            std::string raw_expr;       // raw expression
            std::string name;           // raw function definition

            std::vector<std::string> var_name_container; // arg list
            size_t var_container_size;
            std::string func_name;      // function name

            long tmp_tensor_id;

            T l1_gradient_sum;
            T l2_gradient_sum;

        public:
            Model() : func(nullptr),
                      tmp_tensor_id(0),
                      l1_gradient_sum(0),
                      l2_gradient_sum(0) {}

            ~Model() {
                while (ops.size() != 0) {
                    auto free_ptr = ops.back();
                    ops.pop_back();
                    delete free_ptr;
                }
                while (tensors.size() != 0) {
                    auto free_ptr = tensors.back();
                    tensors.pop_back();
                    delete free_ptr;
                }
            }

            size_t size() {
                return tensors.size();
            }

            std::vector<std::string> &_get_container() {
                return var_name_container;
            }

            size_t VarContainerSize() {
                return var_container_size;
            }

            size_t OpSize() {
                return ops.size();
            }

            int CreateSession(const std::string &func_name, const std::string &expr);

            int AddTensor(const std::string &name_p, const T &init_value, bool const_expr, bool is_trainable);

            std::string InsertOp(const std::string &op_name,
                                 std::string lhs_name,
                                 std::string rhs_name,
                                 bool rhs_constexpr);

            std::string _name_() { return name; }

            std::string _func_name_() { return func_name; }

            std::string _func_() { return raw_expr; }

            void FeedData(std::vector<std::pair<std::string, T>> kvpair_v);

            int forward();

            void backward(const T &grad_orientation);

            void apply_gradient();

            void clear();

            void clear_gradient();

            void PrintGraph();

            T _compute_l1_grad_sum();

            T _compute_l2_grad_sum();

            std::vector<std::pair<T, std::string>> FetchTrainableValues();


            T eval();

        };
    }  // namespace Model
}  // namespace Numeric

#endif
