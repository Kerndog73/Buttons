//
//  contact listener.cpp
//  Buttons
//
//  Created by Indi Kernick on 7/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "contact listener.hpp"

#include <utility>
#include "../Libraries/Box2D/Dynamics/Contacts/b2Contact.h"

namespace {
  std::pair<EntityID, EntityID> getEntityPair(b2Contact *const contact) {
    return {
      static_cast<EntityID>(
        reinterpret_cast<uintptr_t>(contact->GetFixtureA()->GetUserData())
      ),
      static_cast<EntityID>(
        reinterpret_cast<uintptr_t>(contact->GetFixtureB()->GetUserData())
      )
    };
  }
}

void ContactListener::BeginContact(b2Contact *const contact) {
  if (beginListener) {
    const auto [entityA, entityB] = getEntityPair(contact);
    beginListener(entityA, entityB);
  }
}

void ContactListener::EndContact(b2Contact *const contact) {
  if (endListener) {
    const auto [entityA, entityB] = getEntityPair(contact);
    endListener(entityA, entityB);
  }
}

void ContactListener::setBeginListener(const Listener &newListener) {
  beginListener = newListener;
}

void ContactListener::setEndListener(const Listener &newListener) {
  endListener = newListener;
}
