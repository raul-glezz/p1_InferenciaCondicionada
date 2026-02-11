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
 * @param[in] distribucion_conjunta: Distribución conjunta sobre la que realizar
 *                                   inferencias
 */
ConditionalInferenceEngine::ConditionalInferenceEngine(
    const BinaryDistribution& distribucion_conjunta)
    : distribucion_conjunta_(distribucion_conjunta) {}

/**
 * @brief Método para calcular la distribución condicional P(X_I | X_C = c)
 *        usando marginalización
 * @param[in] maskC: Máscara de variables condicionadas
 * @param[in] valC: Valores de variables condicionadas
 * @param[in] maskI: Máscara de variables de interés
 * @return Array con probabilidades condicionales
 */
double* ConditionalInferenceEngine::prob_cond_bin(uint64_t maskC,
                                                   uint64_t valC,
                                                   uint64_t maskI) {
  int numero_bits_interes = countBits(maskI);
  uint64_t estados_interes = 1ULL << numero_bits_interes;

  double* salida = new double[estados_interes];
  std::memset(salida, 0, estados_interes * sizeof(double));

  uint64_t total_estados = distribucion_conjunta_.getStateSpaceSize();
  for (uint64_t estado = 0; estado < total_estados; ++estado) {
    if (isConsistent(estado, maskC, valC)) {
      uint64_t indice_interes = extractInterestBits(estado, maskI);
      salida[indice_interes] += distribucion_conjunta_.getProbability(estado);
    }
  }

  double suma = 0.0;
  for (uint64_t i = 0; i < estados_interes; ++i) {
    suma += salida[i];
  }
  if (suma > 1e-10) {
    for (uint64_t i = 0; i < estados_interes; ++i) {
      salida[i] /= suma;
    }
  }

  return salida;
}

/**
 * @brief Método principal para calcular la distribución condicional
 *        P(X_I | X_C = c)
 * @param[in] consulta: Consulta condicional que especifica las variables de
 *                      interés y condicionadas
 * @return Estructura con la distribución condicional resultante y métricas de
 *         ejecución
 */
InferenceResult ConditionalInferenceEngine::computeConditional(
    const ConditionalQuery& consulta) {
  InferenceResult resultado;

  auto inicio = std::chrono::high_resolution_clock::now();

  double* salida =
      prob_cond_bin(consulta.getMaskC(), consulta.getValC(),
                    consulta.getMaskI());
  int numero_bits_interes = countBits(consulta.getMaskI());
  uint64_t estados_interes = 1ULL << numero_bits_interes;
  
  auto distribucion =
      std::make_unique<BinaryDistribution>(numero_bits_interes);
  for (uint64_t i = 0; i < estados_interes; ++i) {
    distribucion->setProbability(i, salida[i]);
  }

  auto fin = std::chrono::high_resolution_clock::now();
  resultado.tiempo_ejecucion =
      std::chrono::duration<double, std::micro>(fin - inicio).count();
  resultado.estados_evaluados =
      distribucion_conjunta_.getStateSpaceSize();
  resultado.distribucion = std::move(distribucion);

  return resultado;
}

/**
 * @brief Método para verificar si una configuración es consistente con las
 *        condiciones
 * @param[in] estado Estado completo
 * @param[in] maskC Máscara de variables condicionadas
 * @param[in] valC Valores esperados
 */
bool ConditionalInferenceEngine::isConsistent(uint64_t estado, uint64_t maskC,
                                               uint64_t valC) const {
  return (estado & maskC) == valC;
}

/**
 * @brief Método para extraer los bits de interés de un estado dado una máscara
 * @param[in] estado: Estado completo
 * @param[in] maskI: Máscara de variables de interés
 */
uint64_t ConditionalInferenceEngine::extractInterestBits(uint64_t estado,
                                                          uint64_t maskI) const {
  uint64_t resultado = 0;
  int bit_resultado = 0;
  
  for (int bit = 0; bit < 64; ++bit) {
    if (maskI & (1ULL << bit)) {
      if (estado & (1ULL << bit)) {
        resultado |= (1ULL << bit_resultado);
      }
      bit_resultado++;
    }
  }
  
  return resultado;
}

/**
 * @brief Método para contar el número de bits a 1 en una máscara
 * @param[in] mascara: Máscara a analizar
 * @return Número de bits a 1 en la máscara
 */
int ConditionalInferenceEngine::countBits(uint64_t mascara) const {
  int cuenta = 0;
  while (mascara) {
    cuenta += mascara & 1;
    mascara >>= 1;
  }
  return cuenta;
}
