"use client";

import { RemoteConnectCard } from "@/components/remote-connect-card";
import { RemoteView } from "@/components/remote-view";
import { RoverAsciiArt } from "@/components/rover-ascii-art";
import { useState } from "react";

export default function Remote() {
  const [connected, setConnected] = useState(false);

  return (
    <>
      {connected ? (
        <RemoteView />
      ) : (
        <>
          <RemoteConnectCard onConnectionChange={setConnected} />
          <RoverAsciiArt />
        </>
      )}
    </>
  );
}
