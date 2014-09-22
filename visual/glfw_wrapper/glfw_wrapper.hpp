#include <GL/glew.h>
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
     * @brief Internal callback for mouse press event. Finds window object according to glfw_window_ptr and calls its mouse_press_callback() method
     *
     * @param glfw_window_ptr is a pointer to plain GLFW window
     * @param key is the keyboard key that was pressed or released. 
     * @param scancode The system-specific scancode of the key. 
     * @param action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT ( when mouse button held down, repeats on release ). 
     * @param mods Bit field describing which modifier keys were held down. GLFW_MOD_SHIFT, GLFW_MOD_CONTROL, GLFW_MOD_ALT, GLFW_MOD_SUPER
     */
    static void g_mouse_press_callback( GLFWwindow* glfw_window_ptr, int key, int scancode, int action, int mods );

    /**
     * @brief Internal callback for mouse move event. Finds window object according to glfw_window_ptr and calls its mouse_move_callback() method
     *
     * @param glfw_window_ptr
     * @param new_pos_x
     * @param new_pos_y
     */
    static void g_mouse_move_callback( GLFWwindow* glfw_window_ptr, double new_pos_x , double new_pos_y );


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
            glfwMakeContextCurrent(_window);

            auto glew_err = glewInit();
            if ( glew_err ) {
                throw std::runtime_error( std::string( "Couldn`t initialize GLEW system - " ) + std::string( (char*)glewGetErrorString( glew_err ) ) );
            }

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
            //glfwSetMouseButtonCallback( _window, g_mouse_press_callback );
            glfwSetCursorPosCallback( _window, g_mouse_move_callback );
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

        /**
         * @brief Resize callback called ufter window resize. Updates _width and _height
         *
         * @param width New width of the window
         * @param height New height of the window
         */
        virtual void resize_callback( size_t width, size_t height ) {
            _width = width;
            _height = height;
        }

        /**
         * @brief Called when some mouse evet occured. Does nothing in glfw::window, but can be overloaded in derived class
         *
         * @param key Mouse button key: GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_RIGHT, GLFW_MOUSE_BUTTON_MIDDLE
         * @param scancode The system-specific scancode of the key. 
         * @param action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT. 
         * @param mods Bit field describing which modifier keys were held down. GLFW_MOD_SHIFT, GLFW_MOD_CONTROL, GLFW_MOD_ALT, GLFW_MOD_SUPER
         */
        virtual void mouse_press_callback( int key, int scancode, int action, int mods ) {

        }

        /**
         * @brief Called on cursor move. The callback is provided with the position, in screen coordinates, relative to the upper-left corner of the client area of the window.
         *
         * @param new_pos_x New position by X
         * @param new_pos_y New position by Y
         */
        virtual void mouse_move_callback( double new_pos_x, double new_pos_y ) {
            
        }

        protected:
        GLFWwindow* _window;

        size_t _height;
        size_t _width;
    };


    /**
     * @brief Finds glfw::window object according to glfw_window_ptr. Throw runtime_error if not found ( exceptional case and it is most likely an internal bug )
     *
     * @param glfw_window_ptr Plain GLFWwindow pointer to be compared with glfw::window:_window
     *
     * @return glfw::window object for plain GLFWwindow
     */
    static window* g_find_window_object( GLFWwindow* glfw_window_ptr ) {
        auto iter = std::find_if( g_window_object_list.begin(), g_window_object_list.end(), [&]( std::pair< window* , GLFWwindow* > item ) { return &(*glfw_window_ptr) == &(*item.second ); } );

        if ( iter == g_window_object_list.end() ) { 
            throw std::runtime_error( "Callback was unable to find window object" );
        }

        return iter->first;
    }

    static void g_resize_callback( GLFWwindow* glfw_window_ptr, int width, int height ) {
        window* win_ptr = g_find_window_object( glfw_window_ptr );
        win_ptr->resize_callback( width, height );
    }

    static void g_mouse_press_callback( GLFWwindow* glfw_window_ptr, int key, int scancode, int action, int mods ) {
        window* win_ptr = g_find_window_object( glfw_window_ptr );
        win_ptr->mouse_press_callback( key, scancode, action, mods );
    }

    static void g_mouse_move_callback( GLFWwindow* glfw_window_ptr, double new_pos_x , double new_pos_y ) {
        window* win_ptr = g_find_window_object( glfw_window_ptr );
        win_ptr->mouse_move_callback( new_pos_x, new_pos_y );
    }
}
