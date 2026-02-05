/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada
 *
 * @file   main.cc
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Programa principal para demostrar el funcionamiento del motor de inferencia
 *         condicional sobre distribuciones binarias (hasta punto 3 del guión)
 */

#include <iostream>
#include <iomanip>
#include <memory>
#include "distribution/binary_distribution/binary_distribution.h"
#include "conditional_inference_engine/conditional_inference_engine.h"

/**
 * @brief Imprime el separador de secciones
 */
void printSeparator(const std::string& title) {
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << std::string(80, '=') << std::endl << std::endl;
}

/**
 * @brief Ejemplo 1: Creación de una distribución binaria simple
 * Punto 1 del guión: Construcción básica
 */
void ejemplo1_construccion_basica() {
    printSeparator("EJEMPLO 1: Construcción de distribución binaria (N=2)");
    
    // Crear distribución con 2 variables: X1, X2
    BinaryDistribution dist(2);
    
    std::cout << "Distribución creada con N=2 variables" << std::endl;
    std::cout << "Espacio de estados: 2^" << dist.getNumberVariables() 
              << " = " << dist.getStateSpaceSize() << " estados" << std::endl << std::endl;
    
    // Configurar probabilidades manualmente
    // Estado 00: P(X1=0, X2=0) = 0.1
    // Estado 01: P(X1=0, X2=1) = 0.2
    // Estado 10: P(X1=1, X2=0) = 0.3
    // Estado 11: P(X1=1, X2=1) = 0.4
    
    std::cout << "Asignando probabilidades:" << std::endl;
    std::cout << "  P(X1=0, X2=0) = 0.1" << std::endl;
    std::cout << "  P(X1=0, X2=1) = 0.2" << std::endl;
    std::cout << "  P(X1=1, X2=0) = 0.3" << std::endl;
    std::cout << "  P(X1=1, X2=1) = 0.4" << std::endl << std::endl;
    
    dist.setProbability(0b00, 0.1);  // Estado 00
    dist.setProbability(0b01, 0.2);  // Estado 01
    dist.setProbability(0b10, 0.3);  // Estado 10
    dist.setProbability(0b11, 0.4);  // Estado 11
    
    // Mostrar distribución
    dist.display();
    
    // Validar distribución
    std::cout << "¿La distribución es válida? " << (dist.isValid() ? "SÍ" : "NO") << std::endl;
}

/**
 * @brief Ejemplo 2: Normalización de distribuciones
 * Punto 2 del guión: Normalización y generación aleatoria
 */
void ejemplo2_normalizacion() {
    printSeparator("EJEMPLO 2: Generación aleatoria y propiedades de distribuciones");
    
    BinaryDistribution dist(3);
    
    std::cout << "Creando distribución con N=3 variables" << std::endl;
    std::cout << "Generando distribución aleatoria (ya normalizada automáticamente)..." << std::endl << std::endl;
    
    // El método generateRandom() ya normaliza automáticamente
    dist.generateRandom();
    
    std::cout << "Distribución generada:" << std::endl;
    dist.display();
    std::cout << "¿Es válida? " << (dist.isValid() ? "SÍ" : "NO") << std::endl << std::endl;
    
    // Demostrar propiedades
    std::cout << "Propiedades de la distribución:" << std::endl;
    std::cout << "  - Número de variables: " << dist.getNumberVariables() << std::endl;
    std::cout << "  - Espacio de estados: " << dist.getStateSpaceSize() << std::endl;
    std::cout << "  - Probabilidad de estado 000: " << dist.getProbability(0b000) << std::endl;
    std::cout << "  - Probabilidad de estado 111: " << dist.getProbability(0b111) << std::endl;
}

/**
 * @brief Ejemplo 3: Inferencia condicional básica
 * Punto 3 del guión: Cálculo de distribuciones condicionales
 */
void ejemplo3_inferencia_condicional() {
    printSeparator("EJEMPLO 3: Inferencia Condicional P(X1 | X2=1)");
    
    // Crear distribución conjunta P(X1, X2)
    BinaryDistribution jointDist(2);
    
    std::cout << "Distribución conjunta P(X1, X2):" << std::endl << std::endl;
    
    jointDist.setProbability(0b00, 0.2);  // P(X1=0, X2=0) = 0.2
    jointDist.setProbability(0b01, 0.3);  // P(X1=0, X2=1) = 0.3
    jointDist.setProbability(0b10, 0.1);  // P(X1=1, X2=0) = 0.1
    jointDist.setProbability(0b11, 0.4);  // P(X1=1, X2=1) = 0.4
    
    jointDist.display();
    
    // Crear motor de inferencia
    ConditionalInferenceEngine engine(jointDist);
    
    std::cout << "\nCalculando P(X1 | X2=1)..." << std::endl;
    std::cout << "Esto significa: probabilidad de X1 dado que observamos X2=1\n" << std::endl;
    
    // Configurar la consulta:
    // - Condición: X2=1 (bit 0 en la máscara)
    // - Interés: X1 (bit 1 en la máscara)
    
    uint64_t maskC = 0b01;  // Máscara de condición: X2 (bit 0)
    uint64_t valC  = 0b01;  // Valor de condición: X2=1
    uint64_t maskI = 0b10;  // Máscara de interés: X1 (bit 1)
    
    std::cout << "Parámetros de la consulta:" << std::endl;
    std::cout << "  maskC (variables condicionadas): 0b01 -> X2" << std::endl;
    std::cout << "  valC  (valores condicionados):    0b01 -> X2=1" << std::endl;
    std::cout << "  maskI (variables de interés):     0b10 -> X1" << std::endl << std::endl;
    
    double* result = engine.prob_cond_bin(maskC, valC, maskI);
    
    std::cout << "Resultado de la inferencia condicional:" << std::endl;
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "  P(X1=0 | X2=1) = " << result[0] << std::endl;
    std::cout << "  P(X1=1 | X2=1) = " << result[1] << std::endl << std::endl;
    
    // Verificación manual
    double p_x2_1 = jointDist.getProbability(0b01) + jointDist.getProbability(0b11);
    double p_x1_0_x2_1 = jointDist.getProbability(0b01) / p_x2_1;
    double p_x1_1_x2_1 = jointDist.getProbability(0b11) / p_x2_1;
    
    std::cout << "Verificación manual:" << std::endl;
    std::cout << "  P(X2=1) = P(X1=0,X2=1) + P(X1=1,X2=1) = " 
              << jointDist.getProbability(0b01) << " + " 
              << jointDist.getProbability(0b11) << " = " << p_x2_1 << std::endl;
    std::cout << "  P(X1=0 | X2=1) = P(X1=0,X2=1) / P(X2=1) = " 
              << jointDist.getProbability(0b01) << " / " << p_x2_1 
              << " = " << p_x1_0_x2_1 << std::endl;
    std::cout << "  P(X1=1 | X2=1) = P(X1=1,X2=1) / P(X2=1) = " 
              << jointDist.getProbability(0b11) << " / " << p_x2_1 
              << " = " << p_x1_1_x2_1 << std::endl;
    
    delete[] result;
}

/**
 * @brief Ejemplo 4: Inferencia condicional con 3 variables
 * Punto 3 del guión: Caso más complejo
 */
void ejemplo4_inferencia_multiple() {
    printSeparator("EJEMPLO 4: Inferencia con 3 variables - P(X1,X2 | X3=1)");
    
    // Crear distribución conjunta P(X1, X2, X3)
    BinaryDistribution jointDist(3);
    
    std::cout << "Distribución conjunta P(X1, X2, X3):" << std::endl << std::endl;
    
    // Asignar probabilidades de forma aleatoria y normalizar
    jointDist.generateRandom();
    jointDist.display();
    
    // Crear motor de inferencia
    ConditionalInferenceEngine engine(jointDist);
    
    std::cout << "\nCalculando P(X1, X2 | X3=1)..." << std::endl;
    std::cout << "Condición: X3=1" << std::endl;
    std::cout << "Variables de interés: X1, X2\n" << std::endl;
    
    uint64_t maskC = 0b001;  // X3
    uint64_t valC  = 0b001;  // X3=1
    uint64_t maskI = 0b110;  // X1 y X2
    
    double* result = engine.prob_cond_bin(maskC, valC, maskI);
    
    std::cout << "Resultado de la inferencia condicional:" << std::endl;
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "  P(X1=0, X2=0 | X3=1) = " << result[0b00] << std::endl;
    std::cout << "  P(X1=0, X2=1 | X3=1) = " << result[0b01] << std::endl;
    std::cout << "  P(X1=1, X2=0 | X3=1) = " << result[0b10] << std::endl;
    std::cout << "  P(X1=1, X2=1 | X3=1) = " << result[0b11] << std::endl << std::endl;
    
    // Verificar que suma 1
    double sum = result[0] + result[1] + result[2] + result[3];
    std::cout << "Suma de probabilidades: " << sum << " ✓" << std::endl;
    
    delete[] result;
}

/**
 * @brief Ejemplo 5: Cargar distribución desde archivo CSV
 */
void ejemplo5_carga_desde_csv() {
    printSeparator("EJEMPLO 5: Carga desde archivo CSV");
    
    std::cout << "Primero exportamos una distribución a CSV..." << std::endl << std::endl;
    
    // Crear y exportar distribución
    BinaryDistribution dist1(2);
    dist1.setProbability(0b00, 0.15);
    dist1.setProbability(0b01, 0.35);
    dist1.setProbability(0b10, 0.25);
    dist1.setProbability(0b11, 0.25);
    
    std::string filename = "test_distribution.csv";
    dist1.exportToCSV(filename);
    std::cout << "Distribución exportada a: " << filename << std::endl << std::endl;
    
    // Cargar desde archivo
    std::cout << "Ahora cargamos desde el archivo CSV..." << std::endl << std::endl;
    BinaryDistribution dist2(filename);
    
    dist2.display();
    std::cout << "¿La distribución cargada es válida? " 
              << (dist2.isValid() ? "SÍ" : "NO") << std::endl;
}

/**
 * @brief Función principal
 */
int main() {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║          PRÁCTICA 1: PROBABILIDAD CONDICIONADA                             ║\n";
    std::cout << "║          Demostración del Motor de Inferencia                              ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════════════════╝\n";
    
    try {
        // Ejecutar ejemplos
        ejemplo1_construccion_basica();
        ejemplo2_normalizacion();
        ejemplo3_inferencia_condicional();
        ejemplo4_inferencia_multiple();
        ejemplo5_carga_desde_csv();
        
        printSeparator("FIN DE LA DEMOSTRACIÓN");
        std::cout << "Todos los ejemplos se ejecutaron correctamente ✓\n" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "\n❌ ERROR: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
