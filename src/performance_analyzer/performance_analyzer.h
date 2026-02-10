/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   performance_analyzer.h
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Declaración de la clase PerformanceAnalyzer, que proporciona funcionalidades para analizar 
 *         el rendimiento del motor de inferencia condicional bajo diferentes configuraciones de consultas.
 */

#pragma once

#include "../conditional_inference_engine/conditional_inference_engine.h"
#include "../distribution/binary_distribution/binary_distribution.h"
#include "../conditional_query/conditional_query.h"
#include <vector>
#include <string>
#include <fstream>

/**
 * @brief Estructura para almacenar un punto de medición de rendimiento
 */
struct PerformanceDataPoint {
  int number_interest_variables;
  int number_conditioned_variables;
  int number_marginalized_variables;
  double execution_time;
  uint64_t states_evaluated;
  
  PerformanceDataPoint(int interest, int conditioned, int marginalized, double execution_time, uint64_t states_evaluated) : 
    number_interest_variables(interest), 
    number_conditioned_variables(conditioned),
    number_marginalized_variables(marginalized), 
    execution_time(execution_time),
    states_evaluated(states_evaluated) {}
};

/**
 * @brief Estructura para almacenar estadísticas básicas de un conjunto de mediciones
 */
struct Statistics {
  double mean;
  double minimum;
  double maximum;
  double standard_deviation;
};

class PerformanceAnalyzer {
 public:
  void runAnalysis(const BinaryDistribution&, int, int, int = 5);
  
  const std::vector<PerformanceDataPoint>& getMeasurements() const { return measurements_; }
  void addMeasurement(const BinaryDistribution&, const ConditionalQuery&);

  void exportToCSV(const std::string&) const;
  void displayStatistics() const;
  void generateReport(const std::string&) const;

  void clear() { measurements_.clear(); }

 private:
  std::vector<PerformanceDataPoint> measurements_;
  
  Statistics computeStatistics(const std::vector<double>&) const;
};
