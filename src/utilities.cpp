//
// Created by lordzth on 12/1/18.
//

#include <iostream>

#include "../include/utilities.hpp"

#define IS_EMPTY(ch)    ((ch == ' ') || (ch == '\t'))

namespace Numeric {
    namespace utilities {
        bool is_reserved(const std::string &expr) {
            return ((expr == "sqrt") ||
                    (expr == "ln") ||
                    (expr == "log2") ||
                    (expr == "ln") ||
                    (expr == "+") ||
                    (expr == "-") ||
                    (expr == "*") ||
                    (expr == "/") ||
                    (expr == "pow") ||
                    (expr == "sin") ||
                    (expr == "cos"));
        }

        bool is_numeric(const std::string &cexpr) {
            bool has_decimal = false;
            if (((cexpr[0] >= '0') && (cexpr[0] <= '9')) || (cexpr[0] == '-') || (cexpr[0] == '.')) {
                size_t l = cexpr.size();
                size_t i = 1;
                while (i < l) {
                    if (cexpr[i] == '.') {
                        if (has_decimal) {
                            return false;
                        } else {
                            has_decimal = true;
                        }
                    } else if ((cexpr[i] < '0') || (cexpr[i] > '9'))
                        return false;
                    i++;
                }
                return true;
            } else {
                return false;
            }
        }

        int IsValidExpr(const std::string &expr) {
            size_t expr_l = expr.size();
            size_t i = 0;
            long depth = 0;
            while (i < expr_l) {
                if (expr[i] == '(') depth++;
                if (expr[i] == ')') depth--;
                i++;
            }
            return (depth == 0);
        }

        int FormatString(std::string &expr) {
            size_t l = expr.size();
            size_t i = 0;
            std::string parsed_str;
            while (i < l) {
                if (IS_EMPTY(expr[i])) {
                    if (i != 0) {
                        if (!IS_EMPTY(expr[i - 1]))
                            parsed_str += ' ';
                    }
                } else if (expr[i] == '(') {
                    if (i == 0) {
                        parsed_str += '(';
                    } else {
                        if (!IS_EMPTY(expr[i - 1])) {
                            parsed_str += ' ';
                            parsed_str += '(';
                        } else {
                            parsed_str += '(';
                        }
                    }
                } else if (expr[i] == ')') {
                    if (i != 0) {
                        if ((!IS_EMPTY(expr[i - 1]) && (expr[i - 1] != ')'))) {
                            parsed_str += ' ';
                            parsed_str += ')';
                        } else {
                            parsed_str += ')';
                        }
                    } else {
                        parsed_str += ')';
                    }
                } else if (i == 0)
                    parsed_str += expr[i];
                else if ((expr[i] != ')') && (!IS_EMPTY(expr[i])) && (expr[i - 1] == ')')) {
                    parsed_str += ' ';
                    parsed_str += expr[i];
                } else if (expr[i - 1] == '(') {
                    parsed_str += ' ';
                    parsed_str += expr[i];
                } else {
                    parsed_str += expr[i];
                }
                i++;
            }
            expr = parsed_str;
            return 1;
        }

        // TODO: Using regex to gain performance
        int ReplaceAll(std::string &expr,
                       const std::vector<std::string> &src_map,
                       const std::vector<std::string> &dst_map) {
            // Stage 1: replace $src_map[i] to ##$dst_map[i]##
            std::string src_str;
            std::string dst_str;
            if (src_map.size() != dst_map.size()) {
                std::cerr << "Parse error!";
                return -1;
            }
            size_t expr_l = expr.size();
            size_t l = src_map.size();
            for (size_t i = 0; i < l; i++) {
                src_str = " " + src_map[i] + " ";
                dst_str = dst_map[i];
                AddProtector(dst_str);
                dst_str = " #" + dst_str + "# ";
                size_t pos_t = expr.find(src_str);
                expr_l = expr.size();
                std::string expr_tmp;
                while (pos_t < expr_l) {
                    expr.replace(pos_t, src_str.size(), dst_str);
                    expr_l = expr.size();
                    pos_t = expr.find(src_str);
                }
            }
            // Stage 2: remove '#'
            std::string res_expr;
            size_t i = 0;
            expr_l = expr.size();
            while (i < expr_l) {
                if (expr[i] != '#') {
                    res_expr += expr[i];
                }
                i++;
            }
            expr = res_expr;
            return 1;
        }

        int ParseArguments(const std::string &def_expr,
                           std::vector<std::string> &argv_container,
                           std::string &func_name) {
            // Input in the format of f(a b c)
            size_t i = 0;
            size_t l = def_expr.size();
            argv_container.clear();
            while (def_expr[i] != '(') {
                i++;
                if (i > l) {
                    std::cerr << "Invalid function! Missing '(' " << std::endl;
                    return -1;
                }
            }
            func_name = def_expr.substr(0, i - 1);
            i++;
            std::string tmp;
            long depth = 0;
            while (i < l) {
                if (((def_expr[i] == ' ') || (def_expr[i] == ')')) && (depth == 0)) {
                    if (!tmp.empty()) {
                        argv_container.push_back(tmp);
                        tmp = "";
                    }
                } else {
                    tmp += def_expr[i];
                    if (def_expr[i] == '(')
                        depth++;
                    if (def_expr[i] == ')')
                        depth--;
                }
                i++;
            }
            if (!tmp.empty())
                argv_container.push_back(tmp);
            return 1;
        }

        void AddProtector(std::string &op_str) {
            long depth = 0;
            std::string new_op_str;
            size_t i = 0;
            size_t l = op_str.size();
            while (i < l) {
                if (op_str[i] == '(')
                    depth++;
                if (op_str[i] == ')')
                    depth--;
                if (depth <= 0)
                    new_op_str += op_str[i];
                else {
                    if (IS_EMPTY(op_str[i])) {
                        new_op_str += '#';
                        new_op_str += ' ';
                        new_op_str += '#';
                    } else {
                        new_op_str += op_str[i];
                    }
                }
                i++;
            }
            op_str = new_op_str;
        }

        int CommandSpliter(const std::string &line, std::string &command, std::string &expr) {
            size_t line_l = line.size();
            size_t pos_t = line.find(' ');
            if (pos_t > line_l) {
                command = line.substr(0, line_l);
                return -1;
            }
            command = line.substr(0, pos_t);
            expr = line.substr(pos_t + 1, line_l - pos_t - 1);
            return 0;
        }

        std::string ParseDef(const std::string &op_str) {
            std::string res;
            size_t i = 2;
            while (!IS_EMPTY(op_str[i])) {
                i++;
            }
            res = op_str.substr(i, op_str.size() - i);
            res = op_str.substr(2, i - 2) + " (" + res;
            return res;
        }


        int SplitExpr(const std::string &expr, std::string &expr1, std::string &expr2) {
            size_t l = expr.size();
            if (expr[0] != '(') {
                size_t pos_t = expr.find('(');
                if (pos_t > l) {
                    size_t pos_s = expr.find(' ');
                    if (pos_s >= l - 1) {
                        std::cerr << "Error! missing operand." << std::endl;
                        return -1;
                    } else {
                        expr1 = expr.substr(0, pos_s);
                        expr2 = expr.substr(pos_s + 1, (l - 1 - pos_s));
                        return 1;
                    }
                } else {
                    expr1 = expr.substr(0, pos_t - 1);
                    expr2 = expr.substr(pos_t, l - pos_t);
                    return 1;
                }
            } else {
                long depth = 1;
                size_t i = 1;
                while (depth != 0) {
                    if (expr[i] == '(')
                        depth++;
                    if (expr[i] == ')')
                        depth--;
                    i++;
                }
                expr1 = expr.substr(0, i);
                if (i + 1 > l) {
                    std::cerr << "Error! missing operand." << std::endl;
                    return -1;
                } else {
                    expr2 = expr.substr(i + 1, (l - i - 1));
                    return 1;
                }
            }
        }

        bool CheckUndefined(std::string dst_expr, const std::set<std::string> &defs) {
            if (dst_expr[0] != '(') {
                if (is_numeric(dst_expr))
                    return false;
                if (defs.find(dst_expr) == defs.end()) {
                    std::cerr << "Error! Undefined variable " << dst_expr;
                    std::cerr << std::endl;
                    return true;
                } else {
                    return false;
                }
            }
            std::string parse_def = ParseDef(dst_expr);
            std::vector<std::string> arg_container;
            std::string func_name;
            if (ParseArguments(parse_def, arg_container, func_name) != 1)
                return false;
            for (size_t i = 0; i < arg_container.size(); i++) {
                if (CheckUndefined(arg_container[i], defs))
                    return true;
            }
            return false;
        }

    }
}

