import { requestEmitter, responseEmitter } from "@/lib/common";
import { useState, useEffect, HTMLAttributes } from "react";

let emitCaptureWaiting = false;

export function CameraView(props: HTMLAttributes<HTMLImageElement>) {
    const [captureUrl, setCaptureUrl] = useState("");

    useEffect(() => {

        let emitCaptureTimeout: NodeJS.Timeout;

        function requestCapture() {
            if (emitCaptureWaiting) {
                return;
            }
            clearTimeout(emitCaptureTimeout);
            requestEmitter.emit("capture", {});
            emitCaptureWaiting = true;
            emitCaptureTimeout = setTimeout(() => {
                requestCapture();
                emitCaptureWaiting = false;
            }, 1000)
        }

        const onBinaryData = (data: Blob) => {
            const url = URL.createObjectURL(data);
            setCaptureUrl(url);
            emitCaptureWaiting = false;
            requestCapture();
        }

        requestCapture();

        responseEmitter.on("binaryData", onBinaryData);

        return () => {
            responseEmitter.off("binaryData", onBinaryData);
        }
    }, []);

    return <img src={captureUrl} {...props} />

}