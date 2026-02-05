/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   conditional_inference_engine.cc
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Implementación de la clase ConditionalInferenceEngine, 
 *         que implementa el motor de inferencia para distribuciones binarias,
 *         permitiendo calcular distribuciones condicionales a partir de una distribución conjunta.
 */

#include <cstring>
#include <stdexcept>
#include <numeric>
#include "conditional_inference_engine.h"

/**
 * @brief Constructor del motor de inferencia
 * @param jointDist Distribución conjunta sobre la que realizar inferencias
 */
ConditionalInferenceEngine::ConditionalInferenceEngine(const BinaryDistribution& jointDist)
  : jointDistribution_(jointDist) {
}

/**
 * @brief Versión de bajo nivel que devuelve array de doubles
 * @param maskC Máscara de variables condicionadas
 * @param valC Valores de variables condicionadas
 * @param maskI Máscara de variables de interés
 * @return Array con probabilidades condicionales (el llamador debe liberar memoria)
 */
double* ConditionalInferenceEngine::prob_cond_bin(uint64_t maskC, uint64_t valC, uint64_t maskI) {
  int numInterestBits = countBits(maskI);
  uint64_t interestStates = 1ULL << numInterestBits;

  // Asignar memoria para el resultado
  double* out = new double[interestStates];
  std::memset(out, 0, interestStates * sizeof(double));

  // Iterar sobre todos los estados
  uint64_t totalStates = jointDistribution_.getStateSpaceSize();

  for (uint64_t state = 0; state < totalStates; ++state) {
      if (isConsistent(state, maskC, valC)) {
          uint64_t interestIdx = extractInterestBits(state, maskI);
          out[interestIdx] += jointDistribution_.getProbability(state);
      }
  }

  // Normalizar
  double sum = 0.0;
  for (uint64_t i = 0; i < interestStates; ++i) {
      sum += out[i];
  }

  if (sum > 1e-10) {
      for (uint64_t i = 0; i < interestStates; ++i) {
          out[i] /= sum;
      }
  }

  return out;
}

/**
 * @brief Verifica si una configuración es consistente con las condiciones
 * @param state Estado completo
 * @param maskC Máscara de variables condicionadas
 * @param valC Valores esperados
 */
bool ConditionalInferenceEngine::isConsistent(uint64_t state, uint64_t maskC, uint64_t valC) const {
    // Un estado es consistente si los bits marcados en maskC tienen los valores de valC
    return (state & maskC) == valC;
}

/**
 * @brief Extrae el valor de las variables de interés de un estado
 * @param state Estado completo
 * @param maskI Máscara de variables de interés
 */
uint64_t ConditionalInferenceEngine::extractInterestBits(uint64_t state, uint64_t maskI) const {
    // Extraer solo los bits marcados en maskI y compactarlos
    uint64_t result = 0;
    int resultBit = 0;
    
    for (int bit = 0; bit < 64; ++bit) {
        if (maskI & (1ULL << bit)) {
            if (state & (1ULL << bit)) {
                result |= (1ULL << resultBit);
            }
            resultBit++;
        }
    }
    
    return result;
}

/**
 * @brief Cuenta el número de bits activos en una máscara
 */
int ConditionalInferenceEngine::countBits(uint64_t mask) const {
    int count = 0;
    while (mask) {
        count += mask & 1;
        mask >>= 1;
    }
    return count;
}
