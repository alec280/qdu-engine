#include "QDUEngine.hpp"

#include <iostream>

class Demo : public QDU::Application
{
public:
    Demo() = default;
    ~Demo() = default;
    virtual void UserStartUp(QDU::World &world) noexcept override {
        std::cout << "Hello world!" << std::endl;
    }
    virtual void UserShutDown(QDU::World& world) noexcept override {
    }
    virtual void UserUpdate(QDU::World& world, float timeStep) noexcept override {
    }
};

int main()
{
    Demo app;
    QDU::Engine engine(app);
    engine.StartMainLoop();
}