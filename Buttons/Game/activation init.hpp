//
//  activation init.hpp
//  Buttons
//
//  Created by Indi Kernick on 18/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef activation_init_hpp
#define activation_init_hpp

#include "activation component.hpp"
#include <Simpleton/ECS/comp init.hpp>

class ActivationInit final : public ECS::CompInit<Activation> {
public:
  void init(Activation &, const json &) override;
};

#endif
