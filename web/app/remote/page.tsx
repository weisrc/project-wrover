"use client";

import { RemoteConnectCard } from "@/components/remote-connect-card";
import { RemoteView } from "@/components/remote-view";
import { RoverAsciiArt } from "@/components/rover-ascii-art";
import { Suspense, useState } from "react";

export default function Remote() {
  const [connected, setConnected] = useState(false);

  return (
    <>
      {connected ? (
        <RemoteView />
      ) : (
        <>
          <Suspense fallback={<div>Loading...</div>}>
            <RemoteConnectCard onConnectionChange={setConnected} />
          </Suspense>
          <RoverAsciiArt />
        </>
      )}
    </>
  );
}
