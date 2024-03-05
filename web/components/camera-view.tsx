import { responseEmitter } from "@/lib/common";
import { useState, useEffect, HTMLAttributes } from "react";

export function CameraView(props: HTMLAttributes<HTMLImageElement>) {
    const [captureUrl, setCaptureUrl] = useState("");

    useEffect(() => {

        const onBinaryData = (data: Blob) => {
            const url = URL.createObjectURL(data);
            setCaptureUrl(url);
        }

        responseEmitter.on("binaryData", onBinaryData);

        return () => {
            responseEmitter.off("binaryData", onBinaryData);
        }
    }, []);

    return <img src={captureUrl} {...props} />

}