local common = import "common.jsonnet";

function(params) {
  [if "id" in params then "id"]: params.id,
  components: common.checkParams(params, ["id", "pos", "orient"]) {
    Activation: {
      speed: 4
    },
    Animation: {
      speed: 0
    },
    AnimSpriteRendering: {
      sprite: "switch 0",
      frames: 5,
      depth: common.getDepth("switch")
    },
    Collision: {},
    PowerOutput: {},
    Switch: {},
    Transform: {},
    PhysicsBody: common.getOrientTransform(params) + {
      body: (import "sensor body.json")
    }
  }
}
