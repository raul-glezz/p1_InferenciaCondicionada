/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   user_interface.cc
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Definición de la clase UserInterface, que proporciona una interfaz
 *         de línea de comandos para interactuar con el motor de inferencia condicional.
 */

#include <limits>
#include <sstream>
#include "user_interface.h"

/**
 * @brief Ejecuta el bucle principal de la interfaz de usuario, mostrando el
 *        menú y procesando las opciones seleccionadas.
 */
void UserInterface::run() { 
  bool ejecutando = true;
  while (ejecutando) {
    displayMainMenu();
    
    int opcion = readInt("Seleccione una opción", 0, 7);
    std::cout << std::endl;
    
    switch (opcion) {
      case 1:
        loadOrGenerateDistribution();
        break;
      case 2:
        displayCurrentDistribution();
        break;
      case 3:
        executeInference();
        break;
      case 4:
        runPerformanceAnalysis();
        break;
      case 5:
        exportDistribution();
        break;
      case 6:
        displayHelp();
        break;
      case 0:
        ejecutando = false;
        break;
      default:
        std::cout << "Opción no válida. Por favor, elige una opción del "
                     "menú."
                  << std::endl;
    }
    
    if (ejecutando && opcion != 0) {
      std::cout << "\nPulse Enter para continuar...";
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
}

/**
 * @brief Muestra el menú principal de la interfaz de usuario.
 */
void UserInterface::displayMainMenu() const {
  std::cout << "1. Cargar o Generar Distribución" << std::endl;
  std::cout << "2. Mostrar Distribución Actual" << std::endl;
  std::cout << "3. Ejecutar Inferencia Condicional" << std::endl;
  std::cout << "4. Análisis de Rendimiento" << std::endl;
  std::cout << "5. Exportar Distribución a CSV" << std::endl;
  std::cout << "6. Ayuda" << std::endl;
  std::cout << "0. Salir" << std::endl;
}

/**
 * @brief Permite al usuario cargar una distribución desde un archivo CSV o
 *        generar una distribución aleatoria.
 */
void UserInterface::loadOrGenerateDistribution() {
  std::cout << "1. Cargar desde archivo CSV" << std::endl;
  std::cout << "2. Generar distribución aleatoria\n";
  int opcion = readInt("Seleccione opción", 1, 2);
  
  if (opcion == 1) {
    std::string nombre_archivo = readString(
        "\nIngrese el nombre del archivo CSV");
    
    try {
      distribucion_ = std::make_unique<BinaryDistribution>(nombre_archivo);
      motor_ =
          std::make_unique<ConditionalInferenceEngine>(*distribucion_);
      
      std::cout << "  Variables: " << distribucion_->getNumberVariables()
                << "\n";
      std::cout << "  Estados: " << distribucion_->getStateSpaceSize()
                << "\n";
      
      // Si la distribución no es válida, la normalizamos
      if (!distribucion_->isValid()) {
        distribucion_->normalize();
      }
    } catch (const std::exception& excepcion) {
      std::cout << "\nError al cargar la distribución: " << excepcion.what()
                << std::endl;
      distribucion_.reset();
      motor_.reset();
    }
  } else {
    int numero_variables = readInt("\nNúmero de variables (1-20)", 1, 20);
    
    try {
      distribucion_ = std::make_unique<BinaryDistribution>(numero_variables);
      distribucion_->generateRandom();
      motor_ =
          std::make_unique<ConditionalInferenceEngine>(*distribucion_);
      
      std::cout << "  Variables: " << numero_variables << std::endl;
      std::cout << "  Estados: " << distribucion_->getStateSpaceSize()
                << std::endl;
    } catch (const std::exception& excepcion) {
      std::cout << "\nError generando la distribución: " << excepcion.what()
                << std::endl;
      distribucion_.reset();
      motor_.reset();
    }
  }
}

/**
 * @brief Crea una consulta condicional a partir de la entrada del usuario,
 *        solicitando las variables de interés y condicionadas.
 * @return Un puntero único a la consulta creada, o nullptr si no se pudo crear.
 */
std::unique_ptr<ConditionalQuery> UserInterface::createQuery() {
  if (!checkDistributionLoaded()) {
    return nullptr;
  }
  
  int numero_variables = distribucion_->getNumberVariables();
  auto consulta = std::make_unique<ConditionalQuery>(numero_variables);
  
  std::cout << "\n--- Variables de interés ---\n";
  int numero_variables_interes =
      readInt("Introduzca el número de variables de interés: ", 1,
              numero_variables - 1);
  
  for (int i = 0; i < numero_variables_interes; ++i) {
    int indice_variable =
        readInt("  Índice de variable (1-" + std::to_string(numero_variables) +
                    ")",
                1, numero_variables) -
        1;
    try {
      consulta->addInterestVariable(indice_variable);
    } catch (const std::exception& excepcion) {
      std::cout << "  Error: " << excepcion.what() << std::endl;
      i--;
    }
  }
  
  std::cout << "\n--- Variables condicionadas ---\n";
  if (readConfirmation("¿Agregar variables condicionadas?")) {
    int maximo_variables_condicionadas =
        numero_variables - numero_variables_interes;
    int numero_variables_condicionadas = readInt(
        "Introduzca el número de variables condicionadas: ", 0,
        maximo_variables_condicionadas);
    
    for (int i = 0; i < numero_variables_condicionadas; ++i) {
      int indice_variable = readInt(
          "  Índice de variable (1-" + std::to_string(numero_variables) + ")",
          1, numero_variables) - 1;
      int valor = readInt("  Valor (0 o 1)", 0, 1);
      
      try {
        consulta->addConditionedVariable(indice_variable, valor);
      } catch (const std::exception& excepcion) {
        std::cout << "  Error: " << excepcion.what() << std::endl;
        i--;
      }
    }
  }
  
  consulta->computeMasks();
  std::cout << std::endl;
  std::cout << consulta->toString();
  
  return consulta;
}

/**
 * @brief Ejecuta el proceso de inferencia condicional utilizando la consulta
 *        creada por el usuario, mostrando los resultados y métricas de
 *        ejecución.
 */
void UserInterface::executeInference() {
  if (!checkDistributionLoaded()) {
    return;
  }
  
  auto consulta = createQuery();
  if (!consulta || !consulta->isValid()) {
    std::cout << "\nConsulta inválida.\n";
    return;
  }
  
  try {
    auto resultado = motor_->computeConditional(*consulta);
    
    resultado.distribucion->display();
    
    std::cout << "\n--- Métricas de ejecución ---\n";
    std::cout << "  Tiempo de ejecución: " << std::fixed
              << std::setprecision(2) << resultado.tiempo_ejecucion
              << " μs" << std::endl;
    std::cout << "  Estados evaluados: " << resultado.estados_evaluados
              << std::endl;
    std::cout << "  Variables de interés: "
              << consulta->getNumberInterestVariables() << std::endl;
    std::cout << "  Variables condicionadas: "
              << consulta->getNumberConditionedVariables() << std::endl;
    std::cout << "  Variables marginalizadas: "
              << consulta->getNumberMarginalizedVariables() << std::endl;
    
    if (readConfirmation("\n¿Guardar resultado en CSV?")) {
      std::string nombre_archivo =
          readString("Nombre del archivo de salida");
      resultado.distribucion->exportToCSV(nombre_archivo);
    }
  } catch (const std::exception& excepcion) {
    std::cout << "\nError durante la inferencia: " << excepcion.what()
              << std::endl;
  }
}

/**
 * @brief Ejecuta un análisis de rendimiento exhaustivo, probando diferentes
 *        configuraciones de variables de interés y condicionadas, mostrando
 *        estadísticas y permitiendo exportar los resultados.
 */
void UserInterface::runPerformanceAnalysis() {
  if (!checkDistributionLoaded()) {
    return;
  }
  
  int numero_variables = distribucion_->getNumberVariables();
  
  int maximo_variables_interes =
      readInt("Máximo de variables de interés a probar", 1,
              numero_variables - 1);
  int maximo_variables_condicionadas = readInt(
      "Máximo de variables condicionadas a probar", 0,
      numero_variables - maximo_variables_interes - 1);
  int repeticiones = readInt("Repeticiones por configuración", 1, 20);
  
  analizador_->clear();
  analizador_->runAnalysis(*distribucion_, maximo_variables_interes,
                          maximo_variables_condicionadas, repeticiones);
  
  std::cout << std::endl;
  analizador_->displayStatistics();
  
  if (readConfirmation("\n¿Exportar datos a CSV?")) {
    std::string nombre_archivo =
        readString("Nombre del archivo CSV de salida");
    analizador_->exportToCSV(nombre_archivo);
  }
  
  if (readConfirmation("\n¿Generar el informe completo?")) {
    std::string nombre_archivo =
        readString("Nombre del archivo de reporte de salida");
    analizador_->generateReport(nombre_archivo);
  }
}

/**
 * @brief Muestra la distribución actual por pantalla, verificando primero que
 *        una distribución esté cargada.
 */
void UserInterface::displayCurrentDistribution() {
  if (!checkDistributionLoaded()) {
    return;
  }
  distribucion_->display();
}

/**
 * @brief Exporta la distribución actual a un archivo CSV, verificando primero
 *        que una distribución esté cargada y manejando posibles errores durante
 *        la exportación.
 */
void UserInterface::exportDistribution() {
  if (!checkDistributionLoaded()) {
    return;
  }
  
  std::string nombre_archivo =
      readString("Nombre del archivo de salida");
  
  try {
    distribucion_->exportToCSV(nombre_archivo);
  } catch (const std::exception& excepcion) {
    std::cout << "Error al exportar: " << excepcion.what() << std::endl;
  }
}

/**
 * @brief Muestra una sección de ayuda detallada, explicando los conceptos clave, el formato de los archivos CSV y el algoritmo utilizado para calcular las probabilidades condicionales.
 */
void UserInterface::displayHelp() const {
  std::cout << "Este programa calcula probabilidades condicionales en distribuciones binarias discretas usando marginalización." << std::endl;
  
  std::cout << "Conceptos Clave:\n";
  std::cout << "  • Variables: Indexadas de 1 a N (internamente de 0 a N-1)" << std::endl;
  std::cout << "  • Variables de Interés: Variables cuya distribución se desea" << std::endl;
  std::cout << "  • Variables Condicionadas: Variables con valores fijos" << std::endl;
  std::cout << "  • Variables Marginalizadas: Todas las demás variables (sumadas)" << std::endl;
  std::cout << std::endl;
  
  std::cout << "Formato CSV:" << std::endl;
  std::cout << "  binary_mask,probability" << std::endl;
  std::cout << "  Ejemplo para N=3:" << std::endl;
  std::cout << "    000,0.10" << std::endl;
  std::cout << "    001,0.15" << std::endl;
  std::cout << "    ..." << std::endl;
  std::cout << std::endl;
  
  std::cout << "Algoritmo:" << std::endl;
  std::cout << "  P(X_I | X_C = c) = Σ P(X_I, X_C = c, X_M) / P(X_C = c)" << std::endl;
  std::cout << "  donde X_M son las variables marginalizadas." << std::endl;
  std::cout << std::endl;
}

/**
 * @brief Lee un número entero del usuario, mostrando un mensaje de error si la
 *        entrada no es válida o está fuera de rango, y repitiendo hasta obtener
 *        una entrada correcta.
 * @param[in] mensaje: El mensaje a mostrar al solicitar la entrada.
 * @param[in] valor_minimo: El valor mínimo permitido (inclusive).
 * @param[in] valor_maximo: El valor máximo permitido (inclusive).
 * @return El número entero ingresado por el usuario, garantizado para estar
 *         dentro del rango
 */
int UserInterface::readInt(const std::string& mensaje, int valor_minimo,
                            int valor_maximo) {
  int valor;
  while (true) {
    std::cout << mensaje << " [" << valor_minimo << "-" << valor_maximo
              << "]: ";
    std::cin >> valor;
    
    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Entrada inválida. Por favor, ingrese un número."
                << std::endl;
    } else if (valor < valor_minimo || valor > valor_maximo) {
      std::cout << "Valor fuera de rango." << std::endl;
    } else {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return valor;
    }
  }
}

/**
 * @brief Lee una cadena de texto del usuario, mostrando un mensaje de
 *        solicitud.
 * @param[in] mensaje: El mensaje a mostrar al solicitar la entrada.
 * @return La cadena de texto ingresada por el usuario.
 */
std::string UserInterface::readString(const std::string& mensaje) {
  std::string valor;
  std::cout << mensaje << ": ";
  std::getline(std::cin, valor);
  return valor;
}

/**
 * @brief Lee una confirmación del usuario, mostrando un mensaje de solicitud y
 *        validando la entrada.
 * @param[in] mensaje: El mensaje a mostrar al solicitar la confirmación.
 * @return true si el usuario confirma (ingresa 's' o 'S'), false en caso
 *         contrario.
 */
bool UserInterface::readConfirmation(const std::string& mensaje) {
  std::cout << mensaje << " (s/n): ";
  std::string respuesta;
  std::getline(std::cin, respuesta);
  return !respuesta.empty() &&
         (respuesta[0] == 's' || respuesta[0] == 'S');
}

/**
 * @brief Verifica que una distribución esté cargada antes de realizar
 *        operaciones que dependan de ella, mostrando un mensaje de error si no
 *        es así.
 * @return true si una distribución está cargada, false en caso contrario.
 */
bool UserInterface::checkDistributionLoaded() const {
  if (!distribucion_) {
    std::cout << "No hay distribución cargada. Por favor, cargue o genere una "
                 "primero."
              << std::endl;
    return false;
  }
  return true;
}
