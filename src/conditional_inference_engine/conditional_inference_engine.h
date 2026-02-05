/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   conditional_inference_engine.h
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Declaración de la clase ConditionalInferenceEngine, 
 *         que implementa el motor de inferencia para distribuciones binarias,
 *         permitiendo calcular distribuciones condicionales a partir de una distribución conjunta.
 */

#pragma once

#include "../distribution/binary_distribution/binary_distribution.h"
#include <memory>
#include <chrono>
#include <vector>

/**
 * @brief Resultado de una inferencia condicional
 */
struct InferenceResult {
    std::unique_ptr<BinaryDistribution> distribution;  // Distribución condicional resultante
    double executionTime;                              // Tiempo de ejecución en microsegundos
    uint64_t statesEvaluated;                          // Número de estados evaluados
    
    InferenceResult() : distribution(nullptr), executionTime(0.0), statesEvaluated(0) {}
};

/**
 * @brief Motor de inferencia para distribuciones binarias
 */
class ConditionalInferenceEngine {
 public:
  explicit ConditionalInferenceEngine(const BinaryDistribution& jointDist);
  double* prob_cond_bin(uint64_t maskC, uint64_t valC, uint64_t maskI);
    
 protected:
  bool isConsistent(uint64_t state, uint64_t maskC, uint64_t valC) const;
  uint64_t extractInterestBits(uint64_t state, uint64_t maskI) const;
  int countBits(uint64_t mask) const;

 private:
  const BinaryDistribution& jointDistribution_;
};