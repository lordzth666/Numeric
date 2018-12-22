#ifndef TOOLCHAIN_NUMERIC_WORKSPACE_H
#define TOOLCHAIN_NUMERIC_WORKSPACE_H

#include "Model.hpp"


#define MAXOPT  -1
#define MINOPT  1

namespace Numeric {
    namespace Workspace {
        template<typename T>
        class Workspace {
        private:
            std::vector<Model::Model<T> *> models;
        public:
            int DefFunction(const std::string &func_name, const std::string &expr);

            int ParseCommand(std::string line);

            int ReplaceExistentFunc(std::string &expr);

            int _Define(const std::string &expr);

            T _Eval(const std::string &expr);

            T _Eval_Recurse(std::string expr);


            int _Test(const std::string &expr);

            T _NumInt(const std::string &expr);

            T _MCInt(const std::string &expr);

            std::pair<std::vector<std::pair<T, std::string>>, T>
            _Optimize(const std::string &expr, const T &opt_option);


        };
    }
}

#endif //TOOLCHAIN_NUMERIC_WORKSPACE_H
