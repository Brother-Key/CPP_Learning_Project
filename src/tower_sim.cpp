#include "tower_sim.hpp"

#include "GL/opengl_interface.hpp"
#include "aircraft.hpp"
#include "airport.hpp"
#include "config.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>

using namespace std::string_literals;

// TASK_1 move to factory
// const std::string airlines[8] = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };

TowerSimulation::TowerSimulation(int argc, char** argv) :
    help { (argc > 1) && (std::string { argv[1] } == "--help"s || std::string { argv[1] } == "-h"s) }
{
    MediaPath::initialize(argv[0]);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    GL::init_gl(argc, argv, "Airport Tower Simulation");

    create_keystrokes();
}

TowerSimulation::~TowerSimulation()
{
    delete airport;
}

/*
// TASK_1 On modifie la méthode afin qu'il nous renvoie un unique_ptr d'un Aircraft
std::unique_ptr<Aircraft> TowerSimulation::create_aircraft(const AircraftType& type) const
{
    assert(airport); // make sure the airport is initialized before creating aircraft

    const std::string flight_number = airlines[std::rand() % 8] + std::to_string(1000 + (rand() % 9000));
    const float angle       = (rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and 2pi
    const Point3D start     = Point3D { std::sin(angle), std::cos(angle), 0 } * 3 + Point3D { 0, 0, 2 };
    const Point3D direction = (-start).normalize();

    //Aircraft* aircraft = new Aircraft { type, flight_number, start, direction, airport->get_tower() };
    return std::make_unique<Aircraft>(type, flight_number, start, direction, airport->get_tower());
}

void TowerSimulation::create_random_aircraft()
{
    // TASK_1
    // On ajoute l'aircraft crée dans notre manager
    aircraft_manager.add(create_aircraft(*(aircraft_types[rand() % 3])));
}

// TASK_1
// Nos méhtode ne sont plus const avec l'ajout de l'Aircraft dans le manager
void TowerSimulation::create_keystrokes()
{
    GL::keystrokes.emplace('x', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('q', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('c', [this]() { create_random_aircraft(); });
    GL::keystrokes.emplace('+', []() { GL::change_zoom(0.95f); });
    GL::keystrokes.emplace('-', []() { GL::change_zoom(1.05f); });
    GL::keystrokes.emplace('f', []() { GL::toggle_fullscreen(); });
    // TASK_0
    // C - Bidouillons
    // Mapping des nouvells fonction avec les touches  u, d et p
    GL::keystrokes.emplace('u', []() { GL::up_framerate(); });
    GL::keystrokes.emplace('d', []() { GL::down_framerate(); });
    GL::keystrokes.emplace('p', []() { GL::pause_framerate(); });
}
*/

// TASK_1 Usage de la factory
void TowerSimulation::create_keystrokes()
{
    GL::keystrokes.emplace('x', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('q', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('c', [this]() { aircraft_manager.add(aircraft_factory->create_random_aircraft(*airport)); });
    GL::keystrokes.emplace('+', []() { GL::change_zoom(0.95f); });
    GL::keystrokes.emplace('-', []() { GL::change_zoom(1.05f); });
    GL::keystrokes.emplace('f', []() { GL::toggle_fullscreen(); });
    // TASK_0
    // C - Bidouillons
    // Mapping des nouvells fonction avec les touches  u, d et p
    GL::keystrokes.emplace('u', []() { GL::up_framerate(); });
    GL::keystrokes.emplace('d', []() { GL::down_framerate(); });
    GL::keystrokes.emplace('p', []() { GL::pause_framerate(); });
}


void TowerSimulation::display_help() const
{
    std::cout << "This is an airport tower simulator" << std::endl
              << "the following keysstrokes have meaning:" << std::endl;

    for (const auto& ks_pair : GL::keystrokes)
    {
        std::cout << ks_pair.first << ' ';
    }

    std::cout << std::endl;
}

void TowerSimulation::init_airport()
{
    airport = new Airport { one_lane_airport, Point3D { 0, 0, 0 },
                            new img::Image { one_lane_airport_sprite_path.get_full_path() } };

    //GL::display_queue.emplace_back(airport);
    GL::move_queue.emplace(airport);

    // TASK_1
    // On ajoute le manager dans la move_queue
    GL::move_queue.emplace(&aircraft_manager);

    // TASK_1
    // On initialise notre factory
    aircraft_factory = new AircraftFactory();
}

void TowerSimulation::launch()
{
    if (help)
    {
        display_help();
        return;
    }

    init_airport();
    // TASK_1 move to factory
    //init_aircraft_types();

    GL::loop();
}
