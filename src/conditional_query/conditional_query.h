/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   conditional_query.h
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Declaración de la clase ConditionalQuery, 
 *         que representa una consulta condicional sobre variables binarias discretas.
 */

#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <sstream>

class ConditionalQuery {
 public:
  explicit ConditionalQuery(int);

  uint64_t getMaskC() const { return maskC_; }
  uint64_t getMaskI() const { return maskI_; }
  uint64_t getMaskM() const { return maskM_; }
  uint64_t getValC() const { return valC_; }
  
  const std::vector<int>& getConditionedVariables() const { return conditioned_variables_; }
  const std::vector<int>& getConditionedValues() const { return conditioned_values_; }
  const std::vector<int>& getInterestVariables() const { return interest_variables_; }
  
  int getNumberConditionedVariables() const { return conditioned_variables_.size(); }
  int getNumberInterestVariables() const { return interest_variables_.size(); }
  int getNumberMarginalizedVariables() const;
  
  void addConditionedVariable(int, int);
  void addInterestVariable(int);
  
  void computeMasks();
  bool isValid() const;
  std::string toString() const;
    
private:
  int number_variables_;
  std::vector<int> conditioned_variables_;
  std::vector<int> conditioned_values_;
  std::vector<int> interest_variables_;
  uint64_t maskC_;
  uint64_t valC_;
  uint64_t maskI_;
  uint64_t maskM_;

  void validateVariableIndex(int) const;
  void validateValue(int) const;
};
