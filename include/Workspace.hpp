
#ifndef TOOLCHAIN_NUMERIC_WORKSPACE_HPP
#define TOOLCHAIN_NUMERIC_WORKSPACE_HPP

#include <cassert>
#include <ctime>
#include <algorithm>

#include "Workspace.h"

#define PRINT_ITERVAL 100000

#define MCRESOLUTION 0.000001

#define INTGSEED 3
#define STOPPING_GRAD_THRESH 1e-8

namespace Numeric {
    namespace Workspace {
        template<typename T>
        int Workspace<T>::ParseCommand(std::string line) {
            //TODO: Add parsing
            if (!utilities::FormatString(line)) {
                std::cerr << "Bad format!" << std::endl;
                return -1;
            }
            std::string command;
            std::string expr;
            utilities::CommandSpliter(line, command, expr);
            if (command == "define") {
                float start_t = clock();
                if (expr == "--help") {
                    std::cout << "Usage: define define ([func] [id] [id] ...) = [expression]" << std::endl;
                    std::cout << "define ([func] [id] [id] ...) = [expression]" << std::endl;
                    std::cout << "Warning will be raised if " << std::endl;
                    std::cout << "define ([func] [id] [id] ...) = [constant]" << std::endl;
                    std::cout << "example: (f x y) = (+ x y)" << std::endl;
                    return 1;
                }
                if (_Define(expr) != 1) {
                    return -1;
                }
                float end_t = clock();
                std::cout << "defined " << expr << std::endl;
                std::cout << (end_t - start_t) / CLOCKS_PER_SEC
                          << " s elasped for defining this function." << std::endl;
            } else if (command == "eval") {
                float start_t = clock();
                if (expr == "--help") {
                    std::cout << "Usage: eval ([func] [expression] [expression] ...)" << std::endl;
                    return 1;
                }
                if (expr[0] != '(') {
                    if (!utilities::is_numeric(expr)) {
                        std::cout << "Invalid expression!" << std::endl;
                        std::cout << "Usage: eval ([func] [expression] [expression] ...)" << std::endl;
                    }
                }
                T result = _Eval_Recurse(expr);
                if (!isnan(result)) {
                    std::cout << result << std::endl;
                    float end_t = clock();
                    std::cout << (end_t - start_t) / CLOCKS_PER_SEC
                              << " s elasped for eval this function." << std::endl;
                }
            } else if (command == "test") {
                if (expr == "--help") {
                    std::cout << "Usage: test [expression1] [expression2]" << std::endl;
                    return 1;
                }
                float start_t = clock();
                if (_Test(expr) == 1) {
                    float end_t = clock();
                    std::cout << (end_t - start_t) / CLOCKS_PER_SEC
                              << " s elasped for testing this function." << std::endl;
                } else {
                    return -1;
                }
            } else if (command == "numint") {
                if (expr == "--help") {
                    std::cout << "Usage: numint [func] [steps] [arg1_start] "
                              << "[arg1_end] [arg2_start] [arg2_end] ... " << std::endl;
                    return 1;
                }
                expr = "( " + expr + " )";
                float start_t = clock();
                T result = _NumInt(expr);
                std::cout << result << std::endl;
                float end_t = clock();
                std::cout << (end_t - start_t) / CLOCKS_PER_SEC
                          << " s elasped for integrating this function." << std::endl;
            } else if (command == "mcint") {
                if (expr == "--help") {
                    std::cout << "Usage: numint [func] [mc points] [arg1_start] "
                              << "[arg1_end] [arg2_start] [arg2_end] ... " << std::endl;
                }
                expr = "( " + expr + " )";
                float start_t = clock();
                T result = _MCInt(expr);
                std::cout << result << std::endl;
                float end_t = clock();
                std::cout << (end_t - start_t) / CLOCKS_PER_SEC
                          << " s elasped for Monte-Carlo integrating this function." << std::endl;
                return -1;
            } else if (command == "max") {
                if (expr == "--help") {
                    std::cout << "Usage: max [func] [steps] [arg1_init] " <<
                              "[arg2_init] ... " << std::endl;
                    std::cout << "gamma and convergence step is pre-defined for best performance."
                              << std::endl;
                    return 1;
                }
                expr = "( " + expr + " )";
                float start_t = clock();
                std::pair<std::vector<std::pair<T, std::string>>, T> result = _Optimize(expr, MAXOPT);
                std::vector<std::pair<T, std::string>> min_args = result.first;
                T min_value = result.second;

                std::cout << "max @ ( ";
                for (size_t i = 0; i < min_args.size(); i++) {
                    std::cout << min_args[i].second << ":" << min_args[i].first << " ";
                }
                std::cout << ") = " << min_value << std::endl;
                float end_t = clock();
                std::cout << (end_t - start_t) / CLOCKS_PER_SEC
                          << " s elasped for maximizing this function." << std::endl;

            } else if (command == "min") {
                if (expr == "--help") {
                    std::cout << "Usage: min [func] [steps] [arg1_init] " <<
                              "[arg2_init] ... " << std::endl;
                    std::cout << "gamma and convergence step is pre-defined for best performance."
                              << std::endl;
                    return 1;
                }
                expr = "( " + expr + " )";
                float start_t = clock();
                std::pair<std::vector<std::pair<T, std::string>>, T> result = _Optimize(expr, MINOPT);
                std::vector<std::pair<T, std::string>> min_args = result.first;
                T min_value = result.second;

                std::cout << "min @ ( ";
                for (size_t i = 0; i < min_args.size(); i++) {
                    std::cout << min_args[i].second << ":" << min_args[i].first << " ";
                }
                std::cout << ") = " << min_value << std::endl;
                float end_t = clock();
                std::cout << (end_t - start_t) / CLOCKS_PER_SEC
                          << " s elasped for minimizing this function." << std::endl;
            } else if (command == "help") {
                std::cout << "Supported command: define, help, eval, test, numint, mcint, min, max" << std::endl;
                std::cout << "Usage: [command] [expression]" << std::endl;
                std::cout << "Use [command] --help to see the details." << std::endl;
                return 1;
            } else {
                std::cerr << "Command not found!" << std::endl;
                std::cout << "Usage: [command] [expression]" << std::endl;
                std::cout << "Supported command: define, help, eval, test, numint, mcint, min, max" << std::endl;
                std::cout << "Use [command] --help to see the details." << std::endl;
                return -1;
            }
            return 1;
        }

        template<typename T>
        T Workspace<T>::_Eval(const std::string &expr) {
            if (expr[0] != '(') {
                return std::atof(expr.c_str());
            }
            std::string def_expr = expr;
            def_expr = utilities::ParseDef(def_expr);
            std::vector<std::string> v_container;
            std::string func_name;
            utilities::ParseArguments(def_expr, v_container, func_name);
            size_t num_models = models.size();
            for (size_t i = 0; i < num_models; i++) {
                if (func_name == models[i]->_func_name_()) {
                    auto cur_model = models[i];

                    std::vector<std::string> model_names = cur_model->_get_container();
                    std::vector<std::pair<std::string, T>> kv_pairs;
                    for (size_t j = 0; j < v_container.size(); j++) {
                        std::pair<std::string, T> cur_pair(model_names[j],
                                                           (T) std::atof(v_container[j].c_str()));
                        kv_pairs.emplace_back(cur_pair);
                    }
                    cur_model->FeedData(kv_pairs);
                    if (cur_model->forward() == 1) {
                        return cur_model->eval();
                    } else {
                        return NAN;
                    }
                }
            }
            return NAN;
        }

        template<typename T>
        T Workspace<T>::_Eval_Recurse(std::string expr) {
            if (expr[0] != '(') {
                return std::atof(expr.c_str());
            }
            std::string def_expr = expr;
            def_expr = utilities::ParseDef(def_expr);
            std::vector<std::string> v_container;
            std::string func_name;
            utilities::ParseArguments(def_expr, v_container, func_name);
            std::string new_expr = "( " + func_name + " ";
            for (size_t i = 0; i < v_container.size(); i++) {
                if (v_container[i][0] == '(') {
                    v_container[i] = std::to_string(_Eval_Recurse(v_container[i]));
                    new_expr += v_container[i] + " ";
                } else {
                    v_container[i] = std::to_string(_Eval(v_container[i]));
                    new_expr += v_container[i] + " ";
                }
            }
            new_expr += ')';

            return _Eval(new_expr);
        }

        template<typename T>
        int Workspace<T>::_Test(const std::string &expr) {
            std::string expr1;
            std::string expr2;
            if (utilities::SplitExpr(expr, expr1, expr2) == 1) {
                T res1 = _Eval_Recurse(expr1);
                T res2 = _Eval_Recurse(expr2);
                if (isnan(res1) || (isnan(res2)))
                    return -1;
                std::cout << "Test " << expr1 << " = " << expr2;
                if (res1 == res2) {
                    std::cout << "Correct" << std::endl;
                } else {
                    std::cout << "Incorrect" << std::endl;
                }
                return 1;

            } else {
                return -1;
            }

        }

        template<typename T>
        T Workspace<T>::_NumInt(const std::string &expr) {
            std::string def_expr = expr;
            def_expr = utilities::ParseDef(def_expr);
            T result = 0;
            std::vector<std::string> v_container;
            std::string func_name;
            size_t num_models = models.size();
            utilities::ParseArguments(def_expr, v_container, func_name);

            for (size_t i = 0; i < num_models; i++) {
                if (func_name == models[i]->_func_name_()) {
                    long op_cnt = models[i]->OpSize();
                    std::vector<T> start_v;
                    std::vector<T> end_v;
                    auto cur_model = models[i];
                    std::vector<std::string> model_names = cur_model->_get_container();
                    size_t var_cnt = models[i]->VarContainerSize();

                    for (size_t j = 0; j < v_container.size(); j++) {
                        if (!utilities::is_numeric(v_container[j])) {
                            std::cerr << "Error! argv[j]: " << v_container[j] << " is not a number!";
                            std::cerr << std::endl;
                            return -1;
                        }
                    }

                    T step = std::atof(v_container[0].c_str());
                    for (size_t j = 0; j < var_cnt; j++) {
                        T number = std::atof(v_container[j * 2 + 1].c_str());
                        start_v.push_back(number);
                        number = std::atof(v_container[j * 2 + 2].c_str());
                        end_v.push_back(number);
                    }


                    T converted_step = std::atof(v_container[0].c_str());
                    long global_step = trunc(converted_step);

                    if (global_step < 0) {
                        std::cerr << "Error! steps cannot be negative." << std::endl;
                        return -1;
                    }
                    std::vector<T> cur_v(start_v);
                    size_t vsize = start_v.size();

                    long vvsize = (long) vsize;
                    long n = vvsize - 1;
                    long step_cnt = 1;

                    float start;
                    float end;

                    start = clock();
                    std::vector<std::pair<std::string, T>> kv_pairs;
                    while (n >= 0) {
                        kv_pairs.clear();
                        for (size_t j = 0; j < vsize; j++) {
                            std::pair<std::string, T> cur_pair(model_names[j],
                                                               (T) cur_v[j]);
                            kv_pairs.emplace_back(cur_pair);
                        }

                        long pfactor;
#ifdef VARIANT_INTEGRAL
                        if (step <= .01) {
                            pfactor = ((long) rand()) % INTGSEED + 1;
                        }
                        else {
                            pfactor = 1;
                        }
#else
                        pfactor = 1;
#endif

                        cur_model->FeedData(kv_pairs);
                        cur_model->forward();
                        result += cur_model->eval() * (T) pfactor;
                        n = vvsize - 1;
                        cur_v[n] += step * pfactor;

                        while (cur_v[n] > end_v[n]) {
                            cur_v[n] = (T) start_v[n];
                            n--;
                            if (n < 0)
                                break;
                            cur_v[n] += (T) step;
                        }

                        global_step += pfactor;

                        if (global_step >= PRINT_ITERVAL * step_cnt) {
                            end = clock();
                            float time_elasped = (end - start) / CLOCKS_PER_SEC;

                            std::cout << "Integral step: " << global_step << ", "
                                      << time_elasped / PRINT_ITERVAL * 1000
                                      << " ms per iteration." << " Throughput: "
                                      << op_cnt * PRINT_ITERVAL / time_elasped / 1000000
                                      << " MOps per second" << std::endl;
                            step_cnt++;
                            start = clock();
                        }
                    }
                    for (size_t ii = 0; ii < vsize; ii++)
                        result *= step;
                    break;
                }

            }
            return result;
        }

        template<typename T>
        T Workspace<T>::_MCInt(const std::string &expr) {
            std::string def_expr = expr;
            def_expr = utilities::ParseDef(def_expr);
            T result = 0;
            std::vector<std::string> v_container;
            std::string func_name;
            size_t num_models = models.size();
            utilities::ParseArguments(def_expr, v_container, func_name);

            for (size_t i = 0; i < num_models; i++) {
                if (func_name == models[i]->_func_name_()) {
                    long op_cnt = models[i]->OpSize();
                    std::vector<T> start_v;
                    std::vector<T> end_v;
                    auto cur_model = models[i];
                    std::vector<std::string> model_names = cur_model->_get_container();
                    size_t var_cnt = models[i]->VarContainerSize();
                    for (size_t j = 0; j < v_container.size(); j++) {
                        if (!utilities::is_numeric(v_container[j])) {
                            std::cerr << "Error! argv[j]: " << v_container[j] << " is not a number!";
                            std::cerr << std::endl;
                            return -1;
                        }
                    }
                    T converted_steps = std::atof(v_container[0].c_str());
                    long global_steps = trunc(converted_steps);

                    if (global_steps < 0) {
                        std::cerr << "Error! steps cannot be negative." << std::endl;
                        return -1;
                    }

                    for (size_t j = 0; j < var_cnt; j++) {
                        T number = std::atof(v_container[j * 2 + 1].c_str());
                        start_v.push_back(number);
                        number = std::atof(v_container[j * 2 + 2].c_str());
                        end_v.push_back(number);
                    }

                    T scale = 1.;
                    for (size_t j = 0; j < var_cnt; j++) {
                        scale *= (end_v[j] - start_v[j]);
                    }

                    float start = clock();
                    float end;

                    srand((unsigned) (time(nullptr)));
                    for (long ii = 1; ii < global_steps + 1; ii++) {
                        std::vector<T> cur_vector;

                        for (size_t j = 0; j < var_cnt; j++) {
                            long scalor = (long) (1. / MCRESOLUTION);
                            T rndp = (std::rand() % scalor) / ((T) scalor);
                            cur_vector.push_back(start_v[j] * rndp + end_v[j] * (1. - rndp));
                        }

                        std::vector<std::pair<std::string, T>> kv_pairs;
                        for (size_t j = 0; j < var_cnt; j++) {
                            std::pair<std::string, T> cur_pair(model_names[j],
                                                               (T) cur_vector[j]);
                            kv_pairs.emplace_back(cur_pair);
                        }

                        cur_model->FeedData(kv_pairs);
                        cur_model->forward();
                        result += cur_model->eval();

                        if (ii % PRINT_ITERVAL == 0) {
                            end = clock();
                            float time_elasped = (end - start) / CLOCKS_PER_SEC;
                            std::cout << "Monte-Carlo step: " << ii << ", "
                                      << time_elasped / PRINT_ITERVAL * 1000
                                      << " ms per iteration." << " Throughput: "
                                      << op_cnt * PRINT_ITERVAL / time_elasped / 1000000
                                      << " MOps per second" << std::endl;
                            start = clock();
                        }
                    }
                    result = result / global_steps * scale;
                    break;
                }
            }
            return result;
        }

        template<typename T>
        int Workspace<T>::_Define(const std::string &expr) {
            size_t l = expr.size();
            size_t pos = expr.find('=');

            if (pos > l) {
                std::cerr << "Error! \'=\' not found!" << std::endl;
                return -1;
            }

            std::string func_t = expr.substr(0, pos - 1);
            if (func_t == "") {
                std::cerr << "Error! Missing function name on the left." << std::endl;
                return -1;
            }

            std::string expr_t = expr.substr(pos + 2, l - pos - 2);
            if (utilities::IsValidExpr(func_t) != 1) {
                std::cerr << "Invalid function name!" << std::endl;
                return -1;
            }
            if (utilities::IsValidExpr(expr_t) != 1) {
                std::cerr << "Invalid expression!" << std::endl;
                return -1;
            }
            if (expr_t[0] != '(') {
                std::cerr << "Warning: setting a function to a constant" << std::endl;
                expr_t = "( + 0 " + expr_t + " )";
            }
            return DefFunction(func_t, expr_t);
        }

        template<typename T>
        int Workspace<T>::DefFunction(const std::string &func_name, const std::string &expr) {
            std::string check_dup_func = utilities::ParseDef(func_name);
            std::vector<std::string> dup_conatiner;
            std::string func_name_dup_check;
            utilities::ParseArguments(check_dup_func, dup_conatiner, func_name_dup_check);
            for (size_t i = 0; i < models.size(); i++) {
                if (func_name_dup_check == models[i]->_func_name_()) {
                    std::cerr << "Error! Duplicate functions!" << std::endl;
                    return -1;
                }
            }

            std::set<std::string> dup_set;
            size_t c_size = dup_conatiner.size();
            for (size_t i = 0; i < c_size; i++) {
                if (utilities::is_numeric(dup_conatiner[i])) {
                    std::cerr << "Error! Numbers in definition!" << std::endl;
                    return -1;
                }
                if (utilities::is_reserved(dup_conatiner[i])) {
                    std::cerr << "Error! variable name" << dup_conatiner[i]
                              << " is reserved!" << std::endl;
                    return -1;
                }
                if (dup_set.find(dup_conatiner[i]) == dup_set.end()) {
                    dup_set.insert(dup_conatiner[i]);
                } else {
                    std::cerr << "Error! Redefined variable " << dup_conatiner[i]
                              << " for current function." << std::endl;
                    return -1;
                }
            }

            std::string check_dup_expr = utilities::ParseDef(expr);
            utilities::ParseArguments(check_dup_expr, dup_conatiner, func_name_dup_check);
            c_size = dup_conatiner.size();
            for (size_t i = 0; i < c_size; i++) {
                if (utilities::CheckUndefined(dup_conatiner[i], dup_set)) {
                    return -1;
                }
            }


            std::string new_expr(expr);
            if (ReplaceExistentFunc(new_expr) != 1) {
                return -1;
            }

            Model::Model<T> *new_model = new Model::Model<T>;
            if (new_model->CreateSession(func_name, new_expr) == 1) {
                std::cout << "Model has " << new_model->size() << " tensors!" << std::endl;
                std::cout << "Model has " << new_model->OpSize() << " operations!" << std::endl;
                models.push_back(new_model);
            } else {
                delete new_model;
                return -1;
            }

            return 1;
        }

        template<typename T>
        int Workspace<T>::ReplaceExistentFunc(std::string &expr) {
            size_t num_models = models.size();
            size_t expr_len;
            size_t pos_t = 0;
            bool has_existent = true;
            while (has_existent) {
                has_existent = false;
                for (size_t id = num_models; id >= 1; id--) {
                    size_t i = id - 1;
                    expr_len = expr.size();
                    std::string swp_func_name = models[i]->_func_name_();
                    std::vector<std::string> &argv_model = models[i]->_get_container();
                    pos_t = expr.find(" " + swp_func_name + " ");

                    std::string expr_substr;
                    std::string expr_func_name;
                    std::string replace_str;
                    std::string finished_str;
                    while (pos_t < expr_len) {
                        has_existent = true;
                        if (pos_t < 1) {
                            std::cerr << "Error! Attempting to parse a function without defining '('. " << std::endl;
                            return -1;
                        } else if (expr[pos_t - 1] != '(') {
                            std::cerr << "Error! Attempting to parse a function without defining '('. " << std::endl;
                        }
                        long depth = 0;
                        size_t j = pos_t + 1;
                        while ((depth != 0) || (expr[j] != ')')) {
                            if (expr[j] == '(') depth++;
                            if (expr[j] == ')') depth--;
                            j++;
                        }
                        size_t parse_start = pos_t - 1;
                        size_t parse_end = j;

                        expr_substr = expr.substr(parse_start, parse_end - parse_start + 1);
                        expr_substr = utilities::ParseDef(expr_substr);
                        std::vector<std::string> argv_expr;

                        utilities::ParseArguments(expr_substr, argv_expr, expr_func_name);
                        assert(expr_func_name == swp_func_name);
                        replace_str = models[i]->_func_();
                        int exit_code = utilities::ReplaceAll(replace_str, argv_model, argv_expr);
                        if (exit_code != 1) {
                            std::cerr << "Parse error at " << swp_func_name << std::endl;
                            std::cerr << "Expected " << argv_model.size() << " parameters:";
                            for (size_t i = 0; i < argv_model.size(); i++) {
                                std::cerr << argv_model[i] << "\t";
                            }
                            std::cerr << std::endl;
                            std::cerr << "But input had: " << argv_expr.size() << " parameters:";
                            for (size_t i = 0; i < argv_expr.size(); i++) {
                                std::cerr << argv_expr[i] << "\t";
                            }
                            std::cerr << std::endl;
                        }
                        expr.replace(parse_start, parse_end - parse_start + 1, replace_str);
                        finished_str += expr.substr(0, parse_start + replace_str.size());
                        expr_len = expr.size();
                        expr = expr.substr(parse_start + replace_str.size(),
                                           expr_len - (parse_start + replace_str.size()));
                        pos_t = expr.find(" " + swp_func_name + " ");
                        expr_len = expr.size();
                    }
                    finished_str += expr;
                    expr = finished_str;
                }
            }
            return 1;
        }

        template<typename T>
        std::pair<std::vector<std::pair<T, std::string>>, T> Workspace<T>::_Optimize(const std::string &expr,
                                                                                     const T &opt_option) {
            std::string def_expr = expr;
            def_expr = utilities::ParseDef(def_expr);
            std::vector<std::string> v_container;
            std::string func_name;
            size_t num_models = models.size();
            utilities::ParseArguments(def_expr, v_container, func_name);
            T result = 0;
            for (size_t i = 0; i < num_models; i++) {
                if (func_name == models[i]->_func_name_()) {
                    std::vector<T> start_v;
                    std::vector<T> end_v;
                    auto cur_model = models[i];
                    long op_cnt = cur_model->OpSize();
                    std::vector<std::string> model_names = cur_model->_get_container();
                    size_t var_cnt = cur_model->VarContainerSize();

                    std::pair<std::vector<std::pair<T, std::string>>, T> null_res;

                    T converted_steps = std::atof(v_container[0].c_str());
                    long max_steps = trunc(converted_steps);
                    if ((double) fabsf(converted_steps - max_steps) > 1e-8) {
                        std::cerr << "Error! steps cannot be floating point!" << std::endl;
                        return null_res;
                    }

                    if (max_steps < 0) {
                        std::cerr << "Error! steps cannot be negative." << std::endl;
                        return null_res;
                    }

                    for (size_t j = 0; j < v_container.size(); j++) {
                        if (!utilities::is_numeric(v_container[j])) {
                            std::cerr << "Error! argv[j]: " << v_container[j] << " is not a number!";
                            std::cerr << std::endl;
                            return null_res;
                        }
                    }

                    // Initialize varaibles
                    std::vector<std::pair<std::string, T>> kv_pairs;
                    for (size_t j = 0; j < var_cnt; j++) {
                        std::pair<std::string, T> cur_pair(model_names[j],
                                                           (T) std::atof(v_container[j + 1].c_str()));
                        kv_pairs.emplace_back(cur_pair);
                    }
                    cur_model->FeedData(kv_pairs);
                    T grad_sum = 0;
                    float start = clock();
                    float end;
                    for (long global_steps = 1; global_steps < max_steps + 1; global_steps++) {
                        cur_model->forward();
                        cur_model->backward(opt_option);
                        cur_model->apply_gradient();
                        grad_sum = cur_model->_compute_l1_grad_sum();
                        if (grad_sum < STOPPING_GRAD_THRESH) {
                            std::cout << "Iteration " << global_steps << " : ";
                            std::cout << "Early stopping! Stops at grad_sum = " << grad_sum << std::endl;
                            result = cur_model->eval();
                            std::vector<std::pair<T, std::string>> args = cur_model->FetchTrainableValues();
                            std::pair<std::vector<std::pair<T, std::string>>, T> return_res(args, result);
                            return return_res;
                        }
                        if (isinf(grad_sum)) {
                            std::cout << "Iteration " << global_steps << " : ";
                            break;
                        }
                        if (global_steps % PRINT_ITERVAL == 0) {
                            end = clock();
                            T cur_value = cur_model->eval();
                            float time_elasped = (end - start) / CLOCKS_PER_SEC;
                            std::cout << " Optimizer step: " << global_steps << ", "
                                      << time_elasped / PRINT_ITERVAL * 1000
                                      << " ms per iteration." << " Throughput: "
                                      << op_cnt * PRINT_ITERVAL / time_elasped / 1000000
                                      << " MOps per second" << std::endl;
                            std::cout << "current function value is " << cur_value << std::endl;
                            std::cout << "Current Grad sum: " << grad_sum << std::endl;
                            start = clock();
                        }
                    }
                    std::cout << "Cannot converge, stopping! grad_sum = " << grad_sum << std::endl;
                    result = cur_model->eval();
                    std::vector<std::pair<T, std::string>> args = cur_model->FetchTrainableValues();
                    std::pair<std::vector<std::pair<T, std::string>>, T> return_res(args, result);
                    return return_res;
                }
            }
            std::vector<std::pair<T, std::string>> null_vec;
            return std::pair<std::vector<std::pair<T, std::string>>, T>(null_vec, 0);
        }
    }
}


#endif //TOOLCHAIN_NUMERIC_WORKSPACE_HPP
