/**
 * @author Wei
 * CameraView component
 */

import { requestEmitter, responseEmitter } from "@/lib/common";
import { useState, useEffect, HTMLAttributes } from "react";

export function CameraView(props: HTMLAttributes<HTMLImageElement>) {
  const [captureUrl, setCaptureUrl] = useState("");

  useEffect(() => {
    let run = true;

    async function start() {
      while (run) {
        requestEmitter.emit("capture", {});
        const data = await Promise.race([
          responseEmitter.wait("binaryData"),
          new Promise<undefined>((resolve) => setTimeout(resolve, 100)),
        ]);
        if (data) {
          const url = URL.createObjectURL(data);
          URL.revokeObjectURL(captureUrl); // revoke the previous object URL to free up space
          setCaptureUrl(url);
        }
      }
    }

    start();

    return () => {
      run = false;
      URL.revokeObjectURL(captureUrl);
    };
  }, []);

  return <img src={captureUrl} {...props} />;
}
