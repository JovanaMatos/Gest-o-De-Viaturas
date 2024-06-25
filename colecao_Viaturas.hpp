#ifndef __COLECAO_VIATURAS_HPP__  
#define __COLECAO_VIATURAS_HPP__  


#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <optional>
#include <stdexcept>
#include <map>
#include <regex>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/core.h>
#include <cstdlib>

#include "Viaturas.hpp"

namespace colecao_viaturas {
    class DuplicateValue : public std::invalid_argument {
        using std::invalid_argument::invalid_argument;
    };

    class VehicleCollection {
    private:
        // ATRIBUTO DA CLASSE
        std::vector<Viatura> viaturas;
    public:
        std::vector<Viatura> get_collection() {
            return this->viaturas;
        }

        static VehicleCollection from_csv(const std::string& path) {
            VehicleCollection viaturas;

            std::ifstream csv_file(path);
            std::string line;
            while (std::getline(csv_file, line)) {
                // VERIFICAR ESPAÇOS DIREITA/ESQUERDA E/OU ESTÁ VAZIA
                if (utils::trim(line).empty()) {
                    continue; // CONTINUA
                }
                if (line.find("##") == 0 || line.find("//") == 0) {
                    continue;
                }
                viaturas.add(Viatura::from_csv(line));
            }
            return viaturas;
        }

        void to_csv(const std::string& path) {
            std::ofstream csv_file;
            csv_file.open(path);
            for (std::size_t i = 0; i < viaturas.size(); i += 1) {
                csv_file << viaturas[i].to_csv();
                if (i < viaturas.size() - 1) {
                    csv_file << '\n';
                }
            }
            fmt::println("");     
        }

        std::optional<Viatura> search_by_mat(const std::string& matricula) {
            for (const auto& mat : this->viaturas) {
                if (mat.get_matricula() == matricula) {
                    return mat;
                }
            }
            return {};
        }

        void add(const Viatura& viat) {
            if (this->search_by_mat(viat.get_matricula())) {
                throw DuplicateValue(fmt::format("Matricula {} já existe", viat.get_matricula()));
            }
            this->viaturas.emplace_back(viat);
        }

        bool delete_(const std::string& matricula) {
            for (std::size_t i = 0; i < viaturas.size(); i++) {
                if (matricula == viaturas[i].get_matricula()) {
                    // viaturas[i].mostra();
                    viaturas.erase(viaturas.begin() + i);
                    return true;
                }
            }
            return false;
        }

        template<typename F>
        VehicleCollection search(F funcao_criterio) const {
            VehicleCollection found_viaturas;
            for (const auto& viat : this->viaturas) {
                if (funcao_criterio(viat)) {
                    found_viaturas.add(viat);
                }
            }
            return found_viaturas;
        }

        std::vector<Viatura>::iterator begin() {
            return this->viaturas.begin();
        }

        std::vector<Viatura>::iterator end() {
            return this->viaturas.end();
        }

        std::size_t size() const {
            return this->viaturas.size();
        }

        bool empty() const {
            return this->viaturas.empty();
        }
    };
}

#endif