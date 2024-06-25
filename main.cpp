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
#include "Viaturas.hpp"
#include "Colecao_Viaturas.hpp"

using namespace std;
using namespace fmt;
using namespace colecao_viaturas;
 
////////////////////////////////////////////////////////////////////////////////
// MAIN
////////////////////////////////////////////////////////////////////////////////
 
VehicleCollection viaturas;
/* definindo como global a variável criada no main que recebe a lista dos produtos */
const int DEFAULT_INDENTATION = 3;
 

// FUNÇÃO PRINTLN COM IDENTAÇÃO PRÉ DEFINIDA
void show_msg(const string& msg, int indent = DEFAULT_INDENTATION) {
    println("{}{}", string(indent, ' '), msg);
}
 

// FUNÇÃO ASK(OUTPUT) + ANSWER(INPUT)
string ask(const string& question, int indent = DEFAULT_INDENTATION) {
    print("{}{}", string(indent, ' '), question);
    string answer;
   
    getline(cin, answer);
    string test =utils::to_upper_copy(answer);
    //string modelo = utils::to_upper_copy(answer);
    return test;
}

 
// FUNÇÃO CLEAR PARA LIMPAR TELA
void clear_screen() {
#if defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
    system("clear");
#elif _WIN32
    system("cls");
#endif
}
 

// FUNÇÃO PAUSE
void pause_(int indent = 3) {
#if defined(__linux__) || defined(__FreeBDS__)  || defined (__APPLE__)
    show_msg("Pressione qualquer tecla para continuar...");
    system("read -s -n 1");
#elif _WIN32
    show_msg("Pressione qualquer tecla para continuar...");
    system("pause>nul");     // NOTA: confirmar se comando está correcto em Windows
#else
    println("   Pressione ENTER para continuar...");
    string nao_interessa;
    getline(cin, nao_interessa);
#endif
}
 
 
// FUNÇÃO LISTAGEM DE VIATURAS 
void exec_list_viaturas() {
    clear_screen();
    println("");
    auto header = format(
        "{:^16} | {:^20} | {:^20} | {:^16}",
        "MATRICULA", "MARCA", "MODELO", "DATA"
    );
    auto sep = format(
        "{}-+-{}-+-{}-+-{}",
        string(16, '-'), string(20, '-'), string(20, '-'), string(16, '-')
    );
    show_msg(header);
    show_msg(sep);
    for (const auto& viat : viaturas) {
        auto data_line = format(
            "{:<16} | {:<20} | {:<20} | {:>16}",
            viat.get_matricula(),
            viat.get_marca(),
            viat.get_modelo(),
            viat.get_data() 
        );
        show_msg(data_line);
        // show_msg(sep);
    }
}


// FUNÇÃO PARA ADICIONAR NO VETOR UMA NOVA VIATURA
void acresc_viatura(){
    clear_screen();
    string matricula;
    string marca;
    string modelo;
    string data;
    show_msg("**Novo veiculo**");
    while (true) {
        println("");
        matricula = ask("Insira Matricula: ");
        if (!(viaturas.search_by_mat(matricula)) && Viatura::valida_matricula(matricula)) {
            break;
        }
        println("   Matricula {} já existe ou formato incorreto!", matricula);
    }
    while (true) {
        marca = ask("Insira Marca: ");
        if (Viatura::valida_marca(marca)) {
            break;
        }
        println("   Marca {} inválida", marca);
    }
    while (true) {
        modelo = ask("Insira Modelo: ");
        if(Viatura::valida_modelo(modelo)){
            break;
        }
        println("   Modelo {} inválida", modelo);
    }
    while (true) {
        data = ask("Insira data formato aaaa-MM-dd: ");
        if(Viatura::valida_data(data)){
            break;
        }
        println("   Data {} inválida", data);
    }
    Viatura novaViatura(matricula,marca,modelo,data);
    viaturas.add(novaViatura);
    println("");
    show_msg("Novo veiculo acrescentado com sucesso!\n");
    pause_();
}


// OUTPUT PARA REMOVER VEÍCULO DA LISTA
void delete_viat(){
    string matricula;
    clear_screen();
    println("");
    exec_list_viaturas();
    println("");
    show_msg("**DELETAR**");
    while (true) {
        matricula = ask("Insira Matricula que deseja deletar: ");
        if (Viatura::valida_matricula(matricula)) {
            break;
        }
        println("   Matricula {} formato incorreto!", matricula);
        }
        if(viaturas.delete_(matricula)){
            show_msg("Viatura excluido com sucesso!");
        }else{
            show_msg("Viatura não encontrado!");
        }
        pause_();       
}

 
// FUNÇÃO PARA PESQUISAR POR MATRÍCULA 
void exec_search_matricula(){
    clear_screen();
    println("");
    string respostaMatricula = ask("Insira a matrícula desejada: ");
    int matriculaEncontrada = 0;
    for (const auto& viat : viaturas) {
        auto data_line = format(
            "{:<16} | {:<20} | {:<20} | {:>16}",
            viat.get_matricula(),
            viat.get_marca(),
            viat.get_modelo(),
            viat.get_data()
        );
        if(respostaMatricula == viat.get_matricula()){
            show_msg(data_line);
            matriculaEncontrada = 1;
        }
    }
    if(!matriculaEncontrada){
        show_msg("Matrícula não encontrada");
    }
    pause_();
}

 
//FUNÇÃO PARA PESQUISAR MARCA
void exec_search_marca(){
    clear_screen();
    println("");
    string resposta = ask("Insira a marca desejada: ");
    int encontrado = 0;
    for (const auto& viat : viaturas) {
        auto data_line = format(
            "{:<16} | {:<20} | {:<20} | {:>16}",
            viat.get_matricula(),
            viat.get_marca(),
            viat.get_modelo(),
            viat.get_data()
        );
        if(resposta == viat.get_marca()){
            show_msg(data_line);
            encontrado = 1;
        }
    }
    if(!encontrado){
        show_msg("Viatura não encontrada");
    }  
    pause_();
}

 
//FUNÇÃO PESQUISAR POR MODELO
void exec_search_modelo() {
    clear_screen();
    string modelo = ask("Digite o modelo desejado: ");
    int modeloEncontrado = 0;
    for (const auto& viat : viaturas) {
        auto data_line = format(
            "{:<16} | {:<20} | {:<20} | {:>16}",
            viat.get_matricula(),
            viat.get_marca(),
            viat.get_modelo(),
            viat.get_data()
        );
        if(modelo == viat.get_modelo()){
            show_msg(data_line);
            modeloEncontrado = 1;
        }
    }
    if(!modeloEncontrado){
        show_msg("Modelo não encontrado");
    }
    pause_();
}
 
 
//FUNÇÃO PARA TERMINAR O APP
void exec_end() {
    println("");
    show_msg("[+] A actualizar catálogo ...");
    show_msg("[+] ... catálogo actualizado");
    show_msg("[+] Programa vai terminar.");
    exit(0);
}
 

// FUNÇÃO PARA CHAMAR O MENU INTERAÇÃO USUÁRIO
void exec_menu() {
    while (true) {
        clear_screen();
        println("");
        show_msg("#################################################");
        show_msg("#                                               #");
        show_msg("#  L  - Listar catálogo                         #");
        show_msg("#  P  - Pesquisar por matricula                 #");
        show_msg("#  PM - Pesquisar por marca                     #");
        show_msg("#  PN - Pesquisar por modelo                    #");
        show_msg("#  A  - Acrescentar viatura                     #");
        show_msg("#  E  - Eliminar viatura                        #");
        show_msg("#  G  - Guardar catálogo em ficheiro            #");
        show_msg("#                                               #");
        show_msg("#  T  - Terminar o programa                     #");
        show_msg("#                                               #");
        show_msg("#################################################");
        println("");
 
        auto opcao = ask("OPÇÃO> ");
        auto OPCAO = utils::to_upper_copy(opcao);
        if (OPCAO == "L" || OPCAO == "LISTAR") {
            exec_list_viaturas();
            pause_();
        }
        else if(OPCAO == "P" || OPCAO == "PESQUISAR POR MATRICULA"){
           exec_search_matricula();  
        }
        else if(OPCAO == "PM" || OPCAO == "PESQUISAR POR MARCA"){
           exec_search_marca(); 
        }
        else if(OPCAO == "PN" || OPCAO == "PESQUISAR POR MODELO"){
           exec_search_modelo(); 
        }
        else if(OPCAO == "A" || OPCAO == "ACRESCENTAR"){
            acresc_viatura();
        }
        else if(OPCAO == "G" || OPCAO == "GUARDAR"){
            viaturas.to_csv("viaturas.csv");
            show_msg("Guardado com sucesso!");
            pause_();
        }
        else if(OPCAO == "E" || OPCAO == "ELIMINAR"){
            delete_viat();
        }
        else if (OPCAO == "T" || OPCAO == "TERMINAR") {
            viaturas.to_csv("viaturas.csv");// gravar catálogo em disco
            exec_end();
        }
        else {
            println("");
            println("ATENÇÃO: Opção {} inválida", opcao);
            pause_();
        }
    }
}


// FUNÇÃO MAIN PRINCIPAL
int main() {
    //INSTANCIANDO DO FICHEIRO CSV
    viaturas = VehicleCollection::from_csv("viaturas.csv");
    exec_menu();
}