class Ship {
  constructor(x, y, w, h, length, sheetStartY, spritesheet) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
    this.collisionW = w;
    this.collisionH = h;
    this.initialW = w;
    this.initialH = h;
    this.sheetStartY = sheetStartY;
    this.boardRow = 0;
    this.boardCol = 0;
    this.currentFrame = 0;
    this.length = length;
    this.spritesheet = spritesheet;
    this.direction = DIRECTION_DOWN;
  }

  draw() {
    CTX.save();
    CTX.translate(this.x + BOARD_UNIT_SIZE / 2, this.y + BOARD_UNIT_SIZE / 2);
    CTX.rotate((this.direction * -90 * Math.PI) / 180);
    CTX.translate(-this.x - BOARD_UNIT_SIZE / 2, -this.y - BOARD_UNIT_SIZE / 2);
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
    );

    CTX.restore();

    // for debugging values
    if (DEBUG_MODE) {
      CTX.strokeStyle = "green";
      CTX.strokeRect(this.x, this.y, this.collisionW, this.collisionH);
      CTX.strokeStyle = "red";
      CTX.strokeRect(this.x, this.y, this.w, this.h);
      console.log(
        this.x,
        this.y,
        this.w,
        this.h,
        this.collisionW,
        this.collisionH,
      );
    }
  }

  getPlacePoints() {
    let midX = this.x + this.w / 2;
    let midY = this.y + this.h / 2;
    let placeX = this.x + BOARD_UNIT_SIZE / 2;
    let placeY = this.y + BOARD_UNIT_SIZE / 2;

    return { midX, midY, placeX, placeY };
  }

  rotate() {
    this.direction += 1;
    this.direction %= NUM_DIRECTIONS;
    this.currentFrame = this.direction;
    if (this.direction == DIRECTION_DOWN) {
      this.collisionW = this.initialW;
      this.collisionH = this.initialH;
    } else {
      this.collisionW = this.initialH;
      this.collisionH = this.initialW;
    }
  }

  getBounds() {
    let x1, x2, y1, y2;
    x1 = this.x;
    x2 = this.x + this.collisionW;
    y1 = this.y;
    y2 = this.y + this.collisionH;

    return { x1, x2, y1, y2 };
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
