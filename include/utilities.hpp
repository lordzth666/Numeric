//
// Created by lordzth on 12/1/18.
//

#ifndef TOOLCHAIN_NUMERIC_UTILITIES_HPP
#define TOOLCHAIN_NUMERIC_UTILITIES_HPP

#include <string>
#include <stack>
#include <vector>
#include <set>

namespace Numeric {
    namespace utilities {
        bool is_reserved(const std::string &expr);

        bool is_numeric(const std::string &cexpr);

        int FormatString(std::string &expr);

        int ReplaceAll(std::string &expr,
                       const std::vector<std::string> &src_map,
                       const std::vector<std::string> &dst_map);

        int ParseArguments(const std::string &def_expr,
                           std::vector<std::string> &argv_container,
                           std::string &func_name);

        int CommandSpliter(const std::string &line,
                           std::string &command,
                           std::string &expr);

        void AddProtector(std::string &op_str);

        std::string ParseDef(const std::string &op_str);

        int SplitExpr(const std::string &expr, std::string &expr1, std::string &expr2);

        bool CheckUndefined(std::string dst_expr, const std::set<std::string> &defs);

        int IsValidExpr(const std::string &expr);

    }
}

#endif //TOOLCHAIN_NUMERIC_UTILITIES_HPP
