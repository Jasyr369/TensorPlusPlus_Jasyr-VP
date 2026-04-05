#include <iostream>
#include "Tensor.h" // Incluimos nuestra librería

int main() {
    try {
        std::cout << "--- Iniciando Flujo de Red Neuronal --- \n";

        Tensor input = Tensor::random({1000, 20, 20}, -1.0, 1.0);
        std::cout << "1. Input creado con exito.\n";

        Tensor input_flat = input.view({1000, 400});
        std::cout << "2. View aplicado.\n";

        Tensor W1 = Tensor::random({400, 100}, -0.1, 0.1);
        Tensor hidden = matmul(input_flat, W1);
        std::cout << "3. Matmul W1 completado.\n";

        Tensor b1 = Tensor::random({1, 100}, -0.1, 0.1);
        hidden = hidden + b1;
        std::cout << "4. Suma Bias b1 completada.\n";

        ReLU relu;
        hidden = hidden.apply(relu);
        std::cout << "5. ReLU aplicado.\n";

        Tensor W2 = Tensor::random({100, 10}, -0.1, 0.1);
        Tensor output = matmul(hidden, W2);
        std::cout << "6. Matmul W2 completado.\n";

        Tensor b2 = Tensor::random({1, 10}, -0.1, 0.1);
        output = output + b2;
        std::cout << "7. Suma Bias b2 completada.\n";

        Sigmoid sigmoid;
        output = output.apply(sigmoid);
        std::cout << "8. Sigmoid aplicado. Dimensiones finales de salida: {"
                  << output.shape()[0] << ", " << output.shape()[1] << "}\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}