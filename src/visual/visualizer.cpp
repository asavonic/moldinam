#include <GL/glew.h>
#include "glfw_wrapper/glfw_wrapper.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <memory>

#include "cube_renderer.h"
#include "particles_renderer.h"

#include <md_types.h>
#include <md_helpers.h>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

class ParticleDataSource {
public:
    ParticleDataSource()
        : updated_(true){};
    virtual bool updated() { return updated_; };
    virtual std::vector<Molecule> get_data() = 0;

protected:
    bool updated_;
};

class StateParticleData : public ParticleDataSource {
public:
    StateParticleData(std::string _state_file)
        : ParticleDataSource()
    {
        state_file = _state_file;
        molecules = read_molecules_from_file(state_file);
    }

    virtual std::vector<Molecule> get_data()
    {
        updated_ = false; // state updated only once
        return molecules;
    }

protected:
    StateParticleData(){};

    std::string state_file;
    std::vector<Molecule> molecules;
};

class TraceParticleData : public ParticleDataSource {
public:
    TraceParticleData(std::string trace_file)
        : ParticleDataSource()
    {
        trace.open(trace_file);
        molecules = trace.initial();
    }

    virtual std::vector<Molecule> get_data()
    {
        if (trace.active) {
            molecules = trace.next();
        }
        return molecules;
    }

protected:
    trace_read trace;
    std::vector<Molecule> molecules;
};

class VisualizerWindow : public glfw::window {

    using parent_t = glfw::window;

public:
    VisualizerWindow()
        : glfw::window()
    {
        std::vector<glm::vec3> particles;
        particles.push_back(glm::vec3(0.5, 0.0, 0.0));
        particle_render.set_positions(particles);
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
        glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 projection = glm::perspective(45.0f, 1.0f * width() / height(), 0.1f, 10.0f);
        glm::mat4 mvp = projection * view * model * anim;

        cube_render.set_mvp(mvp);

        if (particle_data_source_->updated()) {
            particle_render.set_particles_positions(particle_data_source_->get_data());
        }

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

    virtual void set_particle_data_source(std::unique_ptr<ParticleDataSource> _in) { particle_data_source_ = std::move(_in); }

    glm::vec2 mouse_move;
    int mouse_action;
    int mouse_button;

    glm::vec2 angle;

    ParticleRenderer particle_render;
    CubeRenderer cube_render;

    std::unique_ptr<ParticleDataSource> particle_data_source_;
};

int main(int argc, char** argv)
{
    try {
        std::string state_file;
        std::string trace_file;

        // named arguments
        po::options_description desc("Allowed options");
        desc.add_options()("help", "produce help message")("state", po::value<std::string>(&state_file), "view state file (input or output)")("trace", po::value<std::string>(&trace_file), "view trace file with animation");

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);

        if (vm.count("help")) {
            std::cout << desc << std::endl;
            return 1;
        }

        if (vm.count("state") + vm.count("trace") > 1 || vm.count("state") + vm.count("trace") == 0) {
            throw po::error("State OR trace file must be specified");
        }

        po::notify(vm);

        VisualizerWindow window;

        if (vm.count("state")) {
            window.set_particle_data_source(std::unique_ptr<ParticleDataSource>(new StateParticleData(state_file)));
        } else {
            window.set_particle_data_source(std::unique_ptr<ParticleDataSource>(new TraceParticleData(trace_file)));
        }

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
