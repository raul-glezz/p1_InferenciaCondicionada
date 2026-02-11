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
  int numero_variables_interes;
  int numero_variables_condicionadas;
  int numero_variables_marginalizadas;
  double tiempo_ejecucion;
  uint64_t estados_evaluados;
  
  PerformanceDataPoint(int interes, int condicionadas, int marginalizadas,
                       double tiempo, uint64_t estados)
      : numero_variables_interes(interes),
        numero_variables_condicionadas(condicionadas),
        numero_variables_marginalizadas(marginalizadas),
        tiempo_ejecucion(tiempo),
        estados_evaluados(estados) {}
};

/**
 * @brief Estructura para almacenar estadísticas básicas de un conjunto de
 *        mediciones
 */
struct Statistics {
  double media;
  double minimo;
  double maximo;
  double desviacion_estandar;
};

/**
 * @brief Clase para analizar el rendimiento del motor de inferencia
 *        condicional bajo diferentes configuraciones de consultas
 */
class PerformanceAnalyzer {
 public:
  void runAnalysis(const BinaryDistribution&, int, int, int = 5);
  
  const std::vector<PerformanceDataPoint>& getMeasurements() const {
    return mediciones_;
  }
  void addMeasurement(const BinaryDistribution&, const ConditionalQuery&);

  void exportToCSV(const std::string&) const;
  void displayStatistics() const;
  void generateReport(const std::string&) const;

  void clear() { mediciones_.clear(); }

 private:
  std::vector<PerformanceDataPoint> mediciones_;

  Statistics computeStatistics(const std::vector<double>&) const;
};
