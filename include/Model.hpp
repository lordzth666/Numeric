#ifndef TOOLCHAIN_NUMERIC_MODEL_HPP
#define TOOLCHAIN_NUMERIC_MODEL_HPP

#include "Model.h"
#include <math.h>

#define _CONSTEXPR true
#define _NOCONSTEXPR false

namespace Numeric {
    namespace Model {

        template<typename T>
        int Model<T>::AddTensor(const std::string &name_p, const T &init_value, bool const_expr, bool is_trainable) {
            if (tensor_names.find(name_p) == tensor_names.end()) {
                Tensor::Tensor<T> *new_tensor = nullptr;
                if (const_expr) {
                    new_tensor = new Tensor::Tensor<T>(name_p, init_value, true);
                } else {
                    new_tensor = new Tensor::Tensor<T>(name_p, init_value);
                }
                tensor_names.emplace(name_p);
                tensors.push_back(new_tensor);
                std::pair<std::string, size_t> name_kv(new_tensor->_name_(), tensors.size() - 1);
                name_map.insert(name_kv);
                if (is_trainable) {
                    trainable_tensors.push_back(new_tensor);
                }

                return 1;
            }
            return 0;
        }


        template<typename T>
        int Model<T>::CreateSession(const std::string &func_name_, const std::string &expr) {
            raw_expr = expr;
            name = func_name_;

            std::string name_copy = utilities::ParseDef(name);
            utilities::ParseArguments(name_copy, var_name_container, func_name);
            var_container_size = var_name_container.size();

            int depth = 0;
            std::stack<std::string> st_operation;
            std::stack<std::string> st_operand;

            std::stack<long> lbracket_pos;
            std::stack<long> invalid_lbracket;
            invalid_lbracket.push(-2);
            lbracket_pos.push(-1);
            bool last_bracket = false;

            // unitary multiplication op
            AddTensor("u_mul_op", 1, _CONSTEXPR, false);
            // unitary multiplication op
            AddTensor("u_add_op", 0, _CONSTEXPR, false);

            size_t i = 0;
            size_t l = expr.size();
            std::string tmp;

            std::vector<long> normal_args;
            long cur_args = 0;

            while (i < l) {
                if ((expr[i] == ')') && (depth == 0)) {
                    std::cerr << ") parsing error at position " << i << ". Invalid function !" << std::endl;
                    return -1;
                }
                if ((expr[i] == ' ') || (expr[i] == '\t')) {
                    if (tmp.empty()) {
                        i++;
                        continue;
                    }
                    if (Ops::is_registered_op(tmp)) {
                        st_operation.push(tmp);
                    } else {
                        st_operand.push(tmp);
                        cur_args++;
                    }

                    while ((expr[i] == ' ') || (expr[i] == '\t')) {
                        i++;
                        if (i == l)
                            break;
                    }
                    i--;
                    tmp = "";

                } else if (expr[i] == ')') {
                    depth--;
                    if (depth < 0) {
                        std::cerr << "parse error!";
                        std::cerr << " Are you missing '(' ?" << std::endl;
                        return -1;
                    }
                    if (lbracket_pos.top() == invalid_lbracket.top()) {
                        lbracket_pos.pop();
                        invalid_lbracket.pop();
                        i++;
                        continue;
                    }
                    lbracket_pos.pop();
                    if (!tmp.empty()) {
                        st_operand.push(tmp);
                        tmp = "";
                        cur_args += 1;
                    }
                    std::string cur_op = st_operation.top();
                    st_operation.pop();

                    if (Ops::is_binary_op(cur_op)) {
                        if (cur_args != 2) {
                            std::cerr << "Parse error at " << cur_op << " at position " << i << std::endl;
                            std::cerr << "Incorrect number of arguments!" << std::endl;
                            return -1;
                        }
                        std::string rhs_var = st_operand.top();
                        st_operand.pop();
                        std::string lhs_var = st_operand.top();
                        st_operand.pop();
                        std::string new_tensor_name = InsertOp(cur_op, lhs_var, rhs_var, false);
                        st_operand.push(new_tensor_name);
                    } else if (Ops::is_unary_op(cur_op)) {
                        if (cur_args != 1) {
                            std::cerr << "Parse error at " << cur_op << " at position " << i << std::endl;
                            std::cerr << "Incorrect number of arguments!" << std::endl;
                            return -1;
                        }
                        std::string lhs_var = st_operand.top();
                        st_operand.pop();
                        std::string new_tensor_name = InsertOp(cur_op, lhs_var, "u_add_op", false);
                        st_operand.push(new_tensor_name);
                    } else {
                        std::cerr << "Error! operator " << cur_op << " not implemented!" << std::endl;
                        return -1;
                    }
                    cur_args = normal_args.back() + 1;
                    normal_args.pop_back();
                } else {
                    if (expr[i] == '(') {
                        normal_args.push_back(cur_args);
                        cur_args = 0;
                        depth++;
                        lbracket_pos.push(i);
                        if (!last_bracket)
                            last_bracket = true;
                        else
                            invalid_lbracket.push(i);
                    } else {
                        tmp.push_back(expr[i]);
                        last_bracket = false;
                    }
                }
                i++;
            }
            if (depth != 0) {
                std::cerr << "Parse error!";
                std::cerr << " Are you missing ')' ?" << std::endl;
                return -1;
            }
            func = dst[dst.size() - 1];

            return 1;
        }

        template<typename T>
        std::string Model<T>::InsertOp(const std::string &op_name,
                                       std::string lhs_name,
                                       std::string rhs_name,
                                       bool rhs_constexpr) {
            std::string new_tensor_name;
            ops.emplace_back(Ops::CreateOp<T>(op_name));
            if (utilities::is_numeric(lhs_name)) {
                T data = (T) std::atof(lhs_name.c_str());
                new_tensor_name = "_Ts_" + std::to_string(tmp_tensor_id++);
                AddTensor(new_tensor_name, data, true, false);
                lhs_name = new_tensor_name;
            } else {
                AddTensor(lhs_name, 0, false, true);
            }
            if (utilities::is_numeric(rhs_name)) {
                T data = (T) std::atof(rhs_name.c_str());
                new_tensor_name = "_Ts_" + std::to_string(tmp_tensor_id++);
                AddTensor(new_tensor_name, data, true, false);
                rhs_name = new_tensor_name;
            } else {
                AddTensor(rhs_name, 0, rhs_constexpr, true);
            }
            size_t tensor_size = tensors.size();
            for (size_t i = 0; i < tensor_size; i++) {
                if (lhs_name == tensors[i]->_name_()) {
                    this->lhs.push_back(tensors[i]);
                }
                if (rhs_name == tensors[i]->_name_()) {
                    this->rhs.push_back(tensors[i]);
                }
            }
            // Creating a new tensor
            new_tensor_name = "_Ts_" + std::to_string(tmp_tensor_id++);
            AddTensor(new_tensor_name, 0, false, false);
            dst.push_back(tensors[tensors.size() - 1]);
            return new_tensor_name;
        }

        template<typename T>
        void Model<T>::PrintGraph() {
            size_t op_size = ops.size();
            for (size_t i = 0; i < op_size; i++) {
                std::cout << lhs[i]->_name_();
                if (lhs[i]->is_const_tensor()) {
                    std::cout << "( (constexpr) " << lhs[i]->eval() << ")";
                }
                std::cout << "\t" << ops[i]->what() << "\t";
                std::cout << rhs[i]->_name_();
                if (rhs[i]->is_const_tensor()) {
                    std::cout << "( (constexpr) " << rhs[i]->eval() << ")";
                }
                std::cout << "\t->\t" << dst[i]->_name_() << std::endl;
            }
        }

        template<typename T>
        void Model<T>::FeedData(std::vector<std::pair<std::string, T>> kvpair_v) {
            clear();
            size_t l = kvpair_v.size();
            for (size_t i = 0; i < l; i++) {
                auto map_iterator = name_map.find(kvpair_v[i].first);
                if (map_iterator != name_map.end()) {
                    auto idx = map_iterator->second;
                    T value = kvpair_v[i].second;
                    tensors[idx]->clear();
                    tensors[idx]->FeedData(value);
                }
            }
        }

        template<typename T>
        int Model<T>::forward() {
            size_t l = ops.size();
            T data;
            for (size_t i = 0; i < l; i++) {
                ops[i]->FeedData(lhs[i]->Fetch(), rhs[i]->Fetch());
                data = ops[i]->forward();
                if (isnan(data)) {
                    std::cerr << "Math error! forward stopped." << std::endl;
                    std::cerr << lhs[i]->_name_() << "\t"
                              << ops[i]->what() << "\t" << rhs[i]->_name_()
                              << "\t->\t" << dst[i]->_name_() << std::endl;
                    return -1;
                }
                dst[i]->FeedData(data);
            }
            return 1;
        }

#define GRADIENT_DESCENT 1
#define GRADIENT_ASCENT -1

        template<typename T>
        void Model<T>::backward(const T &grad_orientation) {
            long tensor_l = tensors.size();
            for (long i = 0; i < tensor_l - 1; i++) {
                tensors[i]->clear_gradient();
            }
            long l = ops.size();
            assert(grad_orientation == GRADIENT_ASCENT || (grad_orientation == GRADIENT_DESCENT));
            dst[l - 1]->FeedGradient(grad_orientation);
            for (long i = l - 1; i >= 0; i--) {
                T parent_grad = dst[i]->FetchGradient();
                std::pair<T, T> grad_pair = ops[i]->backward();
                lhs[i]->AccGradient(parent_grad * grad_pair.first);
                rhs[i]->AccGradient(parent_grad * grad_pair.second);
            }
        }

        template<typename T>
        void Model<T>::apply_gradient() {
            long tsize = trainable_tensors.size();
            for (long i = 0; i < tsize; i++) {
                trainable_tensors[i]->apply_gradient();
            }
        }

        template<typename T>
        T Model<T>::eval() {
            return func->eval();
        }

        template<typename T>
        void Model<T>::clear() {
            for (size_t i = 0; i < tensors.size(); i++)
                tensors[i]->clear();
        }

        template<typename T>
        T Model<T>::_compute_l1_grad_sum() {
            T result = 0;
            for (size_t i = 0; i < trainable_tensors.size(); i++) {
                T tmp = trainable_tensors[i]->_compute_l1_gradient();
                result += tmp;
            }
            return result;
        }

        template<typename T>
        std::vector<std::pair<T, std::string>> Model<T>::FetchTrainableValues() {
            std::vector<std::pair<T, std::string>> result;
            for (size_t i = 0; i < trainable_tensors.size(); i++) {
                result.push_back(
                        std::pair<T, std::string>(trainable_tensors[i]->Fetch(), trainable_tensors[i]->_name_()));
            }
            return result;
        }


    }  // namespace Model
}  // namespace Numeric

#endif
