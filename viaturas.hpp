#ifndef __VIATURAS_HPP__
#define __VIATURAS_HPP__

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

#include "Utils.hpp"

using namespace fmt;

//definido para separar quando enviar o objeto para csv em fmt::formato de string
const std::string CSV_DELIM = "|";

namespace colecao_viaturas {

    // HERDA CONSTRUTORES DA CLASSE invalid_argument
    class InvalidAttr : public std::invalid_argument {
        using std::invalid_argument::invalid_argument;       
    };

    ////////////////////////////////////////////////////////////////////////////////
    //       VIATURA
    ////////////////////////////////////////////////////////////////////////////////
    class Viatura {
    public:
        Viatura(
                const std::string& matricula,  // matricula: DD-LL-DD onde D: Dígito L: Letra
                const std::string& marca,      // deve ter uma ou mais palavras (apenas letras ou dígitos)
                const std::string& modelo,     // mesmo que a marca
                const std::string& data        // deve vir no fmt::formato ISO: 'YYYY-MM-DD'
        ) {
            // VALIDAÇÃO DOS PARÂMETROS
            if (!this->valida_matricula(matricula)) {
                throw InvalidAttr(fmt::format("Matricula {} inválida", matricula));
            }
            if (!this->valida_marca(marca)) {
                throw InvalidAttr(fmt::format("Marca {} inválida", marca));
            }
            if (!this->valida_modelo(modelo)) {
                throw InvalidAttr(fmt::format("Modelo {} inválida", modelo));
            }
            if (!this->valida_data(data)) {
                throw InvalidAttr(fmt::format("Data {} inválida", data));
            }
            // Associar parâmetros a atributos depois da validação
            this->matricula = matricula;
            this->marca = marca;
            this->modelo = modelo;
            this->data = data;
        }
        Viatura(
                const std::string& matricula,
                const std::string& marca,
                const std::string& modelo
        ) : Viatura(matricula, marca, modelo, "2024-01-01")
        {
        }

        // criando um objeto á partir csv
        static Viatura from_csv(const std::string& viat_csv) {
            auto attrs = utils::split(viat_csv, CSV_DELIM);
            if (attrs.size() != 4) {
                throw InvalidAttr("from_csv: Número de atributos inválidos");
            }
            return Viatura(
                attrs[0],       // matricula
                attrs[1],       // marca
                attrs[2],       // modelo
                attrs[3]        // data
            );
        }
    
        // Montando uma string com delimitador para mandar para o csv
        std::string to_csv() {
            return utils::join(
                {
                    this->matricula,
                    this->marca,
                    this->modelo,
                    this->data
                },
                CSV_DELIM
            );
        }

        // MÉTODO PARA MOSTRAR VIATURA
        void mostra() const {
            println(
                "   Matricula -> {} | Marca/modelo -> {}/{}",
                this->matricula, this->marca, this->modelo
            );
        }
    
        // MÉTODO PARA VALIDAR MATRÍCULA
        static bool valida_matricula(const std::string& matricula) {
            return std::regex_match(matricula, std::regex("^[0-9]{2}-[A-Z]{2}-[0-9]{2}$"));//expreções regulares manipula a string 
        }   //de acordo com o padrão especifico

        // MÉTODO PARA VALIDAR MARCA
        static bool valida_marca(const std::string& marca) {
            // return regex_match(marca, regex("^([A-Z0-9]+)+$"));
            // não pode conter simbolos estranhos
            auto palavras = utils::split(marca);
            for (const auto& palavra : palavras) {
                if (!utils::is_alnum(palavra)) {
                    return false;
                }
            }
            return palavras.size() > 0;
        }

        // MÉTODO PARA VALIDAR MODELO
        static bool valida_modelo(const std::string& modelo) {
            return Viatura::valida_marca(modelo);
        }
        // Validando data com o fmt::formato aaaa/MM/dd
        static bool valida_data(const std::string& data) {
            // return regex_match(data, regex("^\\d{4}-\\d{2}-\\d{2}$"));
            auto date_parts = utils::split(data, "-");
            if (date_parts.size() != 3) {
                return false;
            }
            auto year = date_parts[0];
            auto month = date_parts[1];
            auto day = date_parts[2];
            return year.size() == 4 && utils::is_digit(year)
                && month.size() == 2 && utils::is_digit(month)
                && day.size() == 2 && utils::is_digit(day)
            ;
        }

        // DEFININDO GETTER
        std::string get_matricula() const {
            return this->matricula;
        }

        // DEFININDO SETTER
        void set_matricula(const std::string& nova_matricula) {
            if (!valida_matricula(nova_matricula)) {
                throw InvalidAttr(fmt::format("Matricula inválida: {}", nova_matricula));
            }
            this->matricula = nova_matricula;
        }
    
        std::string get_marca() const {
            return this->marca;
        }
    
        std::string get_modelo() const {
            return this->modelo;
        }
    
        std::string get_data() const {
            return this->data;
        }
    
        void set_data(const std::string& nova_data) {
            if (!this->valida_data(nova_data)) {
                throw InvalidAttr(fmt::format("Data inválida: {}", nova_data));
            }
            this->data = nova_data;
        }
    
        int get_ano() const {
            auto ano_str = utils::split(this->data, "-")[0];
            return utils::convert<int>(ano_str);
        }
    private:
        std::string matricula;
        std::string marca;
        std::string modelo;
        std::string data;
    };
}


#endif