class Button {
  constructor(x, y, w, h, text, callback) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
    this.text = text;
    this.callback = callback;
    this.isClicked = false;
    this.isHover = false;
  }

  draw() {
    CTX.save();
    CTX.fillStyle = BUTTON_COL;
    CTX.fillRect(this.x, this.y, this.w, this.h);
    CTX.font = "8px serif";
    CTX.fillStyle = TEXT_COL;
    CTX.fillText(this.text, this.x, this.y);
    CTX.restore();
  }

  click() {
    this.isClicked = true;
    setTimeout(() => (this.isClicked = false), 50);
    callback();
  }
}

class UI {
  constructor() {
    this.buttons = [];
    this.menus = [];
    this.currentMenu = 0;
  }

  drawMenu() {}

  draw() {
    this.buttons.forEach((b) => b.draw());
  }
}
