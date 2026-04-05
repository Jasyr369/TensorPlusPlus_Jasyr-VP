#pragma once

#include <vector>
#include <stdexcept>
#include <random>
#include <cmath>
#include <numeric>
#include <algorithm>

// ==========================================
// 1. INTERFAZ DE TRANSFORMACIÓN
// ==========================================
class Tensor;

class TensorTransform {
public:
    [[nodiscard]] virtual Tensor apply(const Tensor& t) const = 0;
    virtual ~TensorTransform() = default;
};

// ==========================================
// 2. CLASE TENSOR
// ==========================================
class Tensor {
private:
    std::vector<size_t> shape_;
    double* data_;
    size_t size_;

    [[nodiscard]] static size_t calculate_size(const std::vector<size_t>& shape) {
        if (shape.empty()) return 0;
        size_t total = 1;
        for (size_t dim : shape) total *= dim;
        return total;
    }

public:
    // Constructor privado para inicializar memoria
    explicit Tensor(const std::vector<size_t>& shape) : shape_(shape) {
        if (shape.size() > 3) throw std::invalid_argument("Maximo 3 dimensiones permitidas.");
        size_ = calculate_size(shape);
        data_ = size_ > 0 ? new double[size_]() : nullptr;
    }

    // Constructor por defecto
    Tensor() : shape_(), data_(nullptr), size_(0) {}

    // Constructor principal
    Tensor(const std::vector<size_t>& shape, const std::vector<double>& values) : shape_(shape) {
        if (shape.size() > 3) throw std::invalid_argument("Maximo 3 dimensiones permitidas.");
        size_ = calculate_size(shape);
        if (values.size() != size_) throw std::invalid_argument("El tamaño de values no coincide con las dimensiones.");

        data_ = size_ > 0 ? new double[size_] : nullptr;
        if (size_ > 0) {
            std::copy(values.begin(), values.end(), data_);
        }
    }

    // --- REGLA DE LOS 5 ---

    ~Tensor() {
        delete[] data_;
    }

    Tensor(const Tensor& other) : shape_(other.shape_), size_(other.size_) {
        data_ = size_ > 0 ? new double[size_] : nullptr;
        if (size_ > 0) {
            std::copy(other.data_, other.data_ + size_, data_);
        }
    }

    Tensor(Tensor&& other) noexcept : shape_(std::move(other.shape_)), data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    Tensor& operator=(const Tensor& other) {
        if (this != &other) {
            delete[] data_;
            shape_ = other.shape_;
            size_ = other.size_;
            data_ = size_ > 0 ? new double[size_] : nullptr;
            if (size_ > 0) {
                std::copy(other.data_, other.data_ + size_, data_);
            }
        }
        return *this;
    }

    Tensor& operator=(Tensor&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            shape_ = std::move(other.shape_);
            data_ = other.data_;
            size_ = other.size_;

            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    // --- MÉTODOS ESTÁTICOS ---

    [[nodiscard]] static Tensor zeros(const std::vector<size_t>& shape) {
        return Tensor(shape);
    }

    [[nodiscard]] static Tensor ones(const std::vector<size_t>& shape) {
        Tensor t(shape);
        if (t.size_ > 0) {
            std::fill(t.data_, t.data_ + t.size_, 1.0);
        }
        return t;
    }

    [[nodiscard]] static Tensor random(const std::vector<size_t>& shape, double min, double max) {
        Tensor t(shape);
        if (t.size_ > 0) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(min, max);
            for (size_t i = 0; i < t.size_; ++i) t.data_[i] = dis(gen);
        }
        return t;
    }

    [[nodiscard]] static Tensor arange(double start, double end) {
        std::vector<double> vals;
        if (end > start) {
            auto steps = static_cast<size_t>(std::ceil(end - start));
            for (size_t i = 0; i < steps; ++i) {
                vals.push_back(start + static_cast<double>(i));
            }
        }
        return Tensor({vals.size()}, vals);
    }

    // --- SOBRECARGA DE OPERADORES ---

    [[nodiscard]] Tensor operator+(const Tensor& b) const {
        if (shape_ == b.shape_) {
            Tensor result(shape_);
            for (size_t i = 0; i < size_; ++i) result.data_[i] = data_[i] + b.data_[i];
            return result;
        }
        else if (shape_.size() == 2 && b.shape_.size() == 2 && b.shape_[0] == 1 && shape_[1] == b.shape_[1]) {
            Tensor result(shape_);
            for (size_t i = 0; i < shape_[0]; ++i) {
                for (size_t j = 0; j < shape_[1]; ++j) {
                    result.data_[i * shape_[1] + j] = data_[i * shape_[1] + j] + b.data_[j];
                }
            }
            return result;
        }
        throw std::invalid_argument("Dimensiones incompatibles para la suma.");
    }

    [[nodiscard]] Tensor operator-(const Tensor& b) const {
        if (shape_ != b.shape_) throw std::invalid_argument("Dimensiones incompatibles.");
        Tensor result(shape_);
        for (size_t i = 0; i < size_; ++i) result.data_[i] = data_[i] - b.data_[i];
        return result;
    }

    [[nodiscard]] Tensor operator*(const Tensor& b) const {
        if (shape_ != b.shape_) throw std::invalid_argument("Dimensiones incompatibles.");
        Tensor result(shape_);
        for (size_t i = 0; i < size_; ++i) result.data_[i] = data_[i] * b.data_[i];
        return result;
    }

    [[nodiscard]] Tensor operator*(double scalar) const {
        Tensor result(shape_);
        for (size_t i = 0; i < size_; ++i) result.data_[i] = data_[i] * scalar;
        return result;
    }

    // --- MODIFICACIÓN DE DIMENSIONES ---

    [[nodiscard]] Tensor view(const std::vector<size_t>& new_shape) {
        if (new_shape.size() > 3) throw std::invalid_argument("Máximo 3 dimensiones.");
        if (calculate_size(new_shape) != size_) throw std::invalid_argument("El numero de elementos no coincide.");

        Tensor result;
        result.shape_ = new_shape;
        result.size_ = size_;
        result.data_ = data_;

        data_ = nullptr;
        size_ = 0;
        shape_.clear();
        return result;
    }

    [[nodiscard]] Tensor unsqueeze(size_t dim) {
        if (shape_.size() >= 3) throw std::invalid_argument("No puede exceder 3 dimensiones.");
        if (dim > shape_.size()) throw std::invalid_argument("Dimensión inválida.");

        std::vector<size_t> new_shape = shape_;
        new_shape.insert(new_shape.begin() + dim, 1);

        return view(new_shape);
    }

    // --- APLICACIÓN DE POLIMORFISMO ---

    [[nodiscard]] Tensor apply(const TensorTransform& transform) const {
        return transform.apply(*this);
    }

    // Getters para helpers y friend functions
    double* data() { return data_; }
    [[nodiscard]] const double* data() const { return data_; }
    [[nodiscard]] const std::vector<size_t>& shape() const { return shape_; }
    [[nodiscard]] size_t size() const { return size_; }

    friend Tensor dot(const Tensor& a, const Tensor& b);
    friend Tensor matmul(const Tensor& a, const Tensor& b);

    static Tensor concat(const std::vector<Tensor>& tensors, size_t axis);
};

// ==========================================
// 3. IMPLEMENTACIONES (Marcadas como inline)
// ==========================================

inline Tensor dot(const Tensor& a, const Tensor& b) {
    if (a.shape_.size() != 1 || b.shape_.size() != 1 || a.size_ != b.size_) {
        throw std::invalid_argument("dot requiere tensores 1D del mismo tamaño.");
    }
    double sum = 0;
    for (size_t i = 0; i < a.size_; ++i) {
        sum += a.data_[i] * b.data_[i];
    }
    return Tensor({1}, {sum});
}

inline Tensor matmul(const Tensor& a, const Tensor& b) {
    if (a.shape_.size() != 2 || b.shape_.size() != 2) throw std::invalid_argument("matmul requiere tensores 2D.");
    if (a.shape_[1] != b.shape_[0]) throw std::invalid_argument("Dimensiones incompatibles para matmul.");

    size_t M = a.shape_[0];
    size_t K = a.shape_[1];
    size_t N = b.shape_[1];

    Tensor result({M, N});
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            double sum = 0;
            for (size_t k = 0; k < K; ++k) {
                sum += a.data_[i * K + k] * b.data_[k * N + j];
            }
            result.data_[i * N + j] = sum;
        }
    }
    return std::move(result);
}

inline Tensor Tensor::concat(const std::vector<Tensor>& tensors, size_t axis) {
    if (tensors.empty()) throw std::invalid_argument("Vector de tensores vacio.");

    std::vector<size_t> res_shape = tensors[0].shape_;
    if (axis >= res_shape.size()) throw std::invalid_argument("Eje invalido.");

    size_t concat_dim_size = 0;
    for (const auto& t : tensors) {
        if (t.shape_.size() != res_shape.size()) throw std::invalid_argument("Dimensiones incompatibles.");
        for (size_t i = 0; i < res_shape.size(); ++i) {
            if (i != axis && t.shape_[i] != res_shape[i]) throw std::invalid_argument("Dimensiones incompatibles.");
        }
        concat_dim_size += t.shape_[axis];
    }
    res_shape[axis] = concat_dim_size;

    Tensor result(res_shape);

    if (axis == 0) {
        size_t offset = 0;
        for (const auto& t : tensors) {
            if (t.size_ > 0) {
                std::copy(t.data_, t.data_ + t.size_, result.data_ + offset);
                offset += t.size_;
            }
        }
    } else {
        throw std::runtime_error("Concatenacion en axis != 0 requiere logica de strides avanzada.");
    }

    return std::move(result);
}

// ==========================================
// 4. CLASES DERIVADAS (TRANSFORMACIONES)
// ==========================================

class ReLU : public TensorTransform {
public:
    [[nodiscard]] Tensor apply(const Tensor& t) const override {
        Tensor result(t.shape());
        for (size_t i = 0; i < t.size(); ++i) {
            result.data()[i] = std::max(0.0, t.data()[i]);
        }
        return result;
    }
};

class Sigmoid : public TensorTransform {
public:
    [[nodiscard]] Tensor apply(const Tensor& t) const override {
        Tensor result(t.shape());
        for (size_t i = 0; i < t.size(); ++i) {
            result.data()[i] = 1.0 / (1.0 + std::exp(-t.data()[i]));
        }
        return result;
    }
};