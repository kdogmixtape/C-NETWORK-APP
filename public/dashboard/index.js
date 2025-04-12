console.log("Connecting to server...");
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
