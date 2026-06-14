/**
 * @file vec4.hpp
 * @brief Implementação da classe de vetores de 4 dimensões.
 * @author Heitor Campos()
 * @details Esta classe contém as definições e operações matemáticas para vetores 4D,
 *          utilizados no motor de renderização para representar pontos, direções e cores RGBA.
 * @see https://raytracing.github.io/ 
 * @par Créditos:
 *      A lógica fundamental, sobrecarga de operadores e design da classe `vec3` e `vec4`
 *      são baseados nos ensinamentos de Peter Shirley no seu livro 
 *      "Ray Tracing in One Weekend".
 * @date 15/04/2026()
 */

#pragma once

#ifndef SSUGARY_MATH_VECTOR_4_HPP
#define SSUGARY_MATH_VECTOR_4_HPP

#include <cmath>
#include <cstdint>
#include <fstream>

#include "vec3.hpp"

using std::size_t;

/**
  *
  * @class vec4
  * @brief Representa um vetor matemático de 4 componentes.
  * @tparam T O tipo de dado numérico armazenado.
  * @details Esta classe é fundamental para o uso de Coordenadas Homogêneas.
  *          Na computação gráfica, a componente 'w' diferencia pontos geométricos (w=1) 
  *          de vetores de direção (w=0). Também pode ser usada para representar cores RGBA.
  */
template<typename T>
    class vec4 {    //ideia pega de Ray Tracing in One Weekend by 
        private:
            T e[4];
        public:
            /**
              * @brief Construtor padrão. Inicializa como um vetor nulo.
              * @details Utiliza static_cast<T> para garantir compatibilidade com qualquer 
              *          tipo numérico, evitando avisos de conversão implícita (narrowing).
              */
            vec4(){e[0] = static_cast<T>(0); e[1] = static_cast<T>(0); e[2] = static_cast<T>(0); e[3] = static_cast<T>(0);};

            /**
            * @brief Construtor parametrizado
            * @param e0 Valor do componente X (ou R).
            * @param e1 Valor do componente Y (ou G).
            * @param e2 Valor do componente Z (ou B).
            * @param e3 Valor do componente W (ou A)
            */
            constexpr vec4(const T& e0, const T& e1, const T& e2, const T& e3) : e{e0, e1, e2, e3} {};

            /**
            * @brief Construtor parametrizado
            * @param vec Vetor com os componentes XYZW (ou RGBA).
            */
            constexpr vec4(const vec4<T>& vec) : e{vec[0], vec[1], vec[2], vec[3]}{};

            /**
            * @brief Construtor parametrizado
            * @param vec Vetor com os componentes XYZ (ou RGB).
            * @param e3 Valor do componente W (ou A)
            */
            constexpr vec4(const vec3<T>& vec, const T& e3) : e{vec[0], vec[1], vec[2], e3}{};  

            /* Acesso de coordenadas espaciais */
            constexpr T x() const {return e[0];};
            constexpr T y() const {return e[1];};
            constexpr T z() const {return e[2];};
            constexpr vec3<T> xyz() const {return vec3<T>(e[0], e[1], e[2]);};
            constexpr T w() const {return e[3];};

            /* Acesso de cor RGBA*/
            constexpr T r() const {return e[0];};
            constexpr T g() const {return e[1];};
            constexpr T b() const {return e[2];};
            constexpr vec3<T> rgb() const {return vec3<T>(e[0], e[1], e[2]);};
            constexpr T a() const {return e[3];};

            /** @brief Retorna o próprio vetor */
            constexpr const vec4<T>& operator+() const {return *this;};

            /** @brief Retorna vetor invertido */
            constexpr vec4<T> operator-() const {return vec4(-e[0], -e[1], -e[2], -e[3]);};

            /**
              * @brief Operador de acesso constante aos elementos do vetor.
              * @param index Representa a posição acessada.
              * @return Retorna o elemento que está na posição indicada pelo parâmetro para leitura.
              */
            constexpr T operator[](const size_t& index) const {return e[index];};

            /**
              * @brief Operador de acesso aos elementos do vetor.
              * @param index Representa a posição acessada.
              * @return Retorna o elemento que está na posição indicada pelo parâmetro para modificação.
              */
            constexpr T& operator[](const size_t& index){return e[index];};

            /**
              * @brief Soma dois vetores termo a termo.
              * @param v2 O vetor do lado direito da soma
              * @return vec3<T> Novo vetor resultante.
              */
            constexpr vec4<T> operator+(const vec4<T> &v2) const {return vec4<T>(v2.e[0] + e[0], v2.e[1] + e[1], v2.e[2] + e[2], v2.e[3] + e[3]);};

            /**
              * @brief Subtrai dois vetores termo a termo.
              * @param v2 O vetor do lado direito da subtração
              * @return vec3<T> Novo vetor resultante.
              */
            constexpr vec4<T> operator-(const vec4<T> &v2) const{return vec4<T>(e[0] - v2.e[0], e[1] - v2.e[1], e[2] - v2.e[2], e[3] - v2.e[3]);};
            
            /**
              * @brief Multiplica um vetor por um escalar t.
              * @param t Escalar que está multiplicando o vetor.
              * @return vec3<T> Novo vetor resultante.
              */
            constexpr vec4<T> operator*(const T& t) const{return vec4<T>(e[0] * t, e[1] * t, e[2] * t, e[3] * t);};
            
            /**
              * @brief Divide um vetor por um escalar t.
              * @param t Escalar que está dividindo o vetor.
              * @return vec3<T> Novo vetor resultante.
              */
            constexpr vec4<T> operator/(const T& t) const{return vec4<T>(e[0]/t, e[1]/t, e[2]/t, e[3]/t);};
    
            /**
              * @brief Calcula o Produto Escalar entre este vetor 4D e outro.
              * @param v2 O vetor secundário.
              * @return O valor do produto escalar.
              */
            constexpr T dot(const vec4<T>& v2) const{
                return e[0] * v2.e[0] + e[1] * v2.e[1] + e[2] * v2.e[2] + e[3] * v2.e[3];
            }
            
            /**
              * @brief Iguala os valores de um vetor a outro.
              * @param v2 Vetor que envia os valores que serão copiados.
              * @return vec4<T> O vetor atual após se igualar ao segundo vetor.
              */
            constexpr vec4<T>& operator=(const vec4<T> &v2){
                e[0] = v2.e[0];
                e[1] = v2.e[1];
                e[2] = v2.e[2];
                e[3] = v2.e[3];
                return *this;
            }

            /**
              * @brief Verifica se dois vetores são *ESTRITAMENTE* iguais
              * @param v2 Vetor que a comparação será feita
              * @return bool Valor correspondente à comparação
              */
            constexpr bool operator==(const vec4<T> &v2) const {
				return
                (e[0] == v2.e[0]) &
                (e[1] == v2.e[1]) &
                (e[2] == v2.e[2]) &
                (e[3] == v2.e[3]);
            }

            /**
              * @brief Soma os valores de um vetor a outro.
              * @param v2 Vetor que envia os valores que serão somados.
              * @return vec4<T> O vetor atual após a soma do segundo vetor.
              */
            constexpr vec4<T>& operator+=(const vec4<T> &v2){
                e[0] += v2.e[0];
                e[1] += v2.e[1];
                e[2] += v2.e[2];
                e[3] += v2.e[3];
                return *this;
            };

            /**
              * @brief Subtrai os valores de um vetor a outro.
              * @param v2 Vetor que envia os valores que serão subtraídos.
              * @return vec3<T> O vetor atual após a subtração do segundo vetor.
              */
            constexpr vec4<T>& operator-=(const vec4<T> &v2){
                e[0] -= v2.e[0];
                e[1] -= v2.e[1];
                e[2] -= v2.e[2];
                e[3] -= v2.e[3];
                return *this;
            };

            /**
              * @brief Multiplica o vetor com um escalar.
              * @param t Escalar que irá multiplicar o vetor.
              * @return vec4<T> O vetor atual após a multiplicação.
              */
            constexpr vec4<T>& operator*=(const T& t){
                e[0] *= t;
                e[1] *= t;
                e[2] *= t;
                e[3] *= t;
                return *this;
            };

            /**
              * @brief Divide o vetor com um escalar.
              * @param t Escalar que irá dividir o vetor.
              * @return vec4<T> O vetor atual após a divisão.
              */
            constexpr vec4<T>& operator/=(const T& t){
                e[0] /= t;
                e[1] /= t;
                e[2] /= t;
                e[3] /= t;
                return *this;
            };

            /** @brief Retorna o comprimento (magnitude) real do vetor 4D. */
            inline T length() const {return std::sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2] + e[3] * e[3]);};

            /** @brief Retorna o comprimento ao quadrado do vetor 4D. */
            constexpr T sqr_length() const {return e[0] * e[0] + e[1] * e[1] + e[2] * e[2] + e[3] * e[3];};

            /** @brief Normaliza o próprio vetor in-place*/
            inline void mk_unit_vec(){
                float k = 1.0 / this->length();
                e[0] *= k; e[1] *= k; e[2] *= k; e[3] *= k;
            }

            /** @brief Retorna uma cópia normalizada de um vetor 4D fornecido. */
            inline vec4<T> unit_vec(const vec4<T>& v){return v / v.length();};
        };

            /**
              * @brief Multiplica um vetor por um escalar t.
              * @param t Escalar que está multiplicando o vetor.
              * @param v Vetor que está sendo multiplicado.
              * @return vec4<T> Novo vetor resultante.
              */
            template<typename T>
            constexpr vec4<T> operator*(const T& t, const vec4<T> &v) {
                return v * t; 
            }
            /** @brief Permite a leitura do vetor a partir de streams de input. */
            template<typename T>
            inline std::istream& operator>>(std::istream &is, vec4<T> &v){
                is >> v[0] >> v[1] >> v[2] >> v[3];
                return is;
            }

            
            /** @brief Permite a impressão do vetor em streams de output. */
            template<typename T>
            inline std::ostream& operator<<(std::ostream &os, const vec4<T> &v){
                os << v[0] << ' ' << v[1] << ' ' << v[2] << ' ' << v[3];
                return os;
            }
#endif // SSUGARY_MATH_VECTOR_4_HPP
