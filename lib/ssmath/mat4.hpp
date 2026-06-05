/**
 * @file mat4.hpp
 * @brief Implementação da classe de matrizes de 4x4 dimensões.
 * @author Heitor Campos()
 * @details Esta classe contém as definições e operações matemáticas para matrizes 4x4,
 *          servindo tanto para posições espaciais quanto para cálculos de espaços vetoriais.
 * @date 15/04/2026()
 */

#pragma once

#ifndef SSUGARY_MATH_MATRIX_4X4_HPP
#define SSUGARY_MATH_MATRIX_4X4_HPP

#include <algorithm>
#include <array>
#include <cstdint>
#include <initializer_list>
#include <utility>

#include "mat3.hpp"
#include "vec4.hpp"

using size_t = std::size_t;

template <typename T>
    class mat4 {
            std::array<T, 16> mat;
        public:

            /**
              * @brief Construtor padrão que inicializa uma Matriz Identidade 4x4.
              * @details Utiliza static_cast<T> para garantir compatibilidade com qualquer 
              *          tipo numérico, evitando avisos de conversão implícita (narrowing).
              */
            constexpr mat4() {
                T zero = static_cast<T>(0);
                T one  = static_cast<T>(1);

                mat = {
                    one,  zero, zero, zero,
                    zero, one,  zero, zero,
                    zero, zero, one,  zero,
                    zero, zero, zero, one
                };
            }

            /**
              * @brief Constrói uma matriz 4x4 a partir de elementos individuais.
              * @details Os elementos seguem a ordem de linha (Row-Major), onde o primeiro 
              *          dígito indica a linha e o segundo a coluna (ex: e12 é linha 1, coluna 2).
              */
            constexpr mat4(const T& e00, const T& e01, const T& e02, const T& e03,
                           const T& e10, const T& e11, const T& e12, const T& e13,
                           const T& e20, const T& e21, const T& e22, const T& e23,
                           const T& e30, const T& e31, const T& e32, const T& e33)
                  : mat{e00, e01, e02, e03, e10, e11, e12, e13, e20, e21, e22, e23, e30, e31, e32, e33}{};

            /**
              * @brief Constrói uma matriz 4x4 a partir de outra matriz 4x4.
              * @param m2 Matriz que servirá de base para a construção.
              */
            constexpr mat4(const mat4 &m2){
                mat = m2.mat;
            }

            /**
              * @brief Constrói uma matriz 4x4 a partir de uma initializer list.
              * @param list Lista genérica que irá construir a matriz.
              * @details Os elementos seguem a ordem de linha (Row-Major), onde o primeiro 
              *          dígito indica a linha e o segundo a coluna (ex: e12 é linha 1, coluna 2).
              */
            constexpr mat4(const std::initializer_list<T>& list){
                mat.fill(static_cast<T>(0)); 
                size_t count = std::min(static_cast<size_t>(16), list.size());
                std::copy(list.begin(), list.begin() + count, mat.begin());
            };

            /** @brief Retorna a própria matriz */
            constexpr const mat4& operator+() const{return *this;};

            /** @brief Retorna a matriz invertida */
            constexpr mat4 operator-() const{return mat4(-mat[0], -mat[1], -mat[2],
                                                        -mat[3], -mat[4], -mat[5],
                                                        -mat[6], -mat[7], -mat[8], -mat[9], -mat[10], -mat[11], -mat[12], -mat[13], -mat[14], -mat[15]);};

            /**
              * @brief Operador de acesso aos elementos da matriz
              * @param row Representa a linha que será acessada
              * @param col Representa a coluna que será acessaad
              * @return Retorna o elemento que está na posição indicada pelos parâmetros 
              *         (ex: (1, 2) retorna o elemento e12) 
              */
            constexpr T& operator()(const size_t& row, const size_t& col){return mat[4 * row + col];};

            /**
              * @brief Operador de acesso constante aos elementos da matriz
              * @param row Representa a linha que será acessada
              * @param col Representa a coluna que será acessaad
              * @return Retorna o elemento de que está na posição indicada pelos parâmetros 
              *         (ex: (1, 2) retorna o elemento e12) 
              */
            constexpr T operator()(const size_t& row, const size_t& col) const{return mat[4 * row + col];};

            /**
              * @brief Soma duas matrizes termo a termo.
              * @param m2 A matriz do lado direito da soma.
              * @return mat4<T> A nova matriz resultante.
              */
            constexpr mat4 operator+(const mat4 &m2) const {return mat4(mat[0] + m2.mat[0], mat[1] + m2.mat[1], mat[2] + m2.mat[2],
                                                                mat[3] + m2.mat[3], mat[4] + m2.mat[4], mat[5] + m2.mat[5],
                                                                mat[6] + m2.mat[6], mat[7] + m2.mat[7], mat[8] + m2.mat[8],
                                                                mat[9] + m2.mat[9], mat[10] + m2.mat[10], mat[11] + m2.mat[11], mat[12] + m2.mat[12],
                                                                mat[13]+ m2.mat[13], mat[14] + m2.mat[14], mat[15] + m2.mat[15]);};

            /**
              * @brief Subtrai duas matrizes termo a termo.
              * @param m2 A matriz do lado direito da subtração.
              * @return mat4<T> A nova matriz resultante.
              */
            constexpr mat4 operator-(const mat4 &m2) const {return mat4(mat[0] - m2.mat[0], mat[1] - m2.mat[1], mat[2] - m2.mat[2],
                                                                mat[3] - m2.mat[3], mat[4] - m2.mat[4], mat[5] - m2.mat[5],
                                                                mat[6] - m2.mat[6], mat[7] - m2.mat[7], mat[8] - m2.mat[8],
                                                                mat[9] - m2.mat[9], mat[10] - m2.mat[10], mat[11] - m2.mat[11], mat[12] - m2.mat[12],
                                                                mat[13]- m2.mat[13], mat[14] - m2.mat[14], mat[15] - m2.mat[15]);};
                                                            
            /**
            * @brief Multiplica duas matrizes 4x4.
            * @param m2 A matriz do lado direito da multiplicação.
            * @return mat4<T> A nova matriz resultante.
            */
            constexpr mat4 operator*(const mat4& m2) const {
            return mat4(
                mat[0]*m2.mat[0]  + mat[1]*m2.mat[4]  + mat[2]*m2.mat[8]   + mat[3]*m2.mat[12],
                mat[0]*m2.mat[1]  + mat[1]*m2.mat[5]  + mat[2]*m2.mat[9]   + mat[3]*m2.mat[13],
                mat[0]*m2.mat[2]  + mat[1]*m2.mat[6]  + mat[2]*m2.mat[10]  + mat[3]*m2.mat[14],
                mat[0]*m2.mat[3]  + mat[1]*m2.mat[7]  + mat[2]*m2.mat[11]  + mat[3]*m2.mat[15],

                mat[4]*m2.mat[0]  + mat[5]*m2.mat[4]  + mat[6]*m2.mat[8]   + mat[7]*m2.mat[12],
                mat[4]*m2.mat[1]  + mat[5]*m2.mat[5]  + mat[6]*m2.mat[9]   + mat[7]*m2.mat[13],
                mat[4]*m2.mat[2]  + mat[5]*m2.mat[6]  + mat[6]*m2.mat[10]  + mat[7]*m2.mat[14],
                mat[4]*m2.mat[3]  + mat[5]*m2.mat[7]  + mat[6]*m2.mat[11]  + mat[7]*m2.mat[15],

                mat[8]*m2.mat[0]  + mat[9]*m2.mat[4]  + mat[10]*m2.mat[8]  + mat[11]*m2.mat[12],
                mat[8]*m2.mat[1]  + mat[9]*m2.mat[5]  + mat[10]*m2.mat[9]  + mat[11]*m2.mat[13],
                mat[8]*m2.mat[2]  + mat[9]*m2.mat[6]  + mat[10]*m2.mat[10] + mat[11]*m2.mat[14],
                mat[8]*m2.mat[3]  + mat[9]*m2.mat[7]  + mat[10]*m2.mat[11] + mat[11]*m2.mat[15],

                mat[12]*m2.mat[0] + mat[13]*m2.mat[4] + mat[14]*m2.mat[8]  + mat[15]*m2.mat[12],
                mat[12]*m2.mat[1] + mat[13]*m2.mat[5] + mat[14]*m2.mat[9]  + mat[15]*m2.mat[13],
                mat[12]*m2.mat[2] + mat[13]*m2.mat[6] + mat[14]*m2.mat[10] + mat[15]*m2.mat[14],
                mat[12]*m2.mat[3] + mat[13]*m2.mat[7] + mat[14]*m2.mat[11] + mat[15]*m2.mat[15]
                );
            }
            /**
              * @brief Multiplica a matriz por um vetor 4D.
              * @details Transforma o vetor de acordo com as regras da matriz (útil para 
              *          aplicar Rotações, Translações ou Projeções a pontos no espaço).
              * @param v4 Vetor que será transformado.
              * @return vec4<T> Novo vetor contendo o resultado da transformação.
              */
            constexpr vec4<T> operator*(const vec4<T>& v4) const {
                return vec4<T>(
                    mat[0] * v4[0] + mat[1] * v4[1] + mat[2] * v4[2]  + mat[3] * v4[3],
                    mat[4] * v4[0] + mat[5] * v4[1] + mat[6] * v4[2]  + mat[7] * v4[3],
                    mat[8] * v4[0] + mat[9] * v4[1] + mat[10]* v4[2]  + mat[11]* v4[3],
                    mat[12]* v4[0] + mat[13]* v4[1] + mat[14]* v4[2]  + mat[15]* v4[3]
                );
            }

            /**
              * @brief Multiplica uma matriz por uma constante.
              * @param t Constante que está multiplicando a matriz.
              * @return mat4<T> A nova matriz resultante.
              */
            constexpr mat4 operator*(const T& t ) const {return mat4(mat[0] * t, mat[1] * t, mat[2] * t,
                                                                mat[3] * t, mat[4] * t, mat[5] * t,
                                                                mat[6] * t, mat[7] * t, mat[8] * t, mat[9] * t, mat[10] * t,
                                                                mat[11] * t, mat[12] * t, mat[13] * t, mat[14] * t, mat[15] * t);};

            /**
              * @brief Divide uma matriz por uma constante.
              * @param t Constante que está dividindo a matriz.
              * @return mat4<T> A nova matriz resultante.
              */
            constexpr mat4 operator/(const T& t ) const{return mat4(mat[0] / t, mat[1] / t, mat[2] / t,
                                                                mat[3] / t, mat[4] / t, mat[5] / t,
                                                                mat[6] / t, mat[7] / t, mat[8] / t, mat[9] / t, mat[10] / t,
                                                                mat[11] / t, mat[12] / t, mat[13] / t, mat[14] / t, mat[15] / t);};

            /**
              * @brief Iguala os valores de uma matriz a outra.
              * @param m2 Matriz que envia os valores que serão copiados.
              * @return mat4<T> A matriz atual após se igualar a segunda matriz.
              */
            constexpr mat4& operator=(const mat4 &m2){
                for(size_t i{0}; i < 16; i++){
                    mat[i] = m2.mat[i];
                }
                return *this;
            };

            /**
              * @brief Soma os valores de uma matriz a outra.
              * @param m2 Matriz que envia os valores que serão somados.
              * @return mat4<T> A matriz atual após a soma da segunda matriz.
              */
            constexpr mat4& operator+=(const mat4 &m2){
                for(size_t i{0}; i < 16; i++){
                    mat[i] += m2.mat[i];
                }
                return *this;
            };

            /**
              * @brief Subtrai os valores de uma matriz a outra.
              * @param m2 Matriz que envia os valores que serão subtraídos.
              * @return mat4<T> A matriz atual após a subtração da segunda matriz.
              */
            constexpr mat4& operator-=(const mat4 &m2){
                for(size_t i{0}; i < 16; i++){
                    mat[i] -= m2.mat[i];
                }
                return *this;
            };

            /**
              * @brief Multiplica a matriz atual por outra e guarda o resultado em si mesma.
              * @param m2 A matriz do lado direito.
              * @return mat4& Referência para a matriz atual modificada.
              */
            constexpr mat4& operator*=(const mat4& m2) {
                *this = *this * m2;
                return *this;
            }

            /**
              * @brief Multiplica a matriz com um escalar.
              * @param t Escalar que irá multiplicar a matriz.
              * @return mat4<T> A matriz atual após a multiplicação.
              */
            constexpr mat4& operator*=(const T& t){
                for(size_t i{0}; i < 16; i++){
                    mat[i] *= t;
                }
                return *this;
            };

            /**
              * @brief Multiplica a matriz com um vetor de mesma largura.
              * @param v Vetor que irá ser multiplicado pela matriz.
              * @return vec4<T> Vetor obtido após a multiplicação.
              */
            constexpr mat4& operator/=(const T& t){
                for(size_t i{0}; i < 16; i++){
                    mat[i] /= t;
                }
                return *this;
            };

            /**
            * @brief Transforma a matriz em sua forma transposta.
            * @return mat4<T> Matriz transposta gerada a partir da matriz original.
            */
            inline mat4& transpose() {
                std::swap(mat[1], mat[4]);
                std::swap(mat[2], mat[8]);
                std::swap(mat[3], mat[12]);
                
                std::swap(mat[6], mat[9]);
                std::swap(mat[7], mat[13]);
                
                std::swap(mat[11], mat[14]);
                    
                return *this;
            }
            /**
            * @brief Retorna a representação da matriz em vetores de vetores
            * @param flag Define se os vetores representam linhas (ROWS) ou colunas (COLUMNS). Padrão é ROWS.
            * @return vec4<vec4<T>> Representação da matriz original em vetor de vetor.
            */
            constexpr vec4<vec4<T>> toVec(MatrixLayout flag = ROWS) const {
                    if (flag == ROWS) {
                        return vec4<vec4<T>>(
                            vec4<T>(mat[0], mat[1], mat[2], mat[3]),
                            vec4<T>(mat[4], mat[5], mat[6], mat[7]),
                            vec4<T>(mat[8], mat[9], mat[10], mat[11]),
                            vec4<T>(mat[12], mat[13], mat[14], mat[15])
                        );
                    }
                    return vec4<vec4<T>>(
                        vec4<T>(mat[0], mat[4], mat[8], mat[12]),
                        vec4<T>(mat[1], mat[5], mat[9], mat[13]),
                        vec4<T>(mat[2], mat[6], mat[10], mat[14]),
                        vec4<T>(mat[3], mat[7], mat[11], mat[15])
                    );
                }

            /**
              * @brief Retorna a determinante de uma matriz.
              * @note Cálculo utilizando a expansão de Laplace.
              * @return T Valor da determinante da matriz.
              */
            constexpr T det() const {
                T d = mat[0] * (mat[5] * (mat[10] * mat[15] - mat[11] * mat[14]) -
                                mat[6] * (mat[9] * mat[15] - mat[11] * mat[13]) +
                                mat[7] * (mat[9] * mat[14] - mat[10] * mat[13]));

                d -= mat[1] * (mat[4] * (mat[10] * mat[15] - mat[11] * mat[14]) -
                                mat[6] * (mat[8] * mat[15] - mat[11] * mat[12]) +
                                mat[7] * (mat[8] * mat[14] - mat[10] * mat[12]));

                d += mat[2] * (mat[4] * (mat[9] * mat[15] - mat[11] * mat[13]) -
                                mat[5] * (mat[8] * mat[15] - mat[11] * mat[12]) +
                                mat[7] * (mat[8] * mat[13] - mat[9] * mat[12]));

                d -= mat[3] * (mat[4] * (mat[9] * mat[14] - mat[10] * mat[13]) -
                                mat[5] * (mat[8] * mat[14] - mat[10] * mat[12]) +
                                mat[6] * (mat[8] * mat[13] - mat[9] * mat[12]));
            return d;
            }

            /**
              * @brief Calcula a matriz inversa de uma 4x4.4
              * @details Utiliza a matriz adjunta dividida pelo determinante
              * @return mat4<T> A matriz inversa (ou a identidade se o determinante for 0).
              */
            inline mat4<T> inverse() const {
                    T d = det();
                    if (std::abs(d) <= static_cast<T>(1e-8)) return mat4(); 
                    mat4<T> cofactors;

                    cofactors.mat[0]  =  mat3<T>(mat[5], mat[6], mat[7], mat[9], mat[10], mat[11], mat[13], mat[14], mat[15]).det();
                    cofactors.mat[1]  = -mat3<T>(mat[4], mat[6], mat[7], mat[8], mat[10], mat[11], mat[12], mat[14], mat[15]).det();
                    cofactors.mat[2]  =  mat3<T>(mat[4], mat[5], mat[7], mat[8], mat[9],  mat[11], mat[12], mat[13], mat[15]).det();
                    cofactors.mat[3]  = -mat3<T>(mat[4], mat[5], mat[6], mat[8], mat[9],  mat[10], mat[12], mat[13], mat[14]).det();

                    cofactors.mat[4]  = -mat3<T>(mat[1], mat[2], mat[3], mat[9], mat[10], mat[11], mat[13], mat[14], mat[15]).det();
                    cofactors.mat[5]  =  mat3<T>(mat[0], mat[2], mat[3], mat[8], mat[10], mat[11], mat[12], mat[14], mat[15]).det();
                    cofactors.mat[6]  = -mat3<T>(mat[0], mat[1], mat[3], mat[8], mat[9],  mat[11], mat[12], mat[13], mat[15]).det();
                    cofactors.mat[7]  =  mat3<T>(mat[0], mat[1], mat[2], mat[8], mat[9],  mat[10], mat[12], mat[13], mat[14]).det();

                    cofactors.mat[8]  =  mat3<T>(mat[1], mat[2], mat[3], mat[5], mat[6],  mat[7],  mat[13], mat[14], mat[15]).det();
                    cofactors.mat[9]  = -mat3<T>(mat[0], mat[2], mat[3], mat[4], mat[6],  mat[7],  mat[12], mat[14], mat[15]).det();
                    cofactors.mat[10] =  mat3<T>(mat[0], mat[1], mat[3], mat[4], mat[5],  mat[7],  mat[12], mat[13], mat[15]).det();
                    cofactors.mat[11] = -mat3<T>(mat[0], mat[1], mat[2], mat[4], mat[5],  mat[6],  mat[12], mat[13], mat[14]).det();

                    cofactors.mat[12] = -mat3<T>(mat[1], mat[2], mat[3], mat[5], mat[6],  mat[7],  mat[9],  mat[10], mat[11]).det();
                    cofactors.mat[13] =  mat3<T>(mat[0], mat[2], mat[3], mat[4], mat[6],  mat[7],  mat[8],  mat[10], mat[11]).det();
                    cofactors.mat[14] = -mat3<T>(mat[0], mat[1], mat[3], mat[4], mat[5],  mat[7],  mat[8],  mat[9],  mat[11]).det();
                    cofactors.mat[15] =  mat3<T>(mat[0], mat[1], mat[2], mat[4], mat[5],  mat[6],  mat[8],  mat[9],  mat[10]).det();

                    return cofactors.transpose() * (static_cast<T>(1) / d);
                }
};

#endif // SSUGARY_MATH_MATRIX_4X4_HPP