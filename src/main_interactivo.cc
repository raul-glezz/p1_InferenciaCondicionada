/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada
 *
 * @file   main_interactivo.cc
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Programa interactivo para probar el motor de inferencia manualmente
 */

#include <iostream>
#include <iomanip>
#include <bitset>
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
 * @brief Inferencia condicional interactiva
 */
void inferencia_interactiva() {
    printSeparator("INFERENCIA CONDICIONAL INTERACTIVA");
    
    // Solicitar número de variables
    int numVariables;
    std::cout << "Ingrese el número de variables (1-10): ";
    std::cin >> numVariables;
    
    if (numVariables < 1 || numVariables > 10) {
        std::cout << "⚠️  Número de variables inválido. Usando N=3 por defecto." << std::endl;
        numVariables = 3;
    }
    
    // Crear distribución conjunta
    BinaryDistribution jointDist(numVariables);
    
    std::cout << "\nGenerando distribución aleatoria con N=" << numVariables << " variables..." << std::endl;
    jointDist.generateRandom();
    
    std::cout << "\nDistribución conjunta generada:" << std::endl;
    jointDist.display();
    
    // Solicitar variables condicionadas
    std::cout << "\n--- CONFIGURACIÓN DE VARIABLES CONDICIONADAS ---" << std::endl;
    std::cout << "¿Cuántas variables desea condicionar? (0-" << numVariables << "): ";
    int numCondicionadas;
    std::cin >> numCondicionadas;
    
    if (numCondicionadas < 0 || numCondicionadas >= numVariables) {
        std::cout << "⚠️  Número inválido. Usando 1 variable condicionada." << std::endl;
        numCondicionadas = 1;
    }
    
    uint64_t maskC = 0;
    uint64_t valC = 0;
    
    for (int i = 0; i < numCondicionadas; i++) {
        int varNum;
        int varVal;
        std::cout << "\nVariable " << (i+1) << " - Indique el número de variable (1-" << numVariables << "): ";
        std::cin >> varNum;
        
        if (varNum < 1 || varNum > numVariables) {
            std::cout << "⚠️  Variable inválida. Saltando..." << std::endl;
            continue;
        }
        
        std::cout << "Variable X" << varNum << " - Indique el valor (0 o 1): ";
        std::cin >> varVal;
        
        if (varVal != 0 && varVal != 1) {
            std::cout << "⚠️  Valor inválido. Usando 0." << std::endl;
            varVal = 0;
        }
        
        // Las variables se numeran desde 1, pero los bits desde 0
        int bitPos = varNum - 1;
        maskC |= (1ULL << bitPos);
        if (varVal == 1) {
            valC |= (1ULL << bitPos);
        }
    }
    
    // Solicitar variables de interés
    std::cout << "\n--- CONFIGURACIÓN DE VARIABLES DE INTERÉS ---" << std::endl;
    std::cout << "¿Cuántas variables de interés desea? (1-" << numVariables << "): ";
    int numInteres;
    std::cin >> numInteres;
    
    if (numInteres < 1 || numInteres > numVariables) {
        std::cout << "⚠️  Número inválido. Usando 1 variable de interés." << std::endl;
        numInteres = 1;
    }
    
    uint64_t maskI = 0;
    
    for (int i = 0; i < numInteres; i++) {
        int varNum;
        std::cout << "\nVariable de interés " << (i+1) << " - Indique el número (1-" << numVariables << "): ";
        std::cin >> varNum;
        
        if (varNum < 1 || varNum > numVariables) {
            std::cout << "⚠️  Variable inválida. Saltando..." << std::endl;
            continue;
        }
        
        int bitPos = varNum - 1;
        maskI |= (1ULL << bitPos);
    }
    
    // Verificar que no haya solapamiento
    if ((maskC & maskI) != 0) {
        std::cout << "\n⚠️  ADVERTENCIA: Hay variables que están en condición e interés simultáneamente." << std::endl;
        std::cout << "    Se continuará con la consulta pero los resultados pueden no ser los esperados.\n" << std::endl;
    }
    
    // Mostrar configuración
    std::cout << "\n--- RESUMEN DE LA CONSULTA ---" << std::endl;
    std::cout << "Variables condicionadas (maskC): ";
    for (int i = 0; i < numVariables; i++) {
        if (maskC & (1ULL << i)) {
            std::cout << "X" << (i+1) << " ";
        }
    }
    std::cout << std::endl;
    
    std::cout << "Valores condicionados (valC):    ";
    for (int i = 0; i < numVariables; i++) {
        if (maskC & (1ULL << i)) {
            std::cout << "X" << (i+1) << "=" << ((valC & (1ULL << i)) ? 1 : 0) << " ";
        }
    }
    std::cout << std::endl;
    
    std::cout << "Variables de interés (maskI):    ";
    for (int i = 0; i < numVariables; i++) {
        if (maskI & (1ULL << i)) {
            std::cout << "X" << (i+1) << " ";
        }
    }
    std::cout << std::endl << std::endl;
    
    std::cout << "Máscaras binarias:" << std::endl;
    std::cout << "  maskC: 0b" << std::bitset<16>(maskC) << std::endl;
    std::cout << "  valC:  0b" << std::bitset<16>(valC) << std::endl;
    std::cout << "  maskI: 0b" << std::bitset<16>(maskI) << std::endl << std::endl;
    
    // Crear motor de inferencia y calcular
    ConditionalInferenceEngine engine(jointDist);
    double* result = engine.prob_cond_bin(maskC, valC, maskI);
    
    // Mostrar resultados
    int numResultados = 1 << __builtin_popcount(maskI);
    std::cout << "--- RESULTADOS DE LA INFERENCIA ---" << std::endl;
    std::cout << std::fixed << std::setprecision(6);
    
    for (int i = 0; i < numResultados; i++) {
        std::cout << "  Estado " << std::bitset<16>(i).to_string().substr(16 - __builtin_popcount(maskI)) 
                  << ": P = " << result[i] << std::endl;
    }
    
    // Verificar normalización
    double sum = 0.0;
    for (int i = 0; i < numResultados; i++) {
        sum += result[i];
    }
    std::cout << "\nSuma de probabilidades: " << sum << " " 
              << (std::abs(sum - 1.0) < 1e-6 ? "✓" : "⚠️") << std::endl;
    
    delete[] result;
}

/**
 * @brief Función principal
 */
int main() {
    char continuar = 's';
    
    std::cout << "\n╔════════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║          PRÁCTICA 1: PROBABILIDAD CONDICIONADA                             ║\n";
    std::cout << "║          Modo Interactivo                                                  ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════════════════╝\n";
    
    do {
        try {
            inferencia_interactiva();
            
            std::cout << "\n¿Desea realizar otra consulta? (s/n): ";
            std::cin >> continuar;
            
        } catch (const std::exception& e) {
            std::cerr << "\n❌ ERROR: " << e.what() << std::endl;
            std::cout << "\n¿Desea intentarlo de nuevo? (s/n): ";
            std::cin >> continuar;
        }
        
    } while (continuar == 's' || continuar == 'S');
    
    std::cout << "\n👋 ¡Hasta pronto!\n" << std::endl;
    return 0;
}
