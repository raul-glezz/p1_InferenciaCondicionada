/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   conditional_query.cc
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Definición de la clase ConditionalQuery, 
 *         que representa una consulta condicional sobre variables binarias discretas.
 */

#include <algorithm>
#include <iomanip>
#include <bitset>

#include "conditional_query.h"

/**
 * @brief Constructor que inicializa la consulta con un número dado de variables
 * @param[in] numero_variables: Número total de variables en la consulta
 * @throws std::invalid_argument si el número de variables es menor o igual a 0
 *         o mayor a 64
 */
ConditionalQuery::ConditionalQuery(int numero_variables)
    : numero_variables_(numero_variables), maskC_(0), valC_(0), maskI_(0),
      maskM_(0) {
  if (numero_variables <= 0 || numero_variables > 64) {
    throw std::invalid_argument(
        "El número de variables debe estar entre 1 y 64");
  }
}

/**
 * @brief Método para añadir una variable condicionada a la consulta
 * @param[in] indice_variable: Índice de la variable a condicionar (0-based)
 * @param[in] valor: Valor de la variable condicionada (0 o 1)
 * @throws std::invalid_argument si el índice de variable está fuera del rango
 *         permitido, si el valor no es binario, o si la variable ya está en
 *         condicionadas
 */
void ConditionalQuery::addConditionedVariable(int indice_variable,
                                              int valor) {
  validateVariableIndex(indice_variable);
  validateValue(valor);
  
  auto it = std::find(variables_condicionadas_.begin(),
                      variables_condicionadas_.end(), indice_variable);
  if (it != variables_condicionadas_.end()) {
    throw std::invalid_argument("Variable ya condicionada: " +
                                std::to_string(indice_variable));
  }
  
  it = std::find(variables_interes_.begin(), variables_interes_.end(),
                 indice_variable);
  if (it != variables_interes_.end()) {
    throw std::invalid_argument("Variable ya en el conjunto de interés: " +
                                std::to_string(indice_variable));
  }
  
  variables_condicionadas_.push_back(indice_variable);
  valores_condicionados_.push_back(valor);
}

/**
 * @brief Método para añadir una variable de interés a la consulta
 * @param[in] indice_variable: Índice de la variable de interés (0-based)
 * @throws std::invalid_argument si el índice de variable está fuera del rango
 *         permitido, o si la variable ya está en interés o condicionadas
 */
void ConditionalQuery::addInterestVariable(int indice_variable) {
  validateVariableIndex(indice_variable);
  
  auto it = std::find(variables_interes_.begin(), variables_interes_.end(),
                      indice_variable);
  if (it != variables_interes_.end()) {
    throw std::invalid_argument("Variable ya en el conjunto de interés: " +
                                std::to_string(indice_variable));
  }
  
  it = std::find(variables_condicionadas_.begin(),
                 variables_condicionadas_.end(), indice_variable);
  if (it != variables_condicionadas_.end()) {
    throw std::invalid_argument("Variable ya condicionada: " +
                                std::to_string(indice_variable));
  }
  
  variables_interes_.push_back(indice_variable);
}

/**
 * @brief Método para calcular las máscaras de la consulta
 *        a partir de las variables condicionadas e interés
 */
void ConditionalQuery::computeMasks() {
  maskC_ = 0;
  valC_ = 0;
  maskI_ = 0;
  maskM_ = 0;
  
  for (size_t i = 0; i < variables_condicionadas_.size(); ++i) {
    int indice_var = variables_condicionadas_[i];
    int valor = valores_condicionados_[i];
    
    maskC_ |= (1ULL << indice_var);
    if (valor == 1) {
      valC_ |= (1ULL << indice_var);
    }
  }
  
  for (int indice_var : variables_interes_) {
    maskI_ |= (1ULL << indice_var);
  }
  
  uint64_t mascara_todas = (1ULL << numero_variables_) - 1;
  maskM_ = mascara_todas & ~(maskC_ | maskI_);
}

/**
 * @brief Método para obtener el número de variables a marginalizar
 * @return Número de variables que no están ni en condicionadas ni en interés
 */
int ConditionalQuery::getNumberMarginalizedVariables() const {
  int total = numero_variables_;
  int usadas = variables_condicionadas_.size() + variables_interes_.size();
  return total - usadas;
}

/**
 * @brief Método para validar que la consulta sea consistente
 * @return True si la consulta es válida, false en caso contrario
 */
bool ConditionalQuery::isValid() const {
  // Debe haber al menos una variable de interés
  if (variables_interes_.empty()) {
    return false;
  }
  // No puede haber variables repetidas entre condicionadas e interés
  if (variables_condicionadas_.size() != valores_condicionados_.size()) {
    return false;
  }
  
  return true;
}

/**
 * @brief Método para obtener una representación en cadena de la consulta
 * @return String que describe la consulta, incluyendo las variables de interés,
 *         condicionadas y sus máscaras
 */
std::string ConditionalQuery::toString() const {
  std::ostringstream oss;
  
  oss << "Consulta: P(";
  
  for (size_t i = 0; i < variables_interes_.size(); ++i) {
    if (i > 0) oss << ", ";
    oss << "X" << (variables_interes_[i] + 1);
  }
  
  if (!variables_condicionadas_.empty()) {
    oss << " | ";
    
    for (size_t i = 0; i < variables_condicionadas_.size(); ++i) {
      if (i > 0) oss << ", ";
      oss << "X" << (variables_condicionadas_[i] + 1) << "="
          << valores_condicionados_[i];
    }
  }
  
  oss << ")" << std::endl;
  
  oss << "  maskC: "
      << std::bitset<64>(maskC_).to_string().substr(64 - numero_variables_)
      << std::endl;
  oss << "  valC:  "
      << std::bitset<64>(valC_).to_string().substr(64 - numero_variables_)
      << std::endl;
  oss << "  maskI: "
      << std::bitset<64>(maskI_).to_string().substr(64 - numero_variables_)
      << std::endl;
  oss << "  maskM: "
      << std::bitset<64>(maskM_).to_string().substr(64 - numero_variables_)
      << std::endl;
  
  return oss.str();
}

/**
 * @brief Método para validar que el índice de variable esté dentro del rango
 *        permitido
 * @param[in] indice_variable: Índice de la variable a validar
 * @throws std::invalid_argument si el índice está fuera del rango
 *         [0, numero_variables_-1]
 */
void ConditionalQuery::validateVariableIndex(int indice_variable) const {
  if (indice_variable < 0 || indice_variable >= numero_variables_) {
    throw std::invalid_argument(
        "Variable fuera de rango: " + std::to_string(indice_variable) +
        " (Debe ser 0-" + std::to_string(numero_variables_ - 1) + ")");
  }
}

/**
 * @brief Método para validar que el valor de variable sea binario
 * @param[in] valor: Valor a validar
 * @throws std::invalid_argument si el valor no es 0 o 1
 */
void ConditionalQuery::validateValue(int valor) const {
  if (valor != 0 && valor != 1) {
    throw std::invalid_argument("El valor debe ser 0 o 1, se recibió: " +
                                std::to_string(valor));
  }
}
