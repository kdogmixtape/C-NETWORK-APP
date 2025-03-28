class PlayerBoard {
  constructor() {
    this.board = Array(BOARD_DIM)
      .fill()
      .map(() => Array(BOARD_DIM).fill(false));
  }

  draw() {
    CTX.save();

    /* another way to draw the board
    CTX.strokeStyle = BOARD_COL;

    // draw vertical gridlines
    for (let col = 0; col < BOARD_DIM + 1; col++) {
      CTX.beginPath();
      CTX.moveTo(col * BOARD_UNIT_SIZE + BOARD_OFFSET_X, BOARD_OFFSET_Y);
      CTX.lineTo(
        col * BOARD_UNIT_SIZE + BOARD_OFFSET_X,
        BOARD_OFFSET_Y + BOARD_HEIGHT,
      );
      CTX.stroke();
    }
    // draw horizontal gridlines
    for (let row = 0; row < BOARD_DIM + 1; row++) {
      CTX.beginPath();
      CTX.moveTo(BOARD_OFFSET_X, row * BOARD_UNIT_SIZE + BOARD_OFFSET_Y);
      CTX.lineTo(
        BOARD_OFFSET_X + BOARD_WIDTH,
        row * BOARD_UNIT_SIZE + BOARD_OFFSET_Y,
      );
      CTX.stroke();
    }
    */

    CTX.fillStyle = BOARD_COL;
    const padding = 2;
    for (let row = 0; row < BOARD_DIM; row++) {
      for (let col = 0; col < BOARD_DIM; col++) {
        CTX.fillRect(
          BOARD_OFFSET_X + row * BOARD_UNIT_SIZE + padding,
          BOARD_OFFSET_Y + col * BOARD_UNIT_SIZE + padding,
          BOARD_UNIT_SIZE - padding * 2,
          BOARD_UNIT_SIZE - padding * 2,
        );
      }
    }
    CTX.restore();
  }

  /**
   * helper for forcing a board position from an x, y position
   * returns { boardRow, boardCol } on board relative to top left corner
   *
   * 1 2 ... 8
   * 2
   * ...
   * 8
   *
   */
  getBoardPosFromXY(x, y) {
    let boardXPosPixels = Math.max(x - BOARD_OFFSET_X, 0);
    let boardYPosPixels = Math.max(y - BOARD_OFFSET_Y, 0);
    let row = Math.floor(boardXPosPixels / BOARD_UNIT_SIZE);
    let col = Math.floor(boardYPosPixels / BOARD_UNIT_SIZE);

    row = Math.min(row, BOARD_DIM - 1);
    col = Math.min(col, BOARD_DIM - 1);

    return { boardRow: row, boardCol: col };
  }
}
