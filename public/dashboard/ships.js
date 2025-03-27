class Ship {
  constructor(x, y, w, h, length, sheetStartY, spritesheet) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
    this.sheetStartY = sheetStartY;
    this.boardRow = 0;
    this.boardCol = 0;
    this.currentFrame = 0;
    this.frameCount = 4;
    this.length = length;
    this.spritesheet = spritesheet;
    this.direction = DIRECTION_UP;

    setInterval(() => this.changeAnimationFrame(), 500);
  }

  draw() {
    CTX.save();
    CTX.rotate((this.direction * 90 * Math.PI) / 180);
    CTX.drawImage(
      this.spritesheet,
      this.w * this.currentFrame,
      this.sheetStartY,
      this.w,
      this.h,
      this.x,
      this.y,
      this.w,
      this.h,
    ); // TODO: factor in rotation
    CTX.restore();
  }

  changeAnimationFrame() {
    this.currentFrame += 1;
    this.currentFrame %= this.frameCount;
  }

  getPlacePoints() {
    let midX = this.x + this.w / 2;
    let midY = this.y + this.h / 2;
    let placeX = this.x + UNIT_SIZE / 2;
    let placeY = this.y + UNIT_SIZE / 2;

    return { midX, midY, placeX, placeY };
  }
  rotate() {
    this.direction += 1;
    this.direction %= NUM_DIRECTIONS;
  }
}

function createShips() {
  let ships = [];
  const lengths = [1, 2, 3, 3, 4, 5];
  const sheetStarts = [];

  var current = 0;
  for (let i = 0; i < 5; i++) {
    current += UNIT_SIZE * i;
    sheetStarts.push(current);
  }
  for (let i = 0; i < lengths.length; i++) {
    ships.push(
      new Ship(
        BOARD_OFFSET_X + BOARD_UNIT_SIZE * i,
        BOARD_OFFSET_Y,
        BOARD_UNIT_SIZE,
        BOARD_UNIT_SIZE * lengths[i],
        lengths[i],
        sheetStarts[lengths[i] - 1],
        battleshipSpritesheet,
      ),
    );
  }

  return ships;
}
