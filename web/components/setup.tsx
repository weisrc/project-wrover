"use client";

import {
  Card,
  CardContent,
  CardDescription,
  CardHeader,
  CardTitle,
} from "@/components/ui/card";

import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs";
import { NAME } from "@/lib/constants";
import Link from "next/link";
import { Button } from "./ui/button";

export function VisibleNetworkSetup() {
  return "Visible Network Setup";
}

export function HiddenNetworkSetup() {
  return "Hidden Network Setup";
}

async function connectSerial() {
  const port = await navigator.serial.requestPort();
  await port.open({ baudRate: 115200 });
  if (!port.writable) {
    console.error("Port is not writable");
    return;
  }
  if (!port.readable) {
    console.error("Port is not readable");
    return;
  }
  const textEncoder = new TextEncoderStream();
  const textDecoder = new TextDecoderStream();
  const writableStreamClosed = textEncoder.readable.pipeTo(port.writable);
  const readableStreamClosed = port.readable.pipeTo(textDecoder.writable);
  const writer = textEncoder.writable.getWriter();
  const reader = textDecoder.readable.getReader();

  await writer.write("\n");

  while (port.readable) {
    try {
      while (true) {
        const { value, done } = await reader.read();
        if (done) {
          console.log("Stream complete");
          break;
        }
        console.log(value);
      }
    } catch (error) {
      console.error(error);
    } finally {
      reader.releaseLock();
    }
  }
}

export function WifiSetup() {
  return (
    <Card className="border-none shadow-none w-[500px]">
      <CardHeader>
        <CardTitle>{NAME} WiFi Setup</CardTitle>
        <CardDescription>
          Connect the {NAME} to wireless network.
        </CardDescription>
      </CardHeader>
      <CardContent>
        <Tabs defaultValue="visible">
          <TabsList>
            <TabsTrigger value="visible">Visible Network</TabsTrigger>
            <TabsTrigger value="hidden">Hidden Network</TabsTrigger>
          </TabsList>
          <TabsContent value="visible">
            <VisibleNetworkSetup />
          </TabsContent>
          <TabsContent value="hidden">
            <HiddenNetworkSetup />
          </TabsContent>
        </Tabs>
      </CardContent>
    </Card>
  );
}

export function SerialSetup() {
  return (
    <Card className="border-none shadow-none w-[500px]">
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
          onClick={() => {
            connectSerial();
          }}
        >
          Connect
        </Button>
      </CardContent>
    </Card>
  );
}

export function Setup() {
  return (
    <div className="flex flex-col items-center h-screen pt-16">
      <WifiSetup />
      <SerialSetup />
    </div>
  );
}
