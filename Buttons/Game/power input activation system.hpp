//
//  power input activation system.hpp
//  Buttons
//
//  Created by Indi Kernick on 7/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef power_input_activation_system_hpp
#define power_input_activation_system_hpp

#include <Simpleton/ECS/registry.hpp>

///Activates an entity when it is receiving input. (Laser, Door, Moving Platform)
void powerInputActivationSystem(ECS::Registry &);

#endif
