"use client";

import { RemoteConnectCard } from "@/components/remote-connect-card";
import { RemoteView } from "@/components/remote-view";
import { useState } from "react";

export default function Remote() {
  const [connected, setConnected] = useState(false);

  return (
    <div className="flex flex-col items-center h-screen pt-16">
      {connected ? (
        <RemoteView />
      ) : (
        <RemoteConnectCard onConnectionChange={setConnected} />
      )}
    </div>
  );
}
