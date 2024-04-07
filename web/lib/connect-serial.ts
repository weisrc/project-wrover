import { requestEmitter, responseEmitter } from "./common";

async function tryRequestPort() {
  try {
    return await navigator.serial.requestPort();
  } catch {
    return null;
  }
}

export async function connectSerial(
  onConnectionChange: (connected: boolean) => void
) {
  const port = await tryRequestPort();

  if (!port) {
    return
  }
  
  await port.open({ baudRate: 115200 });
  if (!port.writable) {
    console.error("Port is not writable");
    return;
  }
  if (!port.readable) {
    console.error("Port is not readable");
    return;
  }

  onConnectionChange(true);

  const textEncoder = new TextEncoderStream();
  const textDecoder = new TextDecoderStream();
  textEncoder.readable.pipeTo(port.writable);
  port.readable.pipeTo(textDecoder.writable);
  const writer = textEncoder.writable.getWriter();
  const reader = textDecoder.readable.getReader();

  function tap(event: string, data: object) {
    writer.write(JSON.stringify({ type: event, ...data }) + "\n");
  }

  requestEmitter.tap(tap);

  let backlog = "";

  while (port.readable) {
    try {
      while (true) {
        const { value, done } = await reader.read();

        if (done) {
          break;
        }

        backlog += value;
        const lines = backlog.split("\n");
        backlog = lines.pop()!;
        for (const line of lines) {
          try {
            const data = JSON.parse(line);
            console.log(data);
            responseEmitter.emit(data.type, data);
          } catch { }
        }
      }
    } catch (error) {
      console.error(error);
      break;
    } finally {
      reader.releaseLock();
    }
  }

  requestEmitter.untap(tap);
  onConnectionChange(false);
}
