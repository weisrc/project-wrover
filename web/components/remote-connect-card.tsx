/**
 * @author Wei
 * Remote Connect Card to connect with the IP address of the robot.
 */

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
import { useRouter, useSearchParams } from "next/navigation";
import { useEffect, useState } from "react";
import { Button } from "./ui/button";
import { Input } from "./ui/input";
import { Label } from "./ui/label";
import { AlertCircle } from "lucide-react";
import { Alert, AlertTitle, AlertDescription } from "./ui/alert";

export function RemoteConnectCard(props: {
  onConnectionChange: (connected: boolean) => void;
}) {
  const params = useSearchParams();
  const router = useRouter();
  const [ip, setIp] = useState(params.get("ip") ?? "");
  const [connecting, setConnecting] = useState(false);
  const [failed, setFailed] = useState(false);

  async function connect(ip: string) {
    try {
      setFailed(false);
      setConnecting(true);
      await connectWebSocket(ip);
      setConnecting(false);
      props.onConnectionChange(true);
    } catch {
      setFailed(true);
      setConnecting(false);
    }
  }

  useEffect(() => {
    console.log("use effect running...");
    if (ip) {
      connect(ip);
    }
  }, []);

  return (
    <>
      <Card className="border-none shadow-none">
        <CardHeader>
          <CardTitle>{NAME} Remote Connection</CardTitle>
          <CardDescription>
            Connecting to the {NAME} remotely. Please make sure you have
            selected the right network.
          </CardDescription>
        </CardHeader>

        <CardContent>
          {failed && (
            <Alert variant="destructive" className="mb-4">
              <AlertCircle className="h-4 w-4" />
              <AlertTitle>Error</AlertTitle>
              <AlertDescription>
                Failed to connect. Please check the IP address and try again.
              </AlertDescription>
            </Alert>
          )}

          <Label>{NAME} IP Address</Label>
          <Input
            disabled={connecting}
            className="mb-4 mt-1"
            placeholder="IP Address"
            value={ip}
            onChange={(e) => setIp(e.currentTarget.value)}
          />
          <Button className="mr-1" onClick={() => router.replace("/")}>
            Return to Setup
          </Button>
          <Button
            disabled={connecting || !ip}
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
    </>
  );
}
