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

import { connectSerial } from "@/lib/serial";

export function SerialSetup(props: {
  onConnectionChange: (connected: boolean) => void;
}) {
  return (
    <Card className="border-none shadow-none">
      <CardHeader>
        <CardTitle>{NAME} Serial Setup</CardTitle>
        <CardDescription>
          Connect the {NAME} to a serial port. Please make sure you are using a{" "}
          <Link href="https://caniuse.com/web-serial" className="underline">
            Web Serial API
          </Link>{" "}
          compatible browser.
        </CardDescription>
      </CardHeader>
      <CardContent>
        <Button
          onClick={async () => {
            await connectSerial(props.onConnectionChange);
          }}
        >
          Connect
        </Button>
      </CardContent>
    </Card>
  );
}
