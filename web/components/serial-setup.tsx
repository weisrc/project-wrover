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
import { Alert, AlertDescription, AlertTitle } from "./ui/alert";
import { AlertCircle } from "lucide-react";

export function SerialSetup(props: {
  onConnectionChange: (connected: boolean) => void;
}) {
  const router = useRouter();
  const [serialSupported, setSerialSupported] = useState(true);

  useEffect(() => {
    setSerialSupported(!!navigator.serial);
  }, []);

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
          {!serialSupported && (
            <Alert variant="destructive" className="mt-3">
              <AlertCircle className="h-4 w-4" />
              <AlertTitle>Error</AlertTitle>
              <AlertDescription>
                Your browser does not support the Web Serial API. Please use a
                compatible browser.
              </AlertDescription>
            </Alert>
          )}
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
