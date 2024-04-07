"use client";

import {
  Card,
  CardContent,
  CardDescription,
  CardHeader,
  CardTitle,
} from "@/components/ui/card";

import { NAME } from "@/lib/common";
import { connectWebSocket } from "@/lib/connect-web-socket";
import { useSearchParams } from "next/navigation";
import { useRouter } from "next/navigation";
import { useEffect, useState } from "react";
import { Button } from "./ui/button";
import { Input } from "./ui/input";
import { Label } from "./ui/label";


export function RemoteConnectCard(props: {
  onConnectionChange: (connected: boolean) => void;
}) {

  const params = useSearchParams()
  const router = useRouter()
  const [ip, setIp] = useState(params.get("ip") ?? "");
  const [connecting, setConnecting] = useState(false);
  const [failed, setFailed] = useState(false)

  async function connect(ip: string) {
    try {
      setFailed(false)
      setConnecting(true)
      await connectWebSocket(ip)
      setConnecting(false)
      props.onConnectionChange(true);
    } catch {
      setFailed(true)
      setConnecting(false)
    }
  }

  useEffect(() => {
    console.log("use effect running...");
    if (ip) {
      connect(ip);
    }
  }, [])

  return (
    <Card className="border-none shadow-none">
      <CardHeader>
        <CardTitle>{NAME} Remote Connection</CardTitle>
        <CardDescription>
          Connecting to the {NAME} remotely. Please make sure you have selected the right network.
        </CardDescription>
      </CardHeader>

      <CardContent>
        {failed && (
          <div>
            Connection Failed!
          </div>
        )}

        <Label>{NAME} IP Address</Label>
        <Input
          disabled={connecting}
          className="mb-4 mt-1"
          placeholder="IP Address"
          value={ip} onChange={(e) => setIp(e.currentTarget.value)} />
        <Button className="mr-1" onClick={() => router.replace("/")}>
          Return to Setup
        </Button>
        <Button disabled={connecting || !ip}
          onClick={() => {
            connect(ip);
            const search = new URLSearchParams();
            search.append("ip", ip);
            router.replace("?" + search.toString());
          }}
        >
          {connecting ? "Connecting..." : "Connect"}
        </Button>
      </CardContent>
    </Card>
  );
}
