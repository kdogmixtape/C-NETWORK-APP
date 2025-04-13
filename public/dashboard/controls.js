function handleReadyUp() {
  // send ready up message
  // set state here or update page to show ready up
  // wait for updates
}

function handleLogout() {
  // send any close or stop messages to server
  window.location.href = "/";
}

function handleConfirmPosition() {
  if (websocket.readyState != WebSocket.OPEN) {
    console.log("Error: connection to server failed");
    return;
  }

  let message = new Uint8Array(65);

  for (let i = 0; i < 8; i++) {
    for (let j = 0; j < 8; j++) {
      message[i * 8 + j + 1] = playerBoard.board[i][j];
    }
  }
  message[0] = 0x20;

  websocket.send(message);
}

function handleResign() {}

function handleSendShot() {}
