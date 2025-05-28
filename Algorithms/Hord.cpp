#include <iostream>
#include <cmath>
#include <functional>
#include <limits>

double chordMethod(std::function<double(double)> f, double x0, double x1, double tol = 1e-6, int max_iter = 100) {
    double f0 = f(x0);
    double f1 = f(x1);
    int iter = 0;

    while (fabs(x1 - x0) > tol && iter < max_iter) {
        if (fabs(f1 - f0) < std::numeric_limits<double>::epsilon()) {
            std::cerr << "Error: division by zero in chord method.\n";
            break;
        }

        double x2 = x1 - f1 * (x1 - x0) / (f1 - f0);

        x0 = x1;
        f0 = f1;
        x1 = x2;
        f1 = f(x1);

        iter++;
    }

    if (iter == max_iter) {
        std::cerr << "Warning: method did not converge after " << max_iter << " iterations.\n";
    }

    return x1;
}

// Example function: f(x) = x^3 - x - 2
double exampleFunction(double x) {
    return x * x * x - x - 2;
}

int main() {
    double x0, x1, tol;
    int max_iter;

    std::cout << "Enter initial guess x0: ";
    std::cin >> x0;

    std::cout << "Enter initial guess x1: ";
    std::cin >> x1;

    std::cout << "Enter tolerance (e.g., 1e-6): ";
    std::cin >> tol;

    std::cout << "Enter maximum number of iterations: ";
    std::cin >> max_iter;

    double root = chordMethod(exampleFunction, x0, x1, tol, max_iter);

    std::cout << "Approximate root: " << root << "\n";
    std::cout << "Function value at root: f(root) = " << exampleFunction(root) << "\n";

    return 0;
}
