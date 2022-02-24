#include "Application.hpp"


int main()
{
    App::Application* app = App::Application::create();

    app->run();

    app->destroy();
}
