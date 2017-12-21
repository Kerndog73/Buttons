local common = import "common.jsonnet";

local validParams = ["start", "end", "size", "on", "in", "operator", "speed", "waiting time", "piston"];

function(params) {
  components: common.checkParams(params, validParams) {
    Activation: {},
    StaticSpriteRendering: {
      sprite: "moving platform"
    },
    Transform: {},
    PhysicsBody: common.getPosSize(params) + {
      body: "platform body.json",
      pos: params.start
    },
    PowerInput: common.getPowerInput(params),
    MovingPlatform: {
      start: params.start,
      end: params.end,
      [if "speed" in params then "speed"]: params.speed,
      [if "waiting time" in params then "waiting time"]: params["waiting time"],
      [if "piston" in params then "piston"]: params["piston"]
    }
  }
}
