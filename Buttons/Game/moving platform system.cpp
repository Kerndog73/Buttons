//
//  moving platform system.cpp
//  Buttons
//
//  Created by Indi Kernick on 16/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "moving platform system.hpp"

#include "physics component.hpp"
#include "activation component.hpp"
#include <Simpleton/Math/vectors.hpp>
#include "moving platform component.hpp"

namespace {
  using State = MovingPlatform::State;

  bool finishedMovingTo(const b2Vec2 dest, b2Body *const body) {
    const b2Vec2 pos = body->GetPosition();
    const b2Vec2 vel = body->GetLinearVelocity();
    
    if (!Math::sameQuadrant(vel, dest - pos)) {
      body->SetTransform(dest, body->GetAngle());
      return true;
    } else {
      return false;
    }
  }
  
  void finishMove(
    MovingPlatform &platform,
    b2Body *const body,
    const State toNewDest,
    const State waitAtDest
  ) {
    if (platform.waitingTime <= 0.0f) {
      platform.state = toNewDest;
      body->SetLinearVelocity(-body->GetLinearVelocity());
    } else {
      platform.state = waitAtDest;
      platform.timeTillFinishWait = platform.waitingTime;
      body->SetLinearVelocity({0.0f, 0.0f});
    }
  }
  
  bool finishedWait(MovingPlatform &platform, const float delta) {
    platform.timeTillFinishWait -= delta;
    return platform.timeTillFinishWait <= 0.0f;
  }
  
  b2Vec2 toEndVel(MovingPlatform &platform) {
    return platform.moveSpeed * (platform.end - platform.start);
  }
  
  void activeState(MovingPlatform &platform, b2Body *const body, const float delta) {
    const b2Vec2 toEnd = toEndVel(platform);
    
    switch (platform.state) {
      case State::AT_START:
        platform.state = State::TO_END;
        body->SetLinearVelocity(toEnd);
        break;
      case MovingPlatform::State::TO_END:
        if (finishedMovingTo(platform.end, body)) {
          finishMove(platform, body, State::TO_START, State::WAIT_AT_END);
        }
        break;
      case MovingPlatform::State::WAIT_AT_END:
        if (finishedWait(platform, delta)) {
          platform.state = State::TO_START;
          body->SetLinearVelocity(-toEnd);
        }
        break;
      case MovingPlatform::State::TO_START:
        if (finishedMovingTo(platform.start, body)) {
          finishMove(platform, body, State::TO_END, State::WAIT_AT_START);
        }
        break;
      case MovingPlatform::State::WAIT_AT_START:
        if (finishedWait(platform, delta)) {
          platform.state = State::TO_END;
          body->SetLinearVelocity(toEnd);
        }
        break;
    }
  }
  
  void inactiveState(MovingPlatform &platform, b2Body *const body) {
    if (platform.state == State::WAIT_AT_START) {
      platform.state = State::AT_START;
    } else if (platform.state == State::TO_START) {
      if (finishedMovingTo(platform.start, body)) {
        platform.state = State::AT_START;
        body->SetLinearVelocity({0.0f, 0.0f});
      }
    } else if (platform.state != State::AT_START) {
      body->SetLinearVelocity(platform.moveSpeed * (platform.start - platform.end));
      platform.state = State::TO_START;
    }
  }
  
  void activePiston(MovingPlatform &platform, b2Body *const body) {
    const b2Vec2 toEnd = toEndVel(platform);
  
    if (platform.state == State::AT_START || platform.state == State::TO_START) {
      platform.state = State::TO_END;
      body->SetLinearVelocity(toEnd);
    } else if (platform.state == State::TO_END) {
      if (finishedMovingTo(platform.end, body)) {
        finishMove(platform, body, State::TO_START, State::WAIT_AT_END);
      }
    }
  }
  
  void inactivePiston(MovingPlatform &platform, b2Body *const body) {
    const b2Vec2 toEnd = toEndVel(platform);
    
    if (platform.state == State::WAIT_AT_END || platform.state == State::TO_END) {
      platform.state = State::TO_START;
      body->SetLinearVelocity(-toEnd);
    } else if (platform.state == State::TO_START) {
      if (finishedMovingTo(platform.start, body)) {
        finishMove(platform, body, State::TO_END, State::AT_START);
      }
    }
  }
}

void movingPlatformSystem(ECS::Registry &registry, const float delta) {
  auto view = registry.view<PhysicsBody, Activation, MovingPlatform>();
  for (const ECS::EntityID entity : view) {
    MovingPlatform &platform = view.get<MovingPlatform>(entity);
    b2Body *const body = view.get<PhysicsBody>(entity).body;
    const bool active = view.get<Activation>(entity).state == Activation::State::ACTIVE;
    if (platform.waitingTime == MovingPlatform::PISTON) {
      if (active) {
        activePiston(platform, body);
      } else {
        inactivePiston(platform, body);
      }
    } else {
      if (active) {
        activeState(platform, body, delta);
      } else {
        inactiveState(platform, body);
      }
    }
  }
}
