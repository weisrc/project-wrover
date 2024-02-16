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

export function SerialSetup(props: {
  onConnectionChange: (connected: boolean) => void;
}) {

  const router = useRouter();

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
          className="mr-1"
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
