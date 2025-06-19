#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <queue>
#include <condition_variable>
#include <atomic>

//Mutex global que protege la salida estandar:
std::mutex cout_mutex;

//Señal de ejecución general:
std::atomic<bool> running(true);

//Contador de la cantidad de sensores que terminaron de generar y agregar tareas:
std::atomic<int> sensoresTerminados = 0;

//Estructura de cada tarea: 
struct tarea{
    int idSensor; 
    int idTarea; 
    std::string descripcionTarea; 
};

/*
Clase Cola: 
Genera la cola que guarda las tareas, sincroniza el acceso a esta cola
Atributos privados:
- mutex_mecanismo: mutex que protege el acceso a la cola
- cola: la cola de punteros a las tareas.
- condicion_var: notifica la presencia de tareas 
*/
class Cola {
    
    private: 
        std::mutex mutex_mecanismo;
        std::queue<std::unique_ptr<tarea>> cola;
        std::condition_variable condicion_var;
    
    public: 
        //Constructor:
        Cola() {

        }
        /*
        Función encolar:
        Agrega tareas a la cola
        Parámetro: puntero a tarea
        Devuelve: void
        */
        void encolar(std::unique_ptr<tarea> tarea) {
            {
            std::unique_lock<std::mutex> lock(mutex_mecanismo);
            cola.push(std::move(tarea));
            } 
            condicion_var.notify_one();
            return;
        }
        /*
        Función desencolar:
        Quita tareas de la cola, espera si no hay tareas disponibles, y termina si los sensores terminaron. 
        Parámetro: totalSensores: un numero del total de los Sensores
        Devuelve: puntero único a la tarea que se quita o nullptr si no hay tareas a quitar.
        */
        std::unique_ptr<tarea> desencolar(int totalSensores){
            std::unique_lock<std::mutex> lock(mutex_mecanismo);
            condicion_var.wait(lock, [this, totalSensores]{
                return !cola.empty() || sensoresTerminados == totalSensores;
            });
            if(cola.empty()){return nullptr;}
            auto tarea = std::move(cola.front());
            cola.pop();
            return tarea;   
        }
};

/*
Clase Sensor
Genera un sensor que crea tareas. 
Atributos: 
- idSensor: identifica el sensor
- contador_id_tareas: cuenta tareas generadas
*/
class Sensor{
    private:
        int idSensor;
        int contador_id_tareas = 0;
    public:
        Sensor() = delete;
        //Constructor: recibe como parámetro el identificador del sensor
        Sensor(int id) : idSensor(id) {}
        /*
        Función producirTarea:
        Produce una tarea luego de esperar 175 milisegundos.
        Devuelve un puntero único a la tarea creada.
        */
        std::unique_ptr<tarea> producirTarea() {
            std::this_thread::sleep_for(std::chrono::milliseconds(175));
            auto tarea_sensor = std::make_unique<tarea>();
            tarea_sensor -> idSensor = idSensor;
            tarea_sensor -> idTarea = contador_id_tareas;
            tarea_sensor -> descripcionTarea = "Sensor: " + std::to_string(tarea_sensor -> idSensor) + ", Tarea: " + std::to_string(tarea_sensor -> idTarea);
            ++contador_id_tareas;
            return tarea_sensor;
        }
};

/*
Función sensorFuncion:
Parámetros:
- id: identificador del sensor
- cola: puntero compartido a la cola
- cantidad_tareas: cantidad de tareas a crear
Devuelve: void
*/
void sensorFuncion(int id, std::shared_ptr<Cola> cola, int cantidad_tareas){
    Sensor sensor(id);
    for(int i = 0; i < cantidad_tareas; i++){
        auto nueva_tarea = sensor.producirTarea();
        cola->encolar(std::move(nueva_tarea));
    }
    ++sensoresTerminados;
    return;
};


/*
Clase Robot:
Atributo: idRobot: identificador del robot
*/
class Robot{
    private:
        int idRobot;
    public:
        Robot() = delete;
        //Constructor: recibe como parámetro el identificador del robot
        Robot(int id) : idRobot(id) {} 
        /*
        Funcion procesarTarea:
        Procesa tarea luego de 250 milisegundos
        Parámetro: referencia a tarea
        Devuelve: void
        */
        void procesarTarea(const tarea& tarea_a_procesar){
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "El robot " << idRobot << " procesó la tarea " << tarea_a_procesar.idTarea << " del sensor " << tarea_a_procesar.idSensor << std::endl;
            return;
        }
};

/*
Función robotFuncion:
El robot desencola y procesa las tareas correspondientes.
Parámetros:
- id: identificador del robot
- cola: puntero a la cola
- totalSensores: numero de sensores
Retorna: void
*/
void robotFuncion(int id, std::shared_ptr<Cola> cola, int totalSensores){
    Robot robot(id);
    while(true){
        auto tarea_a_procesar = cola -> desencolar(totalSensores);
        if(!tarea_a_procesar){break;}
        robot.procesarTarea(*tarea_a_procesar);

    }
    return;
};


//main: crea los hilos, sensores y robots para llevar a cabo el proceso

int main() {
    const int cantidad_sensores = 3; 
    const int cantidad_robots = 3; 
    const int cantidad_tareas_por_sensor = 4;

    auto cola = std::make_shared<Cola>();
    std::vector<std::thread> threads;

    for (int i = 0; i < cantidad_sensores; ++i) {
        threads.push_back(std::thread(sensorFuncion, i, cola, cantidad_tareas_por_sensor));
    }
    
    for (int i = 0; i < cantidad_robots; ++i) {
        threads.push_back(std::thread(robotFuncion, i, cola, cantidad_sensores));
    }

    for (auto& t : threads) {
        if(t.joinable()){
            t.join();
        }    
    }

    return 0;
}