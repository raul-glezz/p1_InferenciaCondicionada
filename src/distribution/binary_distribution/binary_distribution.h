/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   binary_distribution.h
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Declaración de la clase BinaryDistribution, 
 *         que representa una distribución conjunta de variables binarias discretas.
 */

#pragma once

#include "../i_distribution.h"

// EPSILON: tolerancia para comparaciones de punto flotante
static constexpr double EPSILON = 1e-9;

class BinaryDistribution : public IDistribution {
 public:
  //----------------CONSTRUCTORES----------------
  /// Constructor que inicializa la distribución con un número dado de variables
  explicit BinaryDistribution(int);
  /// Constructor que carga la distribución desde un archivo CSV
  explicit BinaryDistribution(const std::string&);
  
  //-----------------------------------MÉTODOS-----------------------------------
  /// Método para obtener el número de variables en la distribución
  int getNumberVariables() const override { return number_variables_; }
  /// Método para obtener el vector completo de probabilidades
  const std::vector<double>& getProbabilities() const { return probabilities_; }
  /// Método para obtener el tamaño del espacio de estados (2^N)
  uint64_t getStateSpaceSize() const override { return state_space_size_; }
  /// Método para obtener la probabilidad de una configuración específica
  double getProbability(uint64_t) const override;
  
  /// Método para establecer la probabilidad de una configuración específica
  void setProbability(uint64_t, double) override;
  
  /// Método para normalizar la distribución
  void normalize() override;
  /// Método para validar que la distribución esté correctamente normalizada
  bool isValid() const override;
  /// Método para generar una distribución aleatoria
  void generateRandom();
  /// Método para convertir un índice numérico a su representación binaria
  std::string indexToBinary(uint64_t) const;
  
  /// Método para mostrar la distribución en consola
  void display() const override;
  /// Método para exportar la distribución a un archivo CSV
  void exportToCSV(const std::string&) const override;
    
 private:
  //--------------ATRIBUTOS--------------
  /// number_variables_: número de variables binarias (N)
  int number_variables_;
  /// probabilities_: vector de probabilidades p[k] para cada configuración
  std::vector<double> probabilities_;
  /// state_space_size_: tamaño del espacio de estados (2^N)
  uint64_t state_space_size_;
  
  //----------------MÉTODOS PRIVADOS----------------
  /// Método que carga la distribución desde un archivo CSV
  void loadFromCSV(const std::string&);
  /// Método que convierte la máscara binaria a su índice numérico
  uint64_t binaryToIndex(const std::string&) const;
};