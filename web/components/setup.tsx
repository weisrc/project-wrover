/**
 * @author Wei
 * Setup component
 */

"use client";

import { useState } from "react";
import { NetworkSetup } from "./network-setup";
import { SerialSetup } from "./serial-setup";

export function Setup() {
  const [serialConnected, setSerialConnected] = useState(false);

  return (
    <>
      {serialConnected ? (
        <NetworkSetup />
      ) : (
        <SerialSetup onConnectionChange={setSerialConnected} />
      )}
    </>
  );
}
