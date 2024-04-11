/**
 * @author Wei
 * VisibleNetworkSetup component
 */

"use client";

import { Button } from "@/components/ui/button";
import { requestEmitter, responseEmitter } from "@/lib/common";

import { NetworkItem } from "@/lib/types";
import { useEffect, useState } from "react";
import { NetworkTable, NetworkTableSkeleton } from "./network-table";

export function VisibleNetworkSetup() {
  const [scanning, setScanning] = useState(false);
  const [networks, setNetworks] = useState<NetworkItem[]>([]);

  async function scan() {
    setNetworks([]);
    setScanning(true);

    // requestEmitter.emit("disconnect", {});
    // await new Promise((res) => setTimeout(res, 3000));
    requestEmitter.emit("scan", {});
    const response = await responseEmitter.wait("scan");
    const uniqueNetworks: Record<string, NetworkItem> = {};
    for (const network of response.networks) {
      const [ssid, rssi] = network;
      if (uniqueNetworks[ssid] && uniqueNetworks[ssid][1] > rssi) {
        continue;
      }
      uniqueNetworks[ssid] = network;
    }
    setScanning(false);
    setNetworks(Object.values(uniqueNetworks));
  }

  useEffect(() => {
    scan();
  }, []);

  return (
    <div>
      <Button onClick={scan} className="mb-5" disabled={scanning}>
        {scanning ? "Scanning Networks..." : "Scan Networks"}
      </Button>
      {scanning ? (
        <NetworkTableSkeleton />
      ) : (
        <NetworkTable networks={networks} />
      )}
    </div>
  );
}
