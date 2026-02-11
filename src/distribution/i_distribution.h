/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   i_distribution.h
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Implementación de la clase base IDistribution 
 *         para distribuciones de probabilidad
 */

#pragma once

#include <vector>
#include <string>
#include <cstdint>

class IDistribution {
 public:
  virtual ~IDistribution() = default;
  
  virtual int getNumberVariables() const = 0;
  virtual double getProbability(uint64_t) const = 0;
  virtual uint64_t getStateSpaceSize() const = 0;
  
  virtual void setProbability(uint64_t, double) = 0;
  
  virtual bool isValid() const = 0;
  virtual void normalize() = 0;
  
  virtual void display() const = 0;
  virtual void exportToCSV(const std::string&) const = 0;
};