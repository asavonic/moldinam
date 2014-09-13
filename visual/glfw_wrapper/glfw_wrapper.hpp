#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>
#include <functional>
#include <memory>
#include <list>
#include <algorithm>

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
            if ( !glfwInit() ) {
                throw std::runtime_error( "Couldn`t initialize GLFW system" );
            }
        }

        ~glfw_system() 
        {
            glfwTerminate();
        }
    };

    class window;
    /**
     * @brief List contains pointers to created window objects and their GLFWwindow* objects. Used to callback handling.
     */
    static std::list< std::pair< window*, GLFWwindow*> > g_window_object_list;

    /**
     * @brief Internal callback for resize event. Finds window object according to glfw_window_ptr and calls its resize_callback() method
     *
     * @param glfw_window_ptr is a pointer to plain GLFW window
     * @param width is a new width
     * @param height is a new height
     */
    static void g_resize_callback( GLFWwindow* glfw_window_ptr, int width, int height );


    /**
     * @brief Class represents window object and contains methods to create and manipulate windows
     */
    class window {
        public:

        /**
         * @brief Creates windows object with default size 640 x 480
         */
        explicit window() : window( 640, 480, "GLFW window" ) { };

        /**
         * @brief Creates window object
         *
         * @param width Width of the window, must be greater than zero
         * @param height Height of the window, must be greater than zero
         */
        explicit window( size_t width, size_t height, std::string title ) 
        {
            glfw_system::instance();

            _window =  glfwCreateWindow( width, height, title.c_str(), NULL /* no need fullscreen */, 
                                                                          NULL /* do not share resources with other windows */ );

            g_window_object_list.push_back( std::make_pair( this, _window ) );

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
            g_window_object_list.remove( std::make_pair( this, _window ) );
            glfwDestroyWindow( _window );
        }

        /**
         * @brief Returns width of the window
         */
        virtual size_t width() {
            return _width;
        }

        /**
         * @brief Returns height of the window
         */
        virtual size_t height() {
            return _height;
        }

        /**
         * @brief This function swaps the front and back buffers of the window. 
         */
        virtual void swap_buffers() {
            glfwSwapBuffers( _window );
        }

        /**
         * @brief Starts the window and calls draw() method which must be overloaded
         */
        virtual void start() {
            glfwMakeContextCurrent ( _window );

            glfwSetFramebufferSizeCallback( _window, g_resize_callback );
            while ( !glfwWindowShouldClose ( _window ) ) {
                draw();

                glfwSwapBuffers ( _window );
                glfwPollEvents();
            }
        }

        /**
         * @brief Overload this function to draw something.
         */
        virtual void draw() = 0;

        virtual void resize_callback( size_t width, size_t height ) {
            _width = width;
            _height = height;
        }

        private:
        GLFWwindow* _window;

        size_t _height;
        size_t _width;
    };


    static void resize_callback( GLFWwindow* glfw_window_ptr, int width, int height ) {
        auto iter = std::find_if( g_window_object_list.begin(), g_window_object_list.end(), [&]( std::pair< window* , GLFWwindow* > item ) { return &(*glfw_window_ptr) == &(*item.second ); } );
        if ( iter != g_window_object_list.end() ) { 
            window* win_ptr = iter->first;
            win_ptr->resize_callback( width, height );
        } else {
            throw std::runtime_error( "Resize callback was unable to find window object" );
        }
    }
}
