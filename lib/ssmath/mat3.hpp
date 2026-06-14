/**
 * @file mat3.hpp
 * @brief Implementação da classe de matrizes de 3x3 dimensões.
 * @author Heitor Campos()
 * @details Esta classe contém as definições e operações matemáticas para matrizes 3x3,
 *          servindo tanto para posições espaciais quanto para cálculos de espaços vetoriais.
 * @date 15/04/2026()
 */

#pragma once

#ifndef SSUGARY_MATH_MATRIX_3X3_HPP
#define SSUGARY_MATH_MATRIX_3X3_HPP
 
#include <algorithm>
#include <array>
#include <cstdint>
#include <initializer_list>
#include <utility>

#include "vec3.hpp"

using size_t = std::size_t;

#ifndef MATRIX_LAYOUT
#define MATRIX_LAYOUT
/**
 * @brief Define a forma como a matriz é populada a partir de vetores.
 */
enum MatrixLayout{
    ROWS=0, ///< Os vetores fornecidos formam as linhas da matriz
    COLUMNS ///< Os vetores fornecidos formam as colunas da matriz.
};
#endif

template<typename T>
class mat3 {
        private:
            std::array<T, 9> mat;
        public:
            /**
              * @brief Construtor padrão que inicializa uma Matriz Identidade 3x3.
              * @details Utiliza static_cast<T> para garantir compatibilidade com qualquer 
              *          tipo numérico, evitando avisos de conversão implícita (narrowing).
              */
            constexpr mat3() {
                T zero = static_cast<T>(0);
                T one  = static_cast<T>(1);

                mat = {
                    one,  zero, zero, 
                    zero, one,  zero, 
                    zero, zero, one  
                };
            };

            /**
              * @brief Constrói uma matriz 3x3 a partir de elementos individuais.
              * @details Os elementos seguem a ordem de linha (Row-Major), onde o primeiro 
              *          dígito indica a linha e o segundo a coluna (ex: e12 é linha 1, coluna 2).
              */
            constexpr mat3(const T& e00, const T& e01, const T& e02, 
                           const T& e10, const T& e11, const T& e12,
                           const T& e20, const T& e21, const T& e22)
                          : mat{e00, e01, e02, e10, e11, e12, e20, e21, e22}{}

            /**
              * @brief Constrói uma matriz 3x3 a partir de três vetores vec3.
              * @param v1 O primeiro vetor (linha 0 ou coluna 0).
              * @param v2 O segundo vetor  (linha 1 ou coluna 1).
              * @param v3 O terceiro vetor (linha 2 ou coluna 2).
              * @param flag Define se os vetores representam linhas (ROWS) ou colunas (COLUMNS). Padrão é ROWS.
              */
            constexpr mat3(const vec3<T>& v1, const vec3<T>& v2, const vec3<T>& v3, MatrixLayout flag = ROWS){
                switch(flag){
                    case ROWS:
                        mat[0] = v1[0]; mat[1] = v1[1]; mat[2] = v1[2];
                        mat[3] = v2[0]; mat[4] = v2[1]; mat[5] = v2[2];
                        mat[6] = v3[0]; mat[7] = v3[1]; mat[8] = v3[2];
                    break;
                    case COLUMNS:
                        mat[0] = v1[0]; mat[1] = v2[0]; mat[2] = v3[0];
                        mat[3] = v1[1]; mat[4] = v2[1]; mat[5] = v3[1];
                        mat[6] = v1[2]; mat[7] = v2[2]; mat[8] = v3[2];
                    break;
                }
            }
            /**
              * @brief Constrói uma matriz 3x3 a partir de outra matriz 3x3.
              * @param m2 Matriz que servirá de base para a construção.
              */
            constexpr mat3(const mat3 &m2){mat = m2.mat;};

            /**
              * @brief Constrói uma matriz 3x3 a partir de uma initializer list.
              * @param list Lista genérica que irá construir a matriz.
              * @details Os elementos seguem a ordem de linha (Row-Major), onde o primeiro 
              *          dígito indica a linha e o segundo a coluna (ex: e12 é linha 1, coluna 2).
              */
            constexpr mat3(const std::initializer_list<T>& list){
                mat.fill(static_cast<T>(0)); 
    
                size_t count = std::min(static_cast<size_t>(9), list.size());
                std::copy(list.begin(), list.begin() + count, mat.begin());
            
            };


            /** @brief Retorna a própria matriz. */
            inline const mat3& operator+() const {return *this;};

            /** @brief Retorna a matriz invertida. */
            inline mat3 operator-() const {return mat3(     -mat[0], -mat[1], -mat[2],
                                                            -mat[3], -mat[4], -mat[5],
                                                            -mat[6], -mat[7], -mat[8]);};
            /**
              * @brief Operador de acesso aos elementos da matriz
              * @param row Representa a linha que será acessada
              * @param col Representa a coluna que será acessaad
              * @return Retorna o elemento que está na posição indicada pelos parâmetros 
              *         (ex: (1, 2) retorna o elemento e12) 
              */
            constexpr T& operator()(const size_t& row, const size_t& col){return mat[row * 3 + col];}

            /**
              * @brief Operador de acesso constante aos elementos da matriz
              * @param row Representa a linha que será acessada
              * @param col Representa a coluna que será acessaad
              * @return Retorna o elemento de que está na posição indicada pelos parâmetros 
              *         (ex: (1, 2) retorna o elemento e12) 
              */
            constexpr T  operator()(const size_t& row, const size_t& col) const {return mat[row * 3 + col];}

            /**
              * @brief Soma duas matrizes termo a termo.
              * @param m2 A matriz do lado direito da soma.
              * @return mat3<T> A nova matriz resultante.
              */
            constexpr mat3 operator+(const mat3 &m2) const {return mat3(mat[0] + m2.mat[0], mat[1] + m2.mat[1], mat[2] + m2.mat[2],
                                                                    mat[3] + m2.mat[3], mat[4] + m2.mat[4], mat[5] + m2.mat[5],
                                                                    mat[6] + m2.mat[6], mat[7] + m2.mat[7], mat[8] + m2.mat[8]);};
            /**
              * @brief Subtrai duas matrizes termo a termo.
              * @param m2 A matriz do lado direito da subtração.
              * @return mat3<T> A nova matriz resultante.
              */
            constexpr mat3 operator-(const mat3 &m2) const {return mat3(mat[0] - m2.mat[0], mat[1] - m2.mat[1], mat[2] - m2.mat[2],
                                                                    mat[3] - m2.mat[3], mat[4] - m2.mat[4], mat[5] - m2.mat[5],
                                                                    mat[6] - m2.mat[6], mat[7] - m2.mat[7], mat[8] - m2.mat[8]);};
            /**
              * @brief Multiplica duas matrizes 3x3.
              * @details Utiliza o método de "linha por coluna". Combina transformações.
              * @param m2 A matriz do lado direito da multiplicação.
              * @return mat3<T> A nova matriz resultante.
              */
            constexpr mat3 operator*(const mat3& m2) const {
                return mat3(
                    mat[0]*m2.mat[0] + mat[1]*m2.mat[3] + mat[2]*m2.mat[6],
                    mat[0]*m2.mat[1] + mat[1]*m2.mat[4] + mat[2]*m2.mat[7],
                    mat[0]*m2.mat[2] + mat[1]*m2.mat[5] + mat[2]*m2.mat[8],

                    mat[3]*m2.mat[0] + mat[4]*m2.mat[3] + mat[5]*m2.mat[6],
                    mat[3]*m2.mat[1] + mat[4]*m2.mat[4] + mat[5]*m2.mat[7],
                    mat[3]*m2.mat[2] + mat[4]*m2.mat[5] + mat[5]*m2.mat[8],

                    mat[6]*m2.mat[0] + mat[7]*m2.mat[3] + mat[8]*m2.mat[6],
                    mat[6]*m2.mat[1] + mat[7]*m2.mat[4] + mat[8]*m2.mat[7],
                    mat[6]*m2.mat[2] + mat[7]*m2.mat[5] + mat[8]*m2.mat[8]
                );
            }

            /**
              * @brief Multiplica uma matriz por uma constante.
              * @param t Constante que está multiplicando a matriz.
              * @return mat3<T> A nova matriz resultante.
              */
            constexpr mat3 operator*(const T& t ) const {return mat3(mat[0] * t, mat[1] * t, mat[2] * t,
                                                                    mat[3] * t, mat[4] * t, mat[5] * t,
                                                                    mat[6] * t, mat[7] * t, mat[8] * t);};
            /**
              * @brief Divide uma matriz por uma constante.
              * @param t Constante que está dividindo a matriz.
              * @return mat3<T> A nova matriz resultante.
              */
            constexpr mat3 operator/(const T& t ) const {return mat3(mat[0] / t, mat[1] / t, mat[2] / t,
                                                                    mat[3] / t, mat[4] / t, mat[5] / t,
                                                                    mat[6] / t, mat[7] / t, mat[8] / t);};
            
            /**
              * @brief Iguala os valores de uma matriz a outra.
              * @param m2 Matriz que envia os valores que serão copiados.
              * @return mat3<T> A matriz atual após se igualar a segunda matriz.
              */
            constexpr mat3& operator=(const mat3 &m2){
                for(size_t i{0}; i < 9; i++){
                    mat[i] = m2.mat[i];
                }
                return *this;
            };

            /**
              * @brief Soma os valores de uma matriz a outra.
              * @param m2 Matriz que envia os valores que serão somados.
              * @return mat3<T> A matriz atual após a soma da segunda matriz.
              */
            constexpr mat3& operator+=(const mat3 &m2){
                for(size_t i{0}; i < 9; i++){
                    mat[i] += m2.mat[i];
                }
                return *this;
            };

            /**
              * @brief Subtrai os valores de uma matriz a outra.
              * @param m2 Matriz que envia os valores que serão subtraídos.
              * @return mat3<T> A matriz atual após a subtração da segunda matriz.
              */
            constexpr mat3& operator-=(const mat3 &m2){
                for(size_t i{0}; i < 9; i++){
                    mat[i] -= m2.mat[i];
                }
                return *this;
            };

            /**
              * @brief Multiplica duas matrizes 3x3.
              * @details Utiliza o método de "linha por coluna". Combina transformações.
              * @param m2 A matriz do lado direito da multiplicação.
              * @return mat3<T> A nova matriz resultante.
              */
            constexpr mat3& operator*=(const mat3& m2) {
                *this = *this * m2;
                return *this;
            }

            /**
              * @brief Multiplica a matriz com um escalar.
              * @param t Escalar que irá multiplicar a matriz.
              * @return mat3<T> A matriz atual após a multiplicação.
              */
            constexpr mat3& operator*=(const T& t){
                for(size_t i{0}; i < 9; i++){
                    mat[i] *= t;
                }
                return *this;
            };
            
            /**
              * @brief Divide a matriz com um escalar.
              * @param t Escalar que irá Dividir a matriz.
              * @return mat3<T> A matriz atual após a multiplicação.
              */
            constexpr mat3& operator /=(const T& t){
                for(size_t i{0}; i < 9; i++){
                    mat[i] /= t;
                }
                return *this;
            }

            /**
              * @brief Multiplica a matriz com um vetor de mesma largura.
              * @param v Vetor que irá ser multiplicado pela matriz.
              * @return vec3<T> Vetor obtido após a multiplicação.
              */
            constexpr vec3<T> operator*(const vec3<T>& v) const {
                return vec3<T>(
                    mat[0] * v[0] + mat[1] * v[1] + mat[2] * v[2],
                    mat[3] * v[0] + mat[4] * v[1] + mat[5] * v[2],
                    mat[6] * v[0] + mat[7] * v[1] + mat[8] * v[2]
                );
            }

            /**
              * @brief Transforma a matriz em sua forma transposta.
              * @return mat3<T> Matriz transposta gerada a partir da matriz original.
              */
            inline mat3& transpose(){
                std::swap(mat[1], mat[3]);
                std::swap(mat[2], mat[6]);
                std::swap(mat[5], mat[7]);
                return *this;
            }

            /**
              * @brief Retorna a representação da matriz em vetores de vetores
              * @param flag Define se os vetores representam linhas (ROWS) ou colunas (COLUMNS). Padrão é ROWS.
              * @return vec3<vec3<T>> Representação da matriz original em vetor de vetor.
              */
            constexpr vec3<vec3<T>> toVec(MatrixLayout flag = ROWS) const {
                switch(flag){
                    case ROWS:
                    return vec3<vec3<T>>({mat[0], mat[1], mat[2]},
                                         {mat[3], mat[4], mat[5]},
                                         {mat[6], mat[7], mat[8]});
                    case COLUMNS:
                    return vec3<vec3<T>>({mat[0], mat[3], mat[6]},
                                         {mat[1], mat[4], mat[7]},
                                         {mat[2], mat[5], mat[8]});
                    }
            };

            /**
              * @brief Retorna a determinante de uma matriz.
              * @return T Valor da determinante da matriz.
              */
            constexpr T det() const {
                return mat[0] * (mat[4] * mat[8] - mat[5] * mat[7]) -
                       mat[1] * (mat[3] * mat[8] - mat[5] * mat[6]) +
                       mat[2] * (mat[3] * mat[7] - mat[4] * mat[6]);
            }

            /**
              * @brief Calcula a matriz inversa de uma 3x3.
              * @details Utiliza a matriz adjunta dividida pelo determinante.
              * @return mat3<T> A matriz inversa. Retorna a identidade se não tiver inversa.
              */
            inline mat3 inverse() const {
                T determinant = det();
                
                if (std::abs(determinant) <= static_cast<T>(1e-8)) {
                    return mat3(); 
                }
                
                T invDet = static_cast<T>(1) / determinant;

                return mat3(
                     (mat[4] * mat[8] - mat[5] * mat[7]) * invDet,
                    -(mat[1] * mat[8] - mat[2] * mat[7]) * invDet,
                     (mat[1] * mat[5] - mat[2] * mat[4]) * invDet,

                    -(mat[3] * mat[8] - mat[5] * mat[6]) * invDet,
                     (mat[0] * mat[8] - mat[2] * mat[6]) * invDet,
                    -(mat[0] * mat[5] - mat[2] * mat[3]) * invDet,

                     (mat[3] * mat[7] - mat[4] * mat[6]) * invDet,
                    -(mat[0] * mat[7] - mat[1] * mat[6]) * invDet,
                     (mat[0] * mat[4] - mat[1] * mat[3]) * invDet
                );
            }
            
    };

#endif // SSUGARY_MATH_MATRIX_3X3_HPP