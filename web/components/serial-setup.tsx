/**
 * @author Wei
 * SerialSetup component to connect to serial port.
 */

"use client";

import {
  Card,
  CardContent,
  CardDescription,
  CardHeader,
  CardTitle,
} from "@/components/ui/card";

import { Button } from "@/components/ui/button";
import { NAME } from "@/lib/common";
import Link from "next/link";

import { connectSerial } from "@/lib/connect-serial";
import { useRouter } from "next/navigation";
import { useEffect, useState } from "react";

export function SerialSetup(props: {
  onConnectionChange: (connected: boolean) => void;
}) {

  const router = useRouter();
  const [serialSupported, setSerialSupported] = useState(false);

  useEffect(() => {
    setSerialSupported(!!navigator.serial)
  })

  return (
    <Card className="border-none shadow-none">
      <CardHeader>
        <CardTitle>{NAME} Serial Setup</CardTitle>
        <CardDescription>
          Connect the {NAME} to a serial port. Please make sure you are using a{" "}
          <Link href="https://caniuse.com/web-serial" className="underline">
            Web Serial API
          </Link>{" "}
          compatible browser.{" "}
          {serialSupported ? "This browser supports the Web Serial API." : "Unfortunately, your browser doesn't support it. Please use a different browser."}
        </CardDescription>
      </CardHeader>
      <CardContent>
        <Button
          className="mr-1"
          disabled={!serialSupported}
          onClick={async () => {
            await connectSerial(props.onConnectionChange);
          }}
        >
          Start Serial Setup
        </Button>
        <Button onClick={() => router.replace("/remote")}>
          Remote Connection
        </Button>
      </CardContent>
    </Card>
  );
}
