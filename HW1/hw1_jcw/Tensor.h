#ifndef TENSOR_H
#define TENSOR_H

#include <vector>
#include <memory>
#include <stdexcept>

struct Tensor3D
{
    int H;
    int W;
    int C;
    std::vector<double> data;//real memory for tensor data 

    Tensor3D() : H(0), W(0), C(0) {} //default constructor for empty tensor
    Tensor3D(int h, int w, int c) : H(h), W(w), C(c), data(h * w * c, 0.0) {}

    inline double &at(int h, int w, int c)
    {
        return data[(h * W + w) * C + c];
    }

    inline const double &at(int h, int w, int c) const
    {
        return data[(h * W + w) * C + c];
    }
};

struct Tensor1D
{
    int N;
    std::vector<double> data;

    Tensor1D() : N(0) {}
    Tensor1D(int n) : N(n), data(n, 0.0) {}

    inline double &at(int i)
    {
        return data[i];
    }

    inline const double &at(int i) const
    {
        return data[i];
    }
};

using Tensor3DPtr = std::shared_ptr<Tensor3D>;
using Tensor1DPtr = std::shared_ptr<Tensor1D>;

#endif