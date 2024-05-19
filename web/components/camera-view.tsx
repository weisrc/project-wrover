/**
 * @author Wei
 * CameraView component
 */

import { requestEmitter, responseEmitter } from "@/lib/common";
import { useState, useEffect, HTMLAttributes } from "react";

let emitCaptureWaiting = false;

export function CameraView(props: HTMLAttributes<HTMLImageElement>) {
    const [captureUrl, setCaptureUrl] = useState("");

    useEffect(() => {

        let emitCaptureTimeout: NodeJS.Timeout;

        function requestCapture() {
            if (emitCaptureWaiting) { // useEffect is called twice somehow unfortunately, debouncing
                return;
            }
            clearTimeout(emitCaptureTimeout);
            requestEmitter.emit("capture", {});
            emitCaptureWaiting = true;
            emitCaptureTimeout = setTimeout(() => { // resend if no response after 1 second
                requestCapture();
                emitCaptureWaiting = false;
            }, 1000)
        }

        const onBinaryData = (data: Blob) => {
            URL.revokeObjectURL(captureUrl); // revoke the previous object URL to free up space
            const url = URL.createObjectURL(data);
            setCaptureUrl(url);
            emitCaptureWaiting = false;
            requestCapture(); // request the next frame
        }

        requestCapture();

        responseEmitter.on("binaryData", onBinaryData);

        return () => {
            responseEmitter.off("binaryData", onBinaryData); // remove the event listener
        }
    }, []);

    return <img src={captureUrl} {...props} />

}