console.log("Establishing websocket connection...");
const websocket = new WebSocket("ws://localhost:4242/ws");

// Must send after connections are open
websocket.onopen = (event) => {
  console.log("Websocket successfully connected");
};

websocket.onmessage = (event) => {
  console.log("Message received!");
  console.log(event.data);
};

websocket.onerror = (event) => {
  console.log("Connection error");
  console.log(event);
};

websocket.onclose = (event) => {
  console.log("Connection closed");
  console.log(event);
};

let board = [
  ["0", "0", "1", "0", "0", "0", "0", "0"],
  ["0", "0", "0", "0", "0", "0", "0", "0"],
  ["0", "0", "0", "0", "0", "0", "0", "0"],
  ["0", "0", "0", "0", "0", "0", "0", "0"],
  ["0", "0", "0", "0", "0", "0", "0", "0"],
  ["3", "0", "0", "0", "0", "0", "0", "0"],
  ["3", "0", "0", "0", "0", "0", "0", "0"],
  ["3", "0", "0", "0", "0", "0", "2", "2"],
];

let message = new Uint8Array(65);

for (let i = 0; i < 8; i++) {
  for (let j = 0; j < 8; j++) {
    message[i * 8 + j + 1] = board[i][j];
  }
}
message[0] = 0x20;
