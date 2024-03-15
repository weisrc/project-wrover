import { requestEmitter, responseEmitter } from "./common";

let singleton: Promise<void> | undefined;

export async function connectWebSocket(ip: string) {
  if (singleton) {
    return singleton;
  }
  singleton = unsafeConnectWebSocket(ip);
  return singleton;
}

async function unsafeConnectWebSocket(ip: string) {

  console.log("connect web socket", ip);
  const ws = new WebSocket(`ws://${ip}/ws`);

  function tap(event: string, data: object) {
    ws.send(JSON.stringify({ type: event, ...data }));
  }

  requestEmitter.tap(tap);

  ws.onmessage = (event) => {
    if (typeof event.data === "string") {

      try {
        const data = JSON.parse(event.data);
        responseEmitter.emit(data.type, data);
      } catch (e) {
        console.error(e);
      }
    } else {
      responseEmitter.emit("binaryData", event.data);
    }
  };

  await Promise.race([
    responseEmitter.wait("socketReady"),
    new Promise((_resolve, reject) => {
      ws.onerror = reject;
    }),
  ]);

  await new Promise((resolve) => setTimeout(resolve, 100));

}
