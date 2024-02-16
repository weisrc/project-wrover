import { requestEmitter, responseEmitter } from "@/lib/common";
import { useEffect, useState } from "react";
import { Input } from "./ui/input";

export function RemoteView() {
  const [captureUrl, setCaptureUrl] = useState("");

  useEffect(() => {
    requestEmitter.emit("beginCamera", {});

    responseEmitter.on("binaryData", (data) => {
      const url = URL.createObjectURL(data);
      setCaptureUrl(url);
    });
  }, []);

  return (
    <div>
      Camera view
      <img src={captureUrl} />
      <Input
        type="number"
        onChange={(e) => {
          requestEmitter.emit("setCameraFPS", {
            fps: +e.currentTarget.value,
          });
        }}
      />
    </div>
  );
}
