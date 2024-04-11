/**
 * @author Wei
 * NetworkConnectionForm component
 * Form for connecting to a network
 */

"use client";

import { Button } from "@/components/ui/button";
import { requestEmitter, responseEmitter } from "@/lib/common";

import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from "@/components/ui/select";

import {
  AUTH_MODE_MAP,
  AuthMode,
  ConnectEventMethod
} from "@/lib/types";
import { useRouter } from "next/navigation";
import { useState } from "react";
import { InputField } from "./input-field";

export function NetworkConnectionForm({
  ssid,
  auth,
}: {
  ssid: string;
  auth: AuthMode;
}) {
  const [connecting, setConnecting] = useState(false);

  const [identity, setIdentity] = useState("");
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [method, setMethod] = useState<ConnectEventMethod>("peap");
  const router = useRouter();

  const authName = AUTH_MODE_MAP[auth];

  const needsIdentity = authName === "WPA2 Enterprise";
  const needsUsername = authName === "WPA2 Enterprise";
  const needsPassword = authName !== "OPEN";
  const needsMethod = authName === "WPA2 Enterprise";

  async function connect() {
    setConnecting(true);
    requestEmitter.emit("connect", {
      auth,
      ssid,
      identity,
      username,
      password,
      method,
    });
    while (true) {
      const { data: status } = await responseEmitter.wait("status");
      console.log("wifi status", status);
      if (status === "3") {
        break;
      }
    }
    requestEmitter.emit("ip", {});
    const { data: ip } = await responseEmitter.wait("ip");
    requestEmitter.emit("begin", {});
    setConnecting(false);
    router.replace("/remote?ip=" + ip)
  }

  return (
    <div>
      {needsMethod && (
        <Select
          value={method}
          onValueChange={(value) => setMethod(value as ConnectEventMethod)}
        >
          <SelectTrigger className="w-[180px]">
            <SelectValue />
          </SelectTrigger>
          <SelectContent>
            <SelectItem value="peap">PEAP</SelectItem>
            <SelectItem value="ttls">TTLS</SelectItem>
            <SelectItem value="tls">TLS</SelectItem>
          </SelectContent>
        </Select>
      )}
      {needsIdentity && (
        <InputField
          label="Identity"
          value={identity}
          onValueChange={setIdentity}
        />
      )}
      {needsUsername && (
        <InputField
          label="Username"
          value={username}
          onValueChange={setUsername}
        />
      )}
      {needsPassword && (
        <InputField
          label="Password"
          value={password}
          onValueChange={setPassword}
          type="password"
        />
      )}

      <Button className="mt-8" onClick={connect} disabled={connecting}>
        {connecting ? "Connecting..." : "Connect"}
      </Button>
    </div>
  );
}
