// Project: Slew
// File: examples/test_entt.cpp
// Description: TEST example for EnTT ECS.
// License: MIT
// https://github.com/colleen05/Slew

#include <slew/slew.hpp>
#include <entt/entt.hpp>

#include <iostream>

struct position_component {
    float x;
    float y;
};

class announcer_component {
    public:
        std::string message;
        
        void announce() {
            std::cout << "[ANNOUNCEMENT] " << message << std::endl;
        }

        announcer_component(const std::string message) :
            message(message) {}

        announcer_component() :
            announcer_component("<Default Message>") {}
};

void update_position_system(entt::registry &registry) {
    auto view = registry.view<position_component>();

    for(auto [entity, position]: view.each()) {
        std::cout << "Entity #" << (uint32_t)entity << " has position <" << position.x << ", " << position.y << ">." << std::endl;
    }

    std::cout << std::endl;
}

void update_announcer_system(entt::registry &registry) {
    auto view = registry.view<announcer_component>();

    for(auto [entity, announcer]: view.each()) {
        std::cout << "Entity #" << (uint32_t)entity << ": ";
        announcer.announce();
    }

    std::cout << std::endl;
}

int main() {
    slew::print_lib_info();
    std::cout << std::endl;

    // EnTT Setup
    entt::registry registry;

    const int ent_count = 10;
    const std::string colour_names[] = {
        "red", "orange", "yellow", "green", "blue", "indigo", "violet",
        "gold", "silver", "bronze"
    };

    // Creating entities
    std::cout << "===== CREATING ENTITIES =====" << std::endl;
    for(int i = 0; i < 10; i++) {
        const auto entity = registry.create();

        // Applying components
        registry.emplace<position_component>(entity, i * 1.0f, i * 1.0f);
        registry.emplace<announcer_component>(entity, "I am " + colour_names[i] + "!");
        
        std::cout << "Created entity #" << (uint32_t)entity << " with <position_component, announcer_component>." << std::endl;
    }

    std::cout << std::endl;

    // Updating systems
    std::cout << "===== UPDATING SYSTEMS =====" << std::endl;
    update_position_system(registry);
    update_announcer_system(registry);

    return 0;
}