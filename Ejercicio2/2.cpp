#include "2.hpp"

Dron::Dron(int id, std::mutex* z1, std::mutex* z2, std::mutex& m)
    : identificador(id), zona1(z1), zona2(z2), mutexSalida(m) {}

void Dron::operator()() {
    {
        std::lock_guard<std::mutex> bloqueo(mutexSalida);  // Bloqueo del cout mutex para evitar problemas con el print
        std::cout << "Dron " << identificador << " listo para despegar...\n";
    }

    std::lock(*zona1, *zona2);
    std::lock_guard<std::mutex> bloqueaZona1(*zona1, std::adopt_lock);  // bloqueamos simultaneamente los mutex de las zonas adyacentes, y con adopt_lock se indica que ya estan bloqueados.
    std::lock_guard<std::mutex> bloqueaZona2(*zona2, std::adopt_lock);

    {
        std::lock_guard<std::mutex> bloqueo(mutexSalida);  // Bloqueo del cout mutex para evitar problemas con el print
        std::cout << "Dron " << identificador << " despegando...\n";  
    }

    std::this_thread::sleep_for(std::chrono::seconds(5)); // Simulamos el tiempo de despegue

    {
        std::lock_guard<std::mutex> bloqueo(mutexSalida);  // Bloqueo del cout mutex para evitar problemas con el print
        std::cout << "Dron " << identificador << " alcanzó los 10 metros de altura.\n"; // Simulamos que el dron alcanzo los 10 metros de altura
    }
}
