//
//  laser detector init.hpp
//  Buttons
//
//  Created by Indi Kernick on 19/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef laser_detector_init_hpp
#define laser_detector_init_hpp

#include <Simpleton/ECS/comp init.hpp>
#include "laser detector component.hpp"

class LaserDetectorInit final : public ECS::CompInit<LaserDetector> {
public:
  void init(LaserDetector &, const json &, const ECS::EntityIDmap &) override;
};

#endif
