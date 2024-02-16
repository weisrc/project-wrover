import { requestEmitter, responseEmitter } from "./common";

export async function connectWebSocket(ip: string) {
    const ws = new WebSocket(`ws://${ip}/ws`)

    function tap(event: string, data: object) {
        ws.send(JSON.stringify({ type: event, ...data }));
    }

    requestEmitter.tap(tap);

    ws.onmessage = (event) => {
        if (typeof event.data === "string") {
            try {
                const data = JSON.parse(event.data);
                responseEmitter.emit(data.type, data)
            } catch (e) {
                console.error(e)
            }
        } else {
            responseEmitter.emit("binaryData", event.data);
        }
    }


    return await Promise.race([
        responseEmitter.wait("socketReady"),
        new Promise((_resolve, reject) => {
            ws.onerror = reject
        })
    ])
}