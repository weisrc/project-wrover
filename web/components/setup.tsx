"use client";

import { useState } from "react";
import { NetworkSetup } from "./network-setup";
import { SerialSetup } from "./serial-setup";

export function Setup() {
  const [serialConnected, setSerialConnected] = useState(false);

  return (
    <div className="flex flex-col items-center h-screen pt-16">
      {serialConnected ? (
        <NetworkSetup />
      ) : (
        <SerialSetup onConnectionChange={setSerialConnected} />
      )}
    </div>
  );
}
