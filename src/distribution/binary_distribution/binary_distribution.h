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

/**
 * @brief Clase que representa una distribución conjunta de variables binarias discretas
 * Cada variable puede tomar valores 0 o 1, y la distribución asigna una probabilidad a cada combinación posible.
 */
class BinaryDistribution : public IDistribution {
 public:
  explicit BinaryDistribution(int);
  explicit BinaryDistribution(const std::string&);
  int getNumberVariables() const override { return numero_variables_; }
  const std::vector<double>& getProbabilities() const {
    return probabilidades_;
  }
  uint64_t getStateSpaceSize() const override {
    return tamano_espacio_estados_;
  }
  double getProbability(uint64_t) const override;
  void setProbability(uint64_t, double) override;
  void normalize() override;
  bool isValid() const override;
  void generateRandom();
  std::string indexToBinary(uint64_t) const;
  void display() const override;
  void exportToCSV(const std::string&) const override;
    
 private:
  int numero_variables_;
  std::vector<double> probabilidades_;
  uint64_t tamano_espacio_estados_;

  void loadFromCSV(const std::string&);
  uint64_t binaryToIndex(const std::string&) const;
};