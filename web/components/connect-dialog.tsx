/**
 * @author Wei
 * ConnectDialog component
 */

"use client";

import { Button } from "@/components/ui/button";
import {
  Dialog,
  DialogContent,
  DialogDescription,
  DialogHeader,
  DialogTitle,
  DialogTrigger,
} from "@/components/ui/dialog";

import {
  AUTH_MODE_KEYS,
  AUTH_MODE_NAMES,
  AuthMode,
  NetworkItem,
} from "@/lib/types";
import { NetworkConnectionForm } from "./network-connection-form";

export function ConnectDialog({ network }: { network: NetworkItem }) {
  const [ssid, rssi, auth, channel, bssid] = network;

  return (
    <Dialog>
      <DialogTrigger asChild>
        <Button variant="ghost" size="sm">
          {ssid}
        </Button>
      </DialogTrigger>
      <DialogContent className="sm:max-w-[425px]">
        <DialogHeader>
          <DialogTitle>
            Connect to <span className="font-bold">{ssid}</span>
          </DialogTitle>
          <DialogDescription>
            {rssi} dBm, {AUTH_MODE_NAMES[auth]}, Channel {channel}, BSSID{" "}
            {bssid}
          </DialogDescription>
        </DialogHeader>
        <NetworkConnectionForm
          ssid={network[0]}
          auth={AUTH_MODE_KEYS[auth] as AuthMode}
        />
      </DialogContent>
    </Dialog>
  );
}
