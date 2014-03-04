#define CATCH_CONFIG_MAIN

#include <iostream>
#include <vector>
#include "entityx/3rdparty/catch.hpp"
#include "entityx/help/Timer.h"
#include "entityx/Entity.h"

using namespace std;
using namespace entityx;


struct AutoTimer {
  ~AutoTimer() {
    cout << timer_.elapsed() << " seconds elapsed" << endl;
  }

private:
  entityx::help::Timer timer_;
};

struct Listener : public Receiver<Listener> {
  void receive(const EntityCreatedEvent &event) {}
  void receive(const EntityDestroyedEvent &event) {}
};

struct Position : public Component<Position> {
};

TEST_CASE("BenchmarksTest", "[entitymanager]") {
  EventManager ev;
  EntityManager em(ev);


  SECTION("TestCreateEntities") {
    AutoTimer t;

    uint64_t count = 10000000L;
    cout << "creating " << count << " entities" << endl;

    for (uint64_t i = 0; i < count; i++) {
      em.create();
    }
  }


  SECTION("TestDestroyEntities") {
    uint64_t count = 10000000L;
    vector<Entity> entities;
    for (uint64_t i = 0; i < count; i++) {
      entities.push_back(em.create());
    }

    AutoTimer t;
    cout << "destroying " << count << " entities" << endl;

    for (auto e : entities) {
      e.destroy();
    }
  }

  SECTION("TestCreateEntitiesWithListener") {
    Listener listen;
    ev.subscribe<EntityCreatedEvent>(listen);

    uint64_t count = 10000000L;

    AutoTimer t;
    cout << "creating " << count << " entities while notifying a single EntityCreatedEvent listener" << endl;

    vector<Entity> entities;
    for (uint64_t i = 0; i < count; i++) {
      entities.push_back(em.create());
    }
  }

  SECTION("TestDestroyEntitiesWithListener") {
    Listener listen;
    ev.subscribe<EntityDestroyedEvent>(listen);

    uint64_t count = 10000000L;
    vector<Entity> entities;
    for (uint64_t i = 0; i < count; i++) {
      entities.push_back(em.create());
    }

    AutoTimer t;
    cout << "destroying " << count << " entities" << endl;

    for (auto &e : entities) {
      e.destroy();
    }
  }

  SECTION("TestEntityIteration") {
    uint64_t count = 10000000L;
    vector<Entity> entities;
    for (uint64_t i = 0; i < count; i++) {
      auto e = em.create();
      e.assign<Position>();
      entities.push_back(e);
    }

    AutoTimer t;
    cout << "iterating over " << count << " entities with a component 10 times" << endl;

    for (int i = 0; i < 10; ++i) {
      Position *position;
      for (auto e : em.entities_with_components<Position>(position)) {
      }
    }
  }
}
