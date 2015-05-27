#include <GL/glew.h>
#include "glfw_wrapper/glfw_wrapper.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <memory>

#include "cube_renderer.h"
#include "particles_renderer.h"

#include <platforms/native/types.hpp>
#include <utils/config/config_manager.hpp>
#include <platforms/native/native_platform.hpp>

#include <boost/program_options.hpp>


namespace po = boost::program_options;

class VisualizerWindow : public glfw::window {

    typedef glfw::window parent_t;

public:
    VisualizerWindow(NativeParticleSystem& psys, std::ifstream& trace_stream)
        : glfw::window(),
          m_part_system(psys),
          m_trace_stream(trace_stream)
    {
        std::vector<glm::vec3> particles;
        particles.push_back(glm::vec3(0.5, 0.0, 0.0));
    }

    virtual void draw()
    {
        glClearColor(0.1, 0.2, 0.5, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        /* here goes an ugly hack:
     *      we`re using angle_y for X rotation
     *      and angle_x for Y rotation
     *
     *      TODO
     *      this must be fixed
     */
        glm::vec3 axis_x(1, 0, 0);
        glm::mat4 anim = glm::rotate(glm::mat4(1.0f), angle.y, axis_x);

        glm::vec3 axis_y(0, 1, 0);
        anim = glm::rotate(anim, angle.x, axis_y);

        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0, -4.0));
        glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 0.0, -4.0),
                                     glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 projection = glm::perspective(45.0f, 1.0f * width() / height(), 0.1f, 10.0f);
        glm::mat4 mvp = projection * view * model * anim;

        cube_render.set_mvp(mvp);

        m_part_system.loadParticles(m_trace_stream);
        particle_render.set_particles_positions(m_part_system.pos(), m_part_system.config().area_size.value());

        particle_render.set_mvp(mvp);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        particle_render.display();
        cube_render.display();
    }

    virtual void mouse_move_callback(double new_x, double new_y)
    {
        static double old_x = new_x;
        static double old_y = new_y;

        mouse_move.x = new_x - old_x;
        mouse_move.y = new_y - old_y;

        old_x = new_x;
        old_y = new_y;

        control();
    }

    virtual void mouse_press_callback(int button, int action, int mods)
    {
        mouse_button = button;
        mouse_action = action;
        control();
    }

    virtual void control()
    {
        if (mouse_action == GLFW_PRESS && mouse_button == GLFW_MOUSE_BUTTON_LEFT) {
            angle.x += mouse_move.x / 10;
            angle.y += mouse_move.y / 10;
        }
    }

    glm::vec2 mouse_move;
    int mouse_action;
    int mouse_button;

    glm::vec2 angle;

    NativeParticleSystem& m_part_system;
    std::istream& m_trace_stream;

    ParticleRenderer particle_render;
    CubeRenderer cube_render;
};

int main(int argc, char** argv)
{
    try {
        std::vector<std::string> config_files;
        std::string trace_file;

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("config,c", po::value<std::vector<std::string> >(&config_files)->required()->multitoken(),
             "path to particle system config")
            ("trace,t", po::value<std::string>(&trace_file), "use another trace file")
        ;

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);

        if (vm.count("help")) {
            std::cout << desc << std::endl;
            return 1;
        }

        po::notify(vm);

        ConfigManager& conf_man = ConfigManager::Instance();
        for (auto& conf : config_files) {
            conf_man.loadFromFile(conf);
        }
        ParticleSystemConfig psys_conf = conf_man.getParticleSystemConfig();
        NativeParticleSystem native_system(psys_conf);

        TraceConfig trace_conf = conf_man.getTraceConfig();
        if (trace_conf.filename.value() == "") {
            throw std::runtime_error("TraceConfig does not contain filename to read");
        }

        std::string filename = (trace_file != "") ? trace_file : trace_conf.filename;
        std::ifstream ifs(filename);

        if (!ifs.good()) {
            throw std::runtime_error("Unable to open file: " + filename);
        }

        VisualizerWindow window(native_system, ifs);

        window.start();
    }
    catch (boost::program_options::error& po_error) {
        std::cerr << po_error.what() << std::endl;
        return 1;
    }
    catch (std::exception& ex) {
        std::cerr << "Unknown exception: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
