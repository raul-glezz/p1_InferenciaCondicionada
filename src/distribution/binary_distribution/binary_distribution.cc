/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   binary_distribution.cc
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Implementación de la clase BinaryDistribution, 
 *         que representa una distribución conjunta de variables binarias discretas.
 */

#include <random>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>

#include "binary_distribution.h"

/**
 * @brief Constructor que inicializa la distribución con un número dado de
 *        variables
 * @param[in] numero_variables: Número de variables binarias (N)
 * @throws std::invalid_argument si el número de variables es menor o igual a 0
 *         o mayor a 64
 */
BinaryDistribution::BinaryDistribution(int numero_variables)
    : numero_variables_(numero_variables) {
  if (numero_variables <= 0 || numero_variables > 64) {
    throw std::invalid_argument(
        "Error: El número de variables debe estar entre 1 y 64");
  }
    
  tamano_espacio_estados_ = 1ULL << numero_variables_;
  probabilidades_.resize(tamano_espacio_estados_, 0.0);
}

/**
 * @brief Constructor que carga la distribución desde un archivo CSV
 * @param[in] nombre_archivo: Ruta del archivo CSV
 * @throws std::runtime_error si hay errores al leer el archivo o al parsear
 *         su contenido
 */
BinaryDistribution::BinaryDistribution(const std::string& nombre_archivo) {
  loadFromCSV(nombre_archivo);
}

/**
 * @brief Método para obtener la probabilidad de una configuración específica
 * @param[in] indice: Índice de la configuración (codificación binaria)
 * @return Probabilidad asociada a la configuración dada por el índice
 * @throws std::out_of_range si el índice está fuera del rango válido
 */
double BinaryDistribution::getProbability(uint64_t indice) const {
  if (indice >= tamano_espacio_estados_) {
    throw std::out_of_range("Error: Índice fuera de rango");
  }
  return probabilidades_[indice];
}

/**
 * @brief Método para establecer la probabilidad de una configuración específica
 * @param[in] indice: Índice de la configuración (codificación binaria)
 * @param[in] probabilidad: Probabilidad a asignar
 * @throws std::out_of_range si el índice está fuera del rango válido
 * @throws std::invalid_argument si la probabilidad no está en el rango [0, 1]
 */
void BinaryDistribution::setProbability(uint64_t indice, double probabilidad) {
  if (indice >= tamano_espacio_estados_) {
    throw std::out_of_range("Error: Índice fuera de rango");
  }
  if (probabilidad < 0.0 || probabilidad > 1.0) {
    throw std::invalid_argument(
        "Error: La probabilidad debe estar entre 0 y 1");
  }
  probabilidades_[indice] = probabilidad;
}

/**
 * @brief Método para validar que la distribución esté correctamente normalizada
 * @return true si la suma de las probabilidades es 1 (dentro de una tolerancia),
 *         false en caso contrario
 */
bool BinaryDistribution::isValid() const {
  double suma = 0.0;
  for (double probabilidad : probabilidades_) {
    if (probabilidad < -EPSILON || probabilidad > 1.0 + EPSILON) {
      return false;
    }
    suma += probabilidad;
  }
  return std::abs(suma - 1.0) < EPSILON;
}

/**
 * @brief Método para normalizar la distribución
 * @throws std::runtime_error si la suma de las probabilidades es cero
 *         (no se puede normalizar)
 */
void BinaryDistribution::normalize() {
  double suma = 0.0;
  for (double probabilidad : probabilidades_) {
    suma += probabilidad;
  }
  
  if (suma < EPSILON) {
    throw std::runtime_error(
        "Error: No se puede normalizar, la suma de probabilidades es cero");
  }
  
  for (double& probabilidad : probabilidades_) {
    probabilidad /= suma;
  }
}

/**
 * @brief Método para mostrar la distribución en consola
 */
void BinaryDistribution::display() const {
  std::cout << "=== Distribución Binaria (N=" << numero_variables_ << ") ==="
            << std::endl;
  std::cout << std::fixed << std::setprecision(6);
  
  std::cout << "Config   |";
  for (int i = numero_variables_ - 1; i >= 0; i--) {
    std::cout << " X" << (i + 1);
  }
  std::cout << " | Probabilidad" << std::endl;
  std::cout << std::string(10 + numero_variables_ * 3 + 15, '-')
            << std::endl;
  
  for (uint64_t i = 0; i < tamano_espacio_estados_; ++i) {
    if (probabilidades_[i] > EPSILON) {
      std::string binario = indexToBinary(i);
      std::cout << std::setw(8) << binario << " |";
      for (char bit : binario) {
        std::cout << "  " << bit;
      }
      std::cout << " | " << std::setw(10) << probabilidades_[i] << std::endl;
    }
  }
  std::cout << std::endl;

  std::cout << "Suma total de probabilidades: "
            << std::accumulate(probabilidades_.begin(), probabilidades_.end(),
                               0.0)
            << std::endl;
}

/**
 * @brief Método para exportar la distribución a un archivo CSV
 * @param[in] nombre_archivo: Ruta del archivo CSV de salida
 * @throws std::runtime_error si no se puede abrir el archivo
 */
void BinaryDistribution::exportToCSV(
    const std::string& nombre_archivo) const {
  std::ofstream archivo(nombre_archivo);
  if (!archivo.is_open()) {
    throw std::runtime_error("Error: No se puede abrir el archivo " +
                             nombre_archivo);
  }
  
  archivo << std::fixed << std::setprecision(10);
  for (uint64_t i = 0; i < tamano_espacio_estados_; ++i) {
    archivo << indexToBinary(i) << "," << probabilidades_[i] << std::endl;
  }
  
  archivo.close();
}

/**
 * @brief Método para generar una distribución aleatoria
 */
void BinaryDistribution::generateRandom() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.1, 10.0);
  
  for (uint64_t i = 0; i < tamano_espacio_estados_; i++) {
    probabilidades_[i] = dis(gen);
  }
  normalize();
}

/**
 * @brief Método para convertir un índice numérico a su representación binaria
 * @param[in] indice: Índice numérico a convertir
 * @return Cadena de texto con la representación binaria del índice, con ceros
 *         a la izquierda
 */
std::string BinaryDistribution::indexToBinary(uint64_t indice) const {
  std::string binario;
  binario.reserve(numero_variables_);
  
  for (int i = 0; i < numero_variables_; ++i) {
    binario = std::string(1, (indice & 1 ? '1' : '0')) + binario;
    indice >>= 1;
  }
  
  return binario;
}

/**
 * @brief Método que carga la distribución desde un archivo CSV
 * @param[in] nombre_archivo: Ruta del archivo CSV
 * @throws std::runtime_error si hay errores al leer el archivo o al parsear
 *         su contenido
 */
void BinaryDistribution::loadFromCSV(const std::string& nombre_archivo) {
  std::ifstream archivo(nombre_archivo);
  if (!archivo.is_open()) {
    throw std::runtime_error("No se puede abrir el archivo: " +
                             nombre_archivo);
  }
  
  std::string linea;
  std::vector<std::pair<std::string, double>> datos;
  
  while (std::getline(archivo, linea)) {
    if (linea.empty()) continue;
    
    size_t posicion_coma = linea.find(',');
    if (posicion_coma == std::string::npos) {
      throw std::runtime_error("Formato CSV inválido: " + linea);
    }
    
    std::string binario = linea.substr(0, posicion_coma);
    double probabilidad = std::stod(linea.substr(posicion_coma + 1));
    
    datos.push_back({binario, probabilidad});
  }
  
  archivo.close();
  
  if (datos.empty()) {
    throw std::runtime_error("Archivo CSV vacío");
  }
  
  numero_variables_ = datos[0].first.length();
  tamano_espacio_estados_ = 1ULL << numero_variables_;
  probabilidades_.resize(tamano_espacio_estados_, 0.0);
  
  for (const auto& [binario, probabilidad] : datos) {
    if (binario.length() != static_cast<size_t>(numero_variables_)) {
      throw std::runtime_error("Longitud de máscara inconsistente en CSV");
    }
    
    uint64_t indice = binaryToIndex(binario);
    probabilidades_[indice] = probabilidad;
  }
}

/**
 * @brief Método que convierte la máscara binaria a su índice numérico
 * @param[in] binario: Cadena de texto con la representación binaria
 * @return Índice numérico correspondiente a la máscara binaria
 */
uint64_t BinaryDistribution::binaryToIndex(const std::string& binario) const {
  uint64_t indice = 0;
  for (char bit : binario) {
    indice = (indice << 1) | (bit == '1' ? 1 : 0);
  }
  return indice;
}
