#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>

/**
 * @brief Namespace for all GLFW related stuff
 *
 * GLFW is a free, Open Source, portable library for OpenGL and OpenGL ES application development. 
 * It provides a simple, platform-independent API for creating windows and contexts, 
 * reading input, handling events, etc.
 */
namespace glfw {
    /**
     * @brief Singletone for GLFW setup and termination 
     */
    class glfw_system 
    {
        public:
            /**
             * @brief Get method for glfw_system singletone
             *
             * @return glfw_instance object
             */
            static glfw_system& instance() 
            {
                static glfw_system system;
                return system;
            }

        private:

        glfw_system( const glfw_system& ) = delete;
        void operator=( const glfw_system& ) = delete;

        glfw_system() 
        {
            if ( not  !glfwInit() ) {
                throw std::runtime_error( "Couldn`t initialize GLFW system" );
            }
        }

        ~glfw_system() 
        {
            glfwTerminate();
        }
    };

    /**
     * @brief Class represents window object and contains methods to create and manipulate windows
     */
    class window {
        public:

        /**
         * @brief Creates windows object with default size 640 x 480
         */
        window() : window( 640, 480, "GLFW window" ) { };

        /**
         * @brief Creates window object
         *
         * @param width Width of the window, must be greater than zero
         * @param height Height of the window, must be greater than zero
         */
        window( size_t width, size_t height, std::string title ) 
        {
            glfw_system::instance();

            _window =  glfwCreateWindow( width, height, title.c_str(), NULL /* no need fullscreen */, 
                                                                          NULL /* do not share resources with other windows */ );

            int temp_width = 0;
            int temp_height = 0;
            glfwGetWindowSize( _window, &temp_width, &temp_height );

            _width = temp_width;
            _height = temp_height;
        }

        /**
         * @brief Destroys window object
         */
        ~window() {
            glfwDestroyWindow( _window );
        }

        /**
         * @brief Returns width of the window
         */
        size_t width() {
            return _width;
        }

        /**
         * @brief Returns height of the window
         */
        size_t height() {
            return _height;
        }

        /**
         * @brief This function swaps the front and back buffers of the window. 
         */
        void swap_buffers() {
            glfwSwapBuffers( _window );
        }

        private:
        GLFWwindow* _window;

        size_t _height;
        size_t _width;
    };
}
