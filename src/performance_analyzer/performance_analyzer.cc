/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   performance_analyzer.cc
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Definición de la clase PerformanceAnalyzer, que proporciona funcionalidades para analizar 
 *         el rendimiento del motor de inferencia condicional bajo diferentes configuraciones de consultas.
 */

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <random>
#include <map>

#include "performance_analyzer.h"

/**
 * @brief Método para ejecutar un análisis de rendimiento en una distribución y
 *        consulta dadas
 * @param[in] distribucion: La distribución binaria sobre la que se realizarán
 *                          las inferencias
 * @param[in] max_variables_interes: Número máximo de variables de interés a
 *                                   probar en las consultas
 * @param[in] max_variables_condicionadas: Número máximo de variables
 *                                         condicionadas a probar en las
 *                                         consultas
 * @param[in] repeticiones: Número de repeticiones para cada configuración de
 *                          consulta
 */
void PerformanceAnalyzer::runAnalysis(const BinaryDistribution& distribucion,
                                       int max_variables_interes,
                                       int max_variables_condicionadas,
                                       int repeticiones) {
  clear();
  
  int numero_variables = distribucion.getNumberVariables();
  ConditionalInferenceEngine motor(distribucion);
  
  std::random_device rd;
  std::mt19937 gen(rd());
  
  int cuenta_tests = 0;
  int total_tests =
      max_variables_interes * max_variables_condicionadas * repeticiones;
  
  for (int numero_interes = 1;
       numero_interes <= max_variables_interes &&
       numero_interes < numero_variables;
       ++numero_interes) {
    for (int numero_condicionadas = 0;
         numero_condicionadas <= max_variables_condicionadas &&
         (numero_interes + numero_condicionadas) < numero_variables;
         ++numero_condicionadas) {
      // Repetimos para promediar
      for (int repeticion = 0; repeticion < repeticiones; ++repeticion) {
        cuenta_tests++;
        
        // Creamos una consulta aleatoria
        ConditionalQuery consulta(numero_variables);
        
        std::vector<int> variables_disponibles(numero_variables);
        std::iota(variables_disponibles.begin(),
                  variables_disponibles.end(), 0);
        std::shuffle(variables_disponibles.begin(),
                     variables_disponibles.end(), gen);
        
        for (int i = 0; i < numero_interes; ++i) {
          consulta.addInterestVariable(variables_disponibles[i]);
        }
        
        std::uniform_int_distribution<> dis(0, 1);
        for (int i = 0; i < numero_condicionadas; ++i) {
          int valor = dis(gen);
          consulta.addConditionedVariable(
              variables_disponibles[numero_interes + i], valor);
        }
        
        consulta.computeMasks();
        
        // Ejecutamos la inferencia y medimos los resultados
        auto resultado = motor.computeConditional(consulta);
        
        mediciones_.emplace_back(
            numero_interes, numero_condicionadas,
            consulta.getNumberMarginalizedVariables(),
            resultado.tiempo_ejecucion, resultado.estados_evaluados);
      }
    }
  }
}

/**
 * @brief Método para agregar un nuevo punto de medición de rendimiento a la
 *        lista de mediciones
 * @param[in] distribucion: La distribución binaria sobre la que se realizará la
 *                          inferencia
 * @param[in] consulta: La consulta condicional para la cual se medirá el
 *                      rendimiento
 */
void PerformanceAnalyzer::addMeasurement(
    const BinaryDistribution& distribucion,
    const ConditionalQuery& consulta) {
  ConditionalInferenceEngine motor(distribucion);
  auto resultado = motor.computeConditional(consulta);
  
  mediciones_.emplace_back(consulta.getNumberInterestVariables(),
                          consulta.getNumberConditionedVariables(),
                          consulta.getNumberMarginalizedVariables(),
                          resultado.tiempo_ejecucion,
                          resultado.estados_evaluados);
}

/**
 * @brief Método para exportar los puntos de medición de rendimiento a un
 *        archivo CSV
 * @param[in] nombre_archivo: El nombre del archivo CSV donde se exportarán los
 *                            datos
 */
void PerformanceAnalyzer::exportToCSV(
    const std::string& nombre_archivo) const {
  std::ofstream archivo(nombre_archivo);
  if (!archivo.is_open()) {
    throw std::runtime_error(
        "Error: No se pudo abrir el archivo para escritura " +
        nombre_archivo);
  }
  
  archivo << "VariablesInteres,VariablesCondicionadas,"
             "VariablesMarginalizadas,TiempoEjecucion(us),EstadosEvaluados\n";
  
  for (const auto& punto : mediciones_) {
    archivo << punto.numero_variables_interes << ","
            << punto.numero_variables_condicionadas << ","
            << punto.numero_variables_marginalizadas << "," << std::fixed
            << std::setprecision(2) << punto.tiempo_ejecucion << ","
            << punto.estados_evaluados << "\n";
  }
  
  archivo.close();
}

/**
 * @brief Método para mostrar estadísticas básicas de los puntos de medición de
 *        rendimiento almacenados
 */
void PerformanceAnalyzer::displayStatistics() const {
  if (mediciones_.empty()) {
    std::cout << "No hay mediciones disponibles." << std::endl;
    return;
  }
  
  std::map<std::pair<int, int>, std::vector<double>> grupos;
  
  for (const auto& medicion : mediciones_) {
    grupos[{medicion.numero_variables_interes,
            medicion.numero_variables_condicionadas}]
        .push_back(medicion.tiempo_ejecucion);
  }
  
  std::cout << std::setw(12) << "Interés" << std::setw(14) << "Condicionado"
            << std::setw(12) << "Marginal" << std::setw(15) << "Media(us)"
            << std::setw(15) << "Mín(us)" << std::setw(15) << "Máx(us)"
            << std::setw(15) << "DesvEst(us)" << std::endl;
  std::cout << std::string(90, '-') << std::endl;
  
  for (const auto& [clave, tiempos] : grupos) {
    auto estadisticas = computeStatistics(tiempos);
    int marginalizadas = mediciones_[0].numero_variables_marginalizadas;
    
    std::cout << std::setw(10) << clave.first << std::setw(12)
              << clave.second << std::setw(12) << marginalizadas
              << std::setw(15) << std::fixed << std::setprecision(2)
              << estadisticas.media << std::setw(15) << estadisticas.minimo
              << std::setw(15) << estadisticas.maximo << std::setw(15)
              << estadisticas.desviacion_estandar << std::endl;
  }
}

/**
 * @brief Método para generar un informe detallado de los puntos de medición de
 *        rendimiento almacenados
 * @param[in] nombre_archivo: El nombre del archivo donde se guardará el informe
 */
void PerformanceAnalyzer::generateReport(
    const std::string& nombre_archivo) const {
  std::ofstream archivo(nombre_archivo);
  if (!archivo.is_open()) {
    throw std::runtime_error(
        "Error: No se pudo abrir el archivo para escritura " +
        nombre_archivo);
  }
  
  archivo << "Mediciones totales: " << mediciones_.size() << std::endl;
  archivo << std::endl;
  
  // Mostramos estadísticas generales
  std::vector<double> todos_tiempos;
  for (const auto& medicion : mediciones_) {
    todos_tiempos.push_back(medicion.tiempo_ejecucion);
  }
  
  auto estadisticas_generales = computeStatistics(todos_tiempos);
  
  archivo << "Estadísticas generales:\n";
  archivo << "  Tiempo medio de ejecución: " << estadisticas_generales.media
          << " us" << std::endl;
  archivo << "  Tiempo mínimo de ejecución:  " << estadisticas_generales.minimo
          << " us" << std::endl;
  archivo << "  Tiempo máximo de ejecución:  " << estadisticas_generales.maximo
          << " us" << std::endl;
  archivo << "  Desviación estándar:       "
          << estadisticas_generales.desviacion_estandar << " us" << std::endl;
  archivo << std::endl;
  
  archivo << "Análisis detallado por configuración:" << std::endl;
  archivo << std::string(80, '-') << std::endl;
  
  std::map<std::pair<int, int>, std::vector<double>> grupos;
  for (const auto& medicion : mediciones_) {
    grupos[{medicion.numero_variables_interes,
            medicion.numero_variables_condicionadas}]
        .push_back(medicion.tiempo_ejecucion);
  }
  
  for (const auto& [clave, tiempos] : grupos) {
    auto estadisticas = computeStatistics(tiempos);
    archivo << "\nVariables de interés: " << clave.first
            << ", Variables condicionadas: " << clave.second << std::endl;
    archivo << "  Muestras: " << tiempos.size() << std::endl;
    archivo << "  Media: " << estadisticas.media << " us" << std::endl;
    archivo << "  Mín:  " << estadisticas.minimo << " us" << std::endl;
    archivo << "  Máx:  " << estadisticas.maximo << " us" << std::endl;
    archivo << "  DesvEst: " << estadisticas.desviacion_estandar << " us"
            << std::endl;
  }
  
  archivo.close();
}

/**
 * @brief Método para calcular estadísticas básicas (media, mínimo, máximo,
 *        desviación estándar)
 * @param[in] valores: Un vector de valores numéricos para los cuales se
 *                     calcularán las estadísticas
 * @return Una estructura Statistics que contiene la media, mínimo, máximo y
 *         desviación estándar de los valores
 */
Statistics PerformanceAnalyzer::computeStatistics(
    const std::vector<double>& valores) const {
  Statistics estadisticas;
  
  if (valores.empty()) {
    estadisticas.media = estadisticas.minimo = estadisticas.maximo =
        estadisticas.desviacion_estandar = 0.0;
    return estadisticas;
  }
  
  estadisticas.media =
      std::accumulate(valores.begin(), valores.end(), 0.0) / valores.size();
  estadisticas.minimo = *std::min_element(valores.begin(), valores.end());
  estadisticas.maximo = *std::max_element(valores.begin(), valores.end());
  
  double varianza = 0.0;
  for (double v : valores) {
    varianza += (v - estadisticas.media) * (v - estadisticas.media);
  }
  varianza /= valores.size();
  estadisticas.desviacion_estandar = std::sqrt(varianza);
  
  return estadisticas;
}
