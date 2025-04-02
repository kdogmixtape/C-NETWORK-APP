var gameRunning = true;

function initCanvas() {
  if (!canvas.getContext) {
    console.error("Error: canvas not supported in this browser");
    return;
  }

  CTX = canvas.getContext("2d");

  // start the game loop
  LAST_FRAME_TIME = new Date().getTime();
  gameIntervalId = setInterval(() => gameloop(), 1 / FPS);
}

// all position updates, frame updates should happen here (no draws)
function update() {
  updateWaves();
  cursor.update();

  LAST_FRAME_TIME = new Date().getTime();
}

// all drawing should happen here
function draw() {
  // draw background
  CTX.save();
  CTX.fillStyle = BACKGROUND_COL;
  CTX.fillRect(0, 0, canvas.width, canvas.height);
  CTX.restore();

  playerBoard.draw();
  drawShotBoard();
  drawWaves();

  ships.forEach((ship) => {
    ship.draw();
  });
  ui.draw();
  cursor.draw();
}

function gameloop() {
  if (!gameRunning) {
    return;
  }

  draw();
  update();
}

function drawShotBoard() {
  CTX.save();
  CTX.fillStyle = BOARD_COL;
  const padding = 1;
  for (let row = 0; row < BOARD_DIM; row++) {
    for (let col = 0; col < BOARD_DIM; col++) {
      CTX.fillRect(
        SHOT_BOARD_OFFSET_X + row * SHOT_BOARD_UNIT_SIZE + padding,
        SHOT_BOARD_OFFSET_Y + col * SHOT_BOARD_UNIT_SIZE + padding,
        SHOT_BOARD_UNIT_SIZE - padding * 2,
        SHOT_BOARD_UNIT_SIZE - padding * 2,
      );
    }
  }

  CTX.restore();
}

function drawWaves() {
  for (let i = 0; i < waves.length; i++) {
    const { x, y, img } = waves[i];
    CTX.drawImage(img, x, y);
  }
}

function updateWaves() {
  // move waves from top left to bottom right
  for (let i = 0; i < waves.length; i++) {
    waves[i].x += WAVE_SPEED * getDeltaTime();
    waves[i].y += WAVE_SPEED * getDeltaTime();

    // if out of bounds, remove
    if (waves[i].x > canvas.width && waves[i].y > canvas.height) {
      let temp = waves[i];
      waves[i] = waves[waves.length - 1];
      waves[waves.length - 1] = temp;

      waves.pop();
    }
  }
}

const waveIntervalId = setInterval(
  () => {
    let img = wavesImgs[Math.ceil(Math.random() * 100) % wavesImgs.length];
    waves.push({
      x: 0 - Math.random() * 500 - img.width,
      y: 0 - Math.random() * 500 - img.height,
      img: img,
    });
  },
  Math.max(Math.random() * 4000, 2000),
);
