local e = import "entities.jsonnet";

[
  e.makePlatform({
    pos: [-16, -9],
    size: [32, 7]
  }),
  e.makePlatform({
    pos: [-16, 2],
    size: [32, 7]
  }),
  e.makePlatform({
    pos: [-16, -2],
    size: [1, 4]
  }),
  e.makePlatform({
    pos: [15, -2],
    size: [1, 4]
  }),
  e.makePlayer({
    pos: [-10, -2]
  }),
  e.makeExit({
    pos: [14, -2]
  }),
  e.makeBox({
    pos: [-13, -2]
  }),
  e.makeButton({
    id: 0,
    pos: [-2, -2]
  }),
  e.makeSwitch({
    id: 1,
    pos: [1, -2]
  }),
  e.makeDoor({
    "in": 1,
    pos: [5.5, 2],
    size: [1, 4],
    rotation: 180
  }),
  e.makeDoor({
    "in": 0,
    pos: [8.5, 2],
    size: [1, 4],
    rotation: 180
  })
]
