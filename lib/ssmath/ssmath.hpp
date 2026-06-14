/**
 * @file ssmath.hpp
 * @brief Agregador principal da biblioteca de matemática SSUGARY MATH.
 * @author Heitor Campos()
 * @details Este arquivo serve como o cabeçalho único (single-header include) para toda 
 * a biblioteca de matemática desenvolvida para a matéria de Computação Gráfica (DIM0451).
 * Ao incluir este arquivo, o motor de renderização tem acesso a todas as estruturas 
 * espaciais e algébricas (Vetores e Matrizes).
 * * Estruturas inclusas:
 * - vec3 (Vetores 3D)
 * - vec4 (Vetores 4D)
 * - mat3 (Matrizes 3x3 para Transformações Lineares)
 * - mat4 (Matrizes 4x4 para Translações e Projeções)
 * * @see https://raytracing.github.io/ 
 * @par Créditos:
 * O design base desta biblioteca (sobrecarga de operadores, inicializações 
 * numéricas e estrutura linear) foi inspirado no livro 
 * "Ray Tracing in One Weekend" de Peter Shirley.
 * @date 15/04/2026()
 */
#pragma once

#ifndef SSUGARY_MATH_CORE_HPP
#define SSUGARY_MATH_CORE_HPP

#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

#include "mat3.hpp"
#include "mat4.hpp"

/**
 * @brief Calcula d distância euclidiana entre pontos 3D.
 * @param p1 O primeiro ponto 3D.
 * @param p2 O segundo ponto 3D.
 * @return A distância entre eles.
 */
template <typename T>
inline double euclidean_distance_3D(const vec3<T> &p1, const vec3<T> &p2) {
  return sqrt(pow(p1.x() - p2.x(), 2) + pow(p1.y() - p2.y(), 2) +
              pow(p1.z() - p2.z(), 2));
};

#endif // SSUGARY_MATH_CORE_HPP
