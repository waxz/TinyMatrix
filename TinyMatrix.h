// LEGAL SHIT HERE

#pragma once

#include <cstring>
#include <iostream>

namespace TinyMatrix {
    template<class T, size_t M, size_t N>
    struct Matrix {
    public:
        Matrix(T val = 0) {
            for (size_t i = 0; i < M; ++i) {
                for (size_t j = 0; j < N; ++j)
                    this->data[i][j] = val;
            }
        }
        Matrix(T data[M][N]) {
            for (size_t i = 0; i < M; ++i) {
                for (size_t j = 0; j < N; ++j)
                    this->data[i][j] = data[i][j];
            }
        }

        Matrix<T,1,N> GetRow(int r) {
            Matrix<T,1,N> ret;
            for (size_t i = 0; i < N; ++i)
                ret(0,i) = this->data[r][i];
            return ret;
        }

        Matrix<T,M,1> GetColumn(int c) {
            Matrix<T,M,1> ret;
            for (size_t i = 0; i < M; ++i)
                ret(i,0) = this->data[i][c];
            return ret;
        }

        void SetRow(int r, T row[N]) {
            for (size_t i = 0; i < N; ++i)
                this->data[r][i] = row[i];
        }

        void SetRow(int r, Matrix<T,1,N> row) {
            for (size_t i = 0; i < N; ++i)
                this->data[r][i] = row(0,i);
        }

        void SetColumn(int c, T column[M]) {
            for (size_t i = 0; i < M; ++i)
                this->data[i][c] = column[i];
        }

        void SetColumn(int c, Matrix<T,M,1> column) {
            for (size_t i = 0; i < M; ++i)
                this->data[i][c] = column(i,0);
        }

        T * Raw() {
            return (T*)this->data;
        }

        bool IsSquare() {
            return (M == N);
        }

        size_t NumRows() {
            return M;
        }
        
        size_t NumColumns() {
            return N;
        }

        Matrix<T,N,M> Transpose() {
            Matrix<T,N,M> ret;
            for (size_t r = 0; r < M; ++r) {
                for (size_t c = 0; c < N; ++c) {
                    ret(c,r) = this->data[r][c];
                }
            }
            return ret;
        }

        static Matrix<T,M,M> Identity() {
            Matrix<T,M,M> ret;
            for (size_t r = 0; r < M; ++r) {
                for (size_t c = 0; c < N; ++c) {
                    if (r == c)
                        ret(r,c) = 1;
                    else
                        ret(r,c) = 0;
                }
            }
            return ret;
        }

        Matrix<T,M,N>& operator=(const Matrix<T,M,N>& other) { // copy
            if (this != &other) {
                for (size_t r = 0; r < M; ++r) {
                    for (size_t c = 0; c < N; ++c)
                        this->data[r][c] = other(r,c);
                }
            }
            return *this;
        }

        friend bool operator==(const Matrix<T,M,N>& lhs, const Matrix<T,M,N>& rhs) {
            for (size_t r = 0; r < M; ++r) {
                for (size_t c = 0; c < N; ++c) {
                    if (lhs(r,c) != rhs(r,c))
                        return false;
                }
            }
            return true;
        }
        friend bool operator!=(const Matrix<T,M,N>& lhs, const Matrix<T,M,N>& rhs) { return !(lhs == rhs); }

        T& operator() (size_t r, size_t c) { return this->data[r][c]; }
        const T& operator() (size_t r, size_t c) const { return this->data[r][c]; }

        Matrix<T,M,N>& operator+=(const Matrix<T,M,N>& rhs) {
            for (size_t r = 0; r < M; ++r) {
                for (size_t c = 0; c < N; ++c)
                    this->data[r][c] += rhs(r,c);
            }
            return *this;
        }
        friend Matrix<T,M,N> operator+(Matrix<T,M,N> lhs, const Matrix<T,M,N>& rhs) {
            return (lhs += rhs);
        }

        Matrix<T,M,N>& operator-=(const Matrix<T,M,N>& rhs) {
            for (size_t r = 0; r < M; ++r) {
                for (size_t c = 0; c < N; ++c)
                    this->data[r][c] -= rhs(r,c);
            }
            return *this;
        }
        friend Matrix<T,M,N> operator-(Matrix<T,M,N> lhs, const Matrix<T,M,N>& rhs) {
            return (lhs -= rhs);
        }

        Matrix<T,M,N>& operator*=(const T& scalar) {
            for (size_t r = 0; r < M; ++r) {
                for (size_t c = 0; c < N; ++c)
                    this->data[r][c] *= scalar;
            }
            return *this;
        }
        friend Matrix<T,M,N> operator*(Matrix<T,M,N> lhs, const T& scalar) {
            return (lhs *= scalar);
        }
        friend Matrix<T,M,N> operator*(const T& scalar, Matrix<T,M,N> rhs) {
            return (rhs *= scalar);
        }

        template<size_t P>
        friend Matrix<T,M,P> operator*(Matrix<T,M,N> lhs, const Matrix<T,N,P>& rhs) {
            Matrix<T,M,P> ret;

            for (size_t r = 0; r < M; ++r) {
                for (size_t c1 = 0; c1 < P; ++c1) {
                    for (size_t c2 = 0; c2 < N; ++c2)
                        ret(r, c1) += lhs(r,c2) * rhs(c2,c1);
                }
            }

            return ret;
        }

        friend std::ostream& operator<<(std::ostream& os, const Matrix<T,M,N> mat) {
            for (size_t r = 0; r < M; ++r) {
                os << "[ ";
                for (size_t c = 0; c < N; ++c)
                    os << mat(r,c) << " ";
                os << "] " << std::endl;
            }
            return os;
        }
    private:
        T data[M][N];
    };

#ifdef TINYMATRIX_CPP11
    template<class T, size_t N>
    using SquareMatrix = Matrix<T, N, N>;
    template<class T, size_t N>
    using Vector = Matrix<T, N, 1>;
#endif

    template<class T, size_t N>
    T DotProduct(Matrix<T,N,1> a, Matrix<T,N,1> b) {
        return (a.Transpose() * b)(0,0);
    }

    template<class T>
    Matrix<T,3,1> CrossProduct(Matrix<T,3,1> a, Matrix<T,3,1> b) {
        Matrix<T,3,1> ret;
        ret(0,0) = (a(1,0) * b(2,0) - a(2,0)*b(1,0));
        ret(1,0) = -(a(0,0) * b(2,0) - a(2,0)*b(0,0));
        ret(2,0) = (a(0,0) * b(1,0) - a(1,0)*b(0,0));
        return ret;
    }
}