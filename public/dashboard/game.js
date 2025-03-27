var gameRunning = true;

function initCanvas() {
  if (!canvas.getContext) {
    console.error("Error: canvas not supported in this browser");
    return;
  }

  CTX = canvas.getContext("2d");

  // start the game loop
  gameIntervalId = setInterval(() => gameloop(), 1 / FPS);
}

// all position updates, frame updates should happen here (no draws)
function update() {
  cursor.update();
}

// all drawing should happen here
function draw() {
  // draw background
  CTX.save();
  CTX.globalAlpha = 1;
  CTX.fillStyle = BACKGROUND_COL;
  CTX.fillRect(0, 0, canvas.width, canvas.height);
  CTX.restore();

  playerBoard.draw();
  drawShotBoard();

  ships.forEach((ship) => {
    ship.draw();
  });
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

  CTX.strokeStyle = BOARD_LINES_COL;
  CTX.globalAlpha = 0.4;
  // draw vertical gridlines
  for (let col = 0; col < BOARD_DIM + 1; col++) {
    CTX.beginPath();
    CTX.moveTo(
      col * SHOT_BOARD_UNIT_SIZE + SHOT_BOARD_OFFSET_X,
      SHOT_BOARD_OFFSET_Y,
    );
    CTX.lineTo(
      col * SHOT_BOARD_UNIT_SIZE + SHOT_BOARD_OFFSET_X,
      SHOT_BOARD_OFFSET_Y + SHOT_BOARD_HEIGHT,
    );
    CTX.stroke();
  }
  // draw horizontal gridlines
  for (let row = 0; row < BOARD_DIM + 1; row++) {
    CTX.beginPath();
    CTX.moveTo(
      SHOT_BOARD_OFFSET_X,
      row * SHOT_BOARD_UNIT_SIZE + SHOT_BOARD_OFFSET_Y,
    );
    CTX.lineTo(
      SHOT_BOARD_OFFSET_X + SHOT_BOARD_WIDTH,
      row * SHOT_BOARD_UNIT_SIZE + SHOT_BOARD_OFFSET_Y,
    );
    CTX.stroke();
  }
  CTX.restore();
}
